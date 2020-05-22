import nltk
import pickle
import pandas as pd
import sys
import queue
import threading
from sklearn.metrics import accuracy_score


root_path = '/Users/dzhiblavi/Documents/prog/tgnews/python'
assets_path = root_path + '/assets'
out_path = root_path + '/out'


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
            net_sys.process(data)


class NetSystem:
    def __init__(self):
        self.stemmers = {'ru': get_stemmer('ru'), 'en': get_stemmer('en')}
        self.models = {'ru': get_model('ru'), 'en': get_model('en')}
        self.vectorizers = {'ru': get_vectorizer('ru'), 'en': get_vectorizer('en')}

    def predict_news(self, text, lang):
        stemmed_text = stem_text(text, self.stemmers[lang])
        x_tfidf = self.vectorizers[lang].transform([stemmed_text])
        return predict(self.models[lang], x_tfidf, lang)

    def process(self, data):
        name = data[0]
        lang = data[1]
        text = data[2]
        if self.predict_news(str(text), lang):
            print("News detected: " + name)
        else:
            print("Non-news detected: " + name)


def get_stemmer(lang):
    if lang == 'ru':
        return nltk.stem.snowball.RussianStemmer()
    elif lang == 'en':
        return nltk.stem.snowball.EnglishStemmer()


def get_vectorizer(lang):
    return load(assets_path + '/news/' + lang + '/vectorizer.pickle')


def get_model(lang):
    return load(assets_path + '/news/' + lang + '/model.pickle')


def stem_text(text, stemmer):
    return ' '.join(map(lambda w: stemmer.stem(w), text.split()))


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
    vectorizer = load('assets/news/' + lang + '/vectorizer.pickle')
    model = load('assets/news/' + lang + '/model.pickle')

    df = pd.read_csv('assets/news/' + lang + '/test.csv', sep='\t')
    texts = df.text
    labels = df.label

    stemmed_texts = []
    number_labels = []
    for i in texts:
        print(type(i))
        stemmed_texts.append(stem_text(i, stemmer))
    for i in labels:
        j = int(i == "News")
        number_labels.append(j)

    x_train_tfidf = vectorizer.transform(stemmed_texts)

    y1 = predict(model, x_train_tfidf, lang)
    print(accuracy_score(number_labels, y1))


if __name__ == '__main__':
    test(sys.argv[1])
