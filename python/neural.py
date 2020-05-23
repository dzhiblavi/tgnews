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


class TGExecutor:
    def __init__(self, lang, max_workers):
        self.lang = lang
        self.q = queue.SimpleQueue()
        self.threads = [threading.Thread(target=self._work) for _ in range(max_workers)]
        for t in self.threads:
            t.start()

    def submit_data(self, data):
        self.q.put(data)

    def _work(self):
        net_sys = NetSystem(self.lang)
        while True:
            data = self.q.get()
            net_sys.process(data)


class NetSystem:
    def __init__(self, lang):
        self.lang = lang
        self.nets = TNetPack(lang)

    def process(self, datalist):
        names = [data[0] for data in datalist]
        texts = [self.nets.stemmer.stem(data[1]) for data in datalist]
        news_test = self.nets.news_net.predict(texts)

        for i in range(len(texts)):
            print('i = ' + str(i) + ', news_test=' + news_test[i])
            if news_test[i]:
                print('News detected, dumping')
                cat = 'Sports'
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
        return 0.45 < self.model.predict_proba(x_tfidf)[:, 1:]


def form_path(lang, category, name):
    return out_path + '/' + lang + '/' + category + '/' + name


def dump_info(js):
    path = form_path(js['lang'], js['category'], js['file_name'])
    print("Dumping: " + path + ": " + json.dumps(js))
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
    with open(path, 'rb') as file:
        return pickle.load(file)


def tokenize(s):
    return s.split()
