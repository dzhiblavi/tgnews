from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.cluster import DBSCAN
from utility import *


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
        self.news_net = TNet(base, 'news', lang)
        self.class_net = TNet(base, 'categories', lang)


class TStemmer:
    def __init__(self, lang):
        self.stemmer = get_stemmer(lang)

    def stem(self, text):
        return ' '.join(map(lambda w: self.stemmer.stem(w), text.split()))


class TNet:
    def __init__(self, base, net_type, lang):
        self.lang = lang
        self.net_type = net_type
        self.model = get_model(base, lang, net_type)
        self.vectorizer = get_vectorizer(base, lang, net_type)

    def predict(self, stemmed_texts):
        if len(stemmed_texts) == 0:
            return []
        x_tfidf = self.vectorizer.transform(stemmed_texts)
        if self.net_type == 'news' and self.lang == 'en':
            return flatten(0.45 < self.model.predict_proba(x_tfidf)[:, 1:])
        else:
            return self.model.predict(x_tfidf)


class ThreadsNet:
    def __init__(self, min_df, lang, category):
        self.lang = lang
        self.category = category
        self.vectorizer = TfidfVectorizer(ngram_range=(1, 1), tokenizer=tokenize,
                                          min_df=min_df, max_df=0.7, use_idf=1, lowercase=True,
                                          smooth_idf=1, sublinear_tf=1, max_features=1000)

    def predict(self, stemmed_texts):
        if len(stemmed_texts) == 0:
            return []
        self.vectorizer.fit(stemmed_texts)
        x_train_tfidf = self.vectorizer.transform(stemmed_texts)
        dbscan = DBSCAN(eps=1.1, metric='euclidean', metric_params=None, algorithm='auto',
                        leaf_size=30, min_samples=5)
        dbscan.fit(x_train_tfidf)
        return dbscan.labels_
