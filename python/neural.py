import nltk
import pickle
import pandas as pd
import sys
import json
import queue
import threading


root_path = '/Users/dzhiblavi/Documents/prog/tgnews/python'
assets_path = root_path + '/assets'
out_path = root_path + '/out'
langs = ['ru', 'en']
categories = ["Entertainment", "Society", "Technology", "Sports", "Science", "Economy", "Other"]


class TGExecutor:
    def __init__(self, max_workers, lang):
        self.lang = lang
        self.q = queue.SimpleQueue()
        self.threads = [threading.Thread(target=self._work) for _ in range(max_workers)]
        for t in self.threads:
            t.start()

    def submit_data(self, data):
        self.q.put(data)

    def get_data(self, limit):
        data = [self.q.get()]
        for i in range(limit):
            try:
                data.append(self.q.get(False))
            except queue.Empty:
                break
        return data

    def _work(self):
        net_sys = NetSystem(self.lang)
        while True:
            datalist = self.get_data(10)
            net_sys.process(datalist)


class NetSystem:
    def __init__(self, lang):
        self.lang = lang
        self.nets = TNetPack(lang)

    def process(self, datalist):
        names = [data[0] for data in datalist]
        texts = [self.nets.stemmer.stem(data[1]) for data in datalist]
        news_test = self.nets.news_net.predict(texts)

        print(self.lang + ':: News test result: ' + str(news_test))

        names = [names[i] for i in range(len(texts)) if news_test[i]]
        texts = [texts[i] for i in range(len(texts)) if news_test[i]]

        try:
            categories_test = self.nets.class_net.predict(texts)
        except Exception:
            print(self.lang + ':: Error: Categorization failed')
            return

        print(self.lang + ':: Categories test result: ' + str(categories_test))

        for i in range(len(texts)):
            cat = categories[int(categories_test[i])]
            dump_info({"file_name": names[i], "lang": self.lang, "category": cat})


class TNetPack:
    def __init__(self, lang):
        self.stemmer = TStemmer(lang)
        self.news_net = TNet('news', lang) if lang == 'ru' else ENNewsNet()
        self.class_net = TNet('classifier', lang)


class TStemmer:
    def __init__(self, lang):
        self.stemmer = get_stemmer(lang)

    def stem(self, text):
        return ' '.join(map(lambda w: self.stemmer.stem(w), text.split()))


class TNet:
    def __init__(self, net_type, lang):
        self.model = get_model(lang, net_type)
        self.vectorizer = get_vectorizer(lang, net_type)

    def predict(self, stemmed_texts):
        x_tfidf = self.vectorizer.transform(stemmed_texts)
        return self.model.predict(x_tfidf)


class ENNewsNet(TNet):
    def __init__(self):
        super(ENNewsNet, self).__init__('news', 'en')

    def predict(self, stemmed_texts):
        x_tfidf = self.vectorizer.transform(stemmed_texts)
        ans = (0.45 < self.model.predict_proba(x_tfidf)[:, 1:])
        return flatten(ans)


def flatten(lst):
    return [item for sublist in lst for item in sublist]


def form_path(lang, category, name):
    return out_path + '/' + lang + '/' + category + '/' + name


def dump_info(js):
    path = form_path(js['lang'], js['category'], js['file_name'])
    print("Dumping: " + path)
    with open(path, 'w') as file:
        file.write(json.dumps(js))


def get_stemmer(lang):
    if lang == 'ru':
        return nltk.stem.snowball.RussianStemmer()
    elif lang == 'en':
        return nltk.stem.snowball.EnglishStemmer()


def get_vectorizer(lang, net):
    return load(assets_path + '/' + net + '/' + lang + '/vectorizer.pickle')


def get_model(lang, net):
    return load(assets_path + '/' + net + '/' + lang + '/model.pickle')


def load(path):
    try:
        print('L:' + path)
        with open(path, 'rb') as file:
            return pickle.load(file)
    except Exception:
        print("ERROR: Failed to load: " + path)


def tokenize(s):
    return s.split()
