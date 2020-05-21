import nltk
import pickle
import pandas as pd
import sys
from sklearn.metrics import accuracy_score


class NewsNet:
    def __init__(self, lang):
        self.lang = lang
        self.stemmer = get_stemmer(lang)
        self.vectorizer = load('assets/news/' + lang + '/vectorizer.pickle')
        self.model = load('assets/news/' + lang + '/model.pickle')

    def check(self, text):
        stemmed_text = stem_text(text, self.stemmer)
        x_tfidf = self.vectorizer.transform([stemmed_text])
        return predict(self.model, x_tfidf, self.lang)


def get_stemmer(lang):
    if lang == 'ru':
        return nltk.stem.snowball.RussianStemmer()
    elif lang == 'en':
        return nltk.stem.snowball.EnglishStemmer()


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
    vectorizer = load('assets/news/' + lang + '/vectorizer.pickle')
    model = load('assets/news/' + lang + '/model.pickle')

    df = pd.read_csv('assets/news/' + lang + '/test.csv', sep='\t')
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
    test(sys.argv[1])
