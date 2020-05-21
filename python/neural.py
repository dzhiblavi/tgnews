import nltk
import pickle
import pandas as pd
import sys
import json
import queue
import threading
from sklearn.metrics import accuracy_score


class TGExecutor:
    def __init__(self, max_workers):
        self.q = queue.Queue()
        self.threads = [threading.Thread(target=self._work) for _ in range(max_workers)]
        for t in self.threads:
            t.start()

    def submit_data(self, data):
        self.q.put(data)

    def _work(self):
        net_sys = NetSystem()
        while True:
            data = self.q.get()
            print("Processing: " + data[0])
            net_sys.process(data)


class NetSystem:
    def __init__(self):
        self.stemmers = {'ru': get_stemmer('ru'), 'en': get_stemmer('en')}
        self.models = {'ru': get_model('ru'), 'en': get_model('en')}
        self.vectorizers = {'ru': get_vectorizer('ru'), 'en': get_vectorizer('en')}

    def process(self, text):
        pass
        # stemmed_text = stem_text(text, self.stemmer)
        # x_tfidf = self.vectorizer.transform([stemmed_text])
        # return predict(self.model, x_tfidf, self.lang)


def get_stemmer(lang):
    if lang == 'ru':
        return nltk.stem.snowball.RussianStemmer()
    elif lang == 'en':
        return nltk.stem.snowball.EnglishStemmer()


def get_vectorizer(lang):
    return load('/Users/dzhiblavi/Documents/prog/tgnews/python/assets/news/' + lang + '/vectorizer.pickle')


def get_model(lang):
    return load('/Users/dzhiblavi/Documents/prog/tgnews/python/assets/news/' + lang + '/model.pickle')


def stem_text(text, stemmer):
    tokens = text.split()
    return ' '.join(map(lambda w: stemmer.stem(w), tokens))


def load(path):
    with open(path, 'rb') as file:
        return pickle.load(file)


def tokenize(s):
    return s.split()


def predict(model, x_tfidf, lang):
    if lang == 'ru':
        return model.predict(x_tfidf)
    else:
        return 0.45 < model.predict_proba(x_tfidf)[:, 1:]


def test(lang):
    stemmer = get_stemmer(lang)
    vectorizer = load('../assets/news/' + lang + '/vectorizer.pickle')
    model = load('../assets/news/' + lang + '/model.pickle')

    df = pd.read_csv('../assets/news/' + lang + '/test.csv', sep='\t')
    texts = df.text
    labels = df.label

    stemmed_texts = []
    number_labels = []
    for i in texts:
        stemmed_texts.append(stem_text(i, stemmer))
    for i in labels:
        j = int(i == "News")
        number_labels.append(j)

    x_train_tfidf = vectorizer.transform(stemmed_texts)

    y1 = predict(model, x_train_tfidf, lang)
    print(accuracy_score(number_labels, y1))


if __name__ == '__main__':
    executor = TGExecutor(8)
    while True:
        pass
    pass
