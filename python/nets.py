from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.cluster import DBSCAN
from utility import *

en_threads_eps = {
    "entertainment": 1.1,
    "society": 1.08,
    "technology": 1.05,
    "sports": 1.06,
    "science": 1.08,
    "economy": 1.12,
    "other": 1.08
}

ru_threads_eps = {
    "entertainment": 1.09,
    "society": 1.10,
    "technology": 1.12,
    "sports": 1.13,
    "science": 1.11,
    "economy": 1.10,
    "other": 1.1
}


class NetSystem:
    def __init__(self, base, lang):
        self.lang = lang
        self.base = base
        self.nets = TNetPack(base, lang)

    def process(self, datalist):
        if len(datalist) == 0:
            return

        ptime = []
        headers = []
        names = []
        stemmed_texts = []
        urls = []
        for data in datalist:
            names.append(data[0])
            stemmed_texts.append(self.nets.stemmer.stem(data[1]))
            headers.append(data[2])
            ptime.append(data[3])
            urls.append(data[4])

        news_test = self.nets.news_net.predict(stemmed_texts)

        ptime = [ptime[i] for i in range(len(ptime)) if news_test[i]]
        headers = [headers[i] for i in range(len(ptime)) if news_test[i]]
        names = [names[i] for i in range(len(ptime)) if news_test[i]]
        stemmed_texts = [stemmed_texts[i] for i in range(len(ptime)) if news_test[i]]
        urls = [urls[i] for i in range(len(ptime)) if news_test[i]]

        try:
            if len(stemmed_texts) > 0:
                categories_test = self.nets.class_net.predict(stemmed_texts)
            else:
                return
        except Exception:
            print(self.lang + ':: Error: Categorization failed: ' + str(len(stemmed_texts)))
            return

        for i in range(len(stemmed_texts)):
            cat = categories[int(categories_test[i])]
            dump_info(self.base, {"file_name": names[i], "lang": self.lang, "category": cat,
                                  "header": headers[i], "og:url": urls[i], "published_time": ptime[i],
                                  "stemmed_text": stemmed_texts[i]})


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

        lang_cat_eps = en_threads_eps[self.category]
        if self.lang == 'ru':
            lang_cat_eps = ru_threads_eps[self.category]

        dbscan = DBSCAN(eps=lang_cat_eps, metric='euclidean', metric_params=None, algorithm='auto',
                        leaf_size=30, min_samples=5)
        dbscan.fit(x_train_tfidf)
        return dbscan.labels_
