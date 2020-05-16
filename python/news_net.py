import nltk
import pickle
import pandas as pd
from sklearn.metrics import accuracy_score


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


def test(lang):
    stemmer = get_stemmer(lang)
    vectorizer = load('assets/' + lang + '/vectorizer.pickle')
    model = load('assets/' + lang + '/model.pickle')

    df = pd.read_csv('assets/' + lang + '/test.csv', sep='\t')
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
    y = model.predict(x_train_tfidf)
    print(accuracy_score(number_labels, y))


if __name__ == '__main__':
    test('ru')