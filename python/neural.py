import nltk
import pickle
import json
import queue
from abc import abstractmethod
import sys
import threading
import warnings
from pathlib import Path
import threading


warnings.filterwarnings("ignore", category=DeprecationWarning)
warnings.filterwarnings("ignore", category=UserWarning)
langs = ['ru', 'en']
categories = ["entertainment", "society", "technology", "sports", "science", "economy", "other"]


def assets_path(base):
    return base + '../../assets/'


def out_path(base):
    return base + '../../out/'


class TGExecutor:
    def __init__(self, base, max_workers):
        self.cv = threading.Condition()
        self.q = []
        self.exit = False

        self.base = base
        self.threads = [threading.Thread(target=self._work, args=[base]) for _ in range(max_workers)]

    def start(self):
        for t in self.threads:
            t.start()

    def join(self):
        for t in self.threads:
            t.join()

    def shutdown(self):
        with self.cv:
            self.exit = True
            self.cv.notifyAll()
        self.join()

    def submit_data(self, data):
        with self.cv:
            self.q.append(data)
            self.cv.notify()

    def get_data(self, limit):
        data = []

        with self.cv:
            self.cv.wait_for(lambda: len(self.q) > 0 or self.exit)
            if not self.exit:
                data.append(self.q.pop())

        return data

    @abstractmethod
    def _work(self, base):
        pass


class TGServerExecutor(TGExecutor):
    def __init__(self, base, max_workers, lang):
        self.lang = lang
        self.base = base
        super(TGServerExecutor, self).__init__(base, max_workers)
        self.start()

    def _work(self, base):
        net_sys = NetSystem(self.base, self.lang)
        while True:
            datalist = self.get_data(10)
            net_sys.process(datalist)


class NetSystem:
    def __init__(self, base, lang):
        self.lang = lang
        self.base = base
        self.nets = TNetPack(base, lang)

    def process(self, datalist):
        if len(datalist) == 0:
            return
        names = [data[0] for data in datalist]
        texts = [self.nets.stemmer.stem(data[1]) for data in datalist]
        news_test = self.nets.news_net.predict(texts)

        names = [names[i] for i in range(len(texts)) if news_test[i]]
        texts = [texts[i] for i in range(len(texts)) if news_test[i]]

        try:
            if len(texts) > 0:
                categories_test = self.nets.class_net.predict(texts)
            else:
                return
        except Exception:
            print(self.lang + ':: Error: Categorization failed: ' + str(len(texts)))
            return

        for i in range(len(texts)):
            cat = categories[int(categories_test[i])]
            dump_info(self.base, {"file_name": names[i], "lang": self.lang, "category": cat})


class TNetPack:
    def __init__(self, base, lang):
        self.stemmer = TStemmer(lang)
        self.news_net = TNet(base, 'news', lang) if lang == 'ru' else ENNewsNet(base)
        self.class_net = TNet(base, 'categories', lang)


class SimNet:
    def __init__(self, base, net_type, lang):
        self.stemmer = TStemmer(lang)
        if net_type == 'news' and lang == 'en':
            self.net = ENNewsNet(base)
        else:
            self.net = TNet(base, net_type, lang)

    def predict(self, texts):
        texts = [self.stemmer.stem(text) for text in texts]
        return self.net.predict(texts)


class TStemmer:
    def __init__(self, lang):
        self.stemmer = get_stemmer(lang)

    def stem(self, text):
        return ' '.join(map(lambda w: self.stemmer.stem(w), text.split()))


class TNet:
    def __init__(self, base, net_type, lang):
        self.model = get_model(base, lang, net_type)
        self.vectorizer = get_vectorizer(base, lang, net_type)

    def predict(self, stemmed_texts):
        x_tfidf = self.vectorizer.transform(stemmed_texts)
        return self.model.predict(x_tfidf)


class ENNewsNet(TNet):
    def __init__(self, base):
        super(ENNewsNet, self).__init__(base, 'news', 'en')

    def predict(self, stemmed_texts):
        x_tfidf = self.vectorizer.transform(stemmed_texts)
        ans = (0.45 < self.model.predict_proba(x_tfidf)[:, 1:])
        return flatten(ans)


def flatten(lst):
    return [item for sublist in lst for item in sublist]


def form_path(base, lang, category, name):
    return out_path(base) + '/' + lang + '/' + category + '/' + name


def dump_info(base, js):
    path = form_path(base, js['lang'], js['category'], js['file_name'])
    with open(path, 'w') as file:
        file.write(json.dumps(js))


def get_stemmer(lang):
    if lang == 'ru':
        return nltk.stem.snowball.RussianStemmer()
    elif lang == 'en':
        return nltk.stem.snowball.EnglishStemmer()


def get_vectorizer(base, lang, net):
    return load(assets_path(base) + '/' + net + '/' + lang + '/vectorizer.pickle')


def get_model(base, lang, net):
    return load(assets_path(base) + '/' + net + '/' + lang + '/model.pickle')


def load(path):
    try:
        with open(path, 'rb') as file:
            return pickle.load(file)
    except Exception:
        print("ERROR: Failed to load: " + path)


def tokenize(s):
    return s.split()


class SimExecutor(TGExecutor):
    def __init__(self, base, max_workers, net_type, lang):
        self.lang = lang
        self.net_type = net_type
        self.result = queue.SimpleQueue()
        super(SimExecutor, self).__init__(base, max_workers)

    def get_data(self, limit):
        data = []

        with self.cv:
            if len(self.q) == 0:
                self.exit = True
            else:
                for i in range(limit):
                    if len(self.q) > 0:
                        data.append(self.q.pop())
                    else:
                        break

        return data

    def _work(self, base):
        net = SimNet(base, self.net_type, self.lang)
        res = []
        while not self.exit:
            texts = []
            files = self.get_data(10)

            if len(files) == 0:
                break

            for art in files:
                with open(art, 'r') as art_file:
                    texts.append(art_file.read())

            rs = net.predict(texts)
            for i in range(len(texts)):
                res.append([files[i].split('/')[-1], rs[i]])

        for r in res:
            self.result.put(r)


if __name__ == '__main__':
    base = str(Path(sys.argv[0]).parent) + '/'
    net_type = sys.argv[1]
    path = sys.argv[2]

    executors = {'ru': SimExecutor(base, 8, net_type, 'ru'), 'en': SimExecutor(base, 8, net_type, 'en')}

    with open(path, 'r') as file:
        js = json.load(file)

    for entry in js:
        lang = entry['lang_code']
        for article in entry['articles']:
            executors[lang].submit_data(article)

    for ex in executors:
        executors[ex].start()

    for ex in executors:
        executors[ex].join()

    if net_type == 'news':
        list_art = []
        for ex in executors:
            q = executors[ex].result
            while not q.empty():
                item = q.get()
                if item[1]:
                    list_art.append(item[0])

        result = {'articles': list_art}
    else:
        result = []
        mp = {}
        for cat in categories:
            mp[cat] = []

        for ex in executors:
            q = executors[ex].result
            while not q.empty():
                item = q.get()
                mp[categories[item[1]]].append(item[0])

        for cat in categories:
            if len(mp[cat]) > 0:
                result.append({"category": cat, "articles": mp[cat]})

    print(json.dumps(result, indent=2))
