import threading
import queue
from abc import abstractmethod
from nets import *


class TGExecutor:
    def __init__(self, base, max_workers):
        self.base = base
        self.q = queue.SimpleQueue()
        self.threads = [threading.Thread(target=self._work) for _ in range(max_workers)]

    def start(self):
        for t in self.threads:
            t.start()

    def join(self):
        for t in self.threads:
            t.join()

    def submit_data(self, data):
        self.q.put(data)

    def get_data(self, limit):
        data = [self.q.get()]
        for i in range(limit):
            try:
                data.append(self.q.get())
            except queue.Empty:
                break
        return data

    @abstractmethod
    def _work(self):
        pass


class TGServerExecutor(TGExecutor):
    def __init__(self, base, max_workers, lang):
        self.lang = lang
        self.base = base
        super(TGServerExecutor, self).__init__(base, max_workers)
        self.start()

    def _work(self):
        net_sys = NetSystem(self.base, self.lang)
        while True:
            datalist = self.get_data(10)
            net_sys.process(datalist)


class TOneWayExecutor:
    def __init__(self, max_workers):
        self.q = queue.SimpleQueue()
        self.threads = [threading.Thread(target=self._work) for _ in range(max_workers)]

    def submit(self, file_name):
        self.q.put(file_name)

    def start(self):
        for t in self.threads:
            t.start()

    def join(self):
        for t in self.threads:
            t.join()

    def get(self, limit):
        if self.q.empty():
            return []
        data = []
        for i in range(limit):
            try:
                data.append(self.q.get(False))
            except queue.Empty:
                break
        return data

    def get_texts(self, limit):
        files = self.get(limit)
        texts = []
        for art in files:
            texts.append(load_file(art))
        return files, texts

    @abstractmethod
    def _work(self):
        pass


class NewsExecutor(TOneWayExecutor):
    def __init__(self, base, max_workers, lang):
        self.base = base
        self.lang = lang
        self.result = queue.SimpleQueue()
        self.stemmer = get_stemmer(self.lang)
        super(NewsExecutor, self).__init__(max_workers)

    def _work(self):
        net = TNet(self.base, 'news', self.lang)
        res = []
        while True:
            files, texts = self.get_texts(10)
            if len(files) == 0:
                break
            assert(type(texts[0]) == HParser)
            stemmed_texts = [self.stemmer.stem(t.result) for t in texts]
            rs = net.predict(stemmed_texts)
            res += [[files[i], rs[i]] for i in range(len(texts))]
        for r in res:
            self.result.put(r)


def news_cat_scan(self, net_news, net_cat):
    files, texts = self.get_texts(10)
    if len(files) == 0:
        return [], [], [], [], True
    assert(type(texts[0]) == HParser)
    stemmed_texts = [self.stemmer.stem(t.result) for t in texts]
    news_test = net_news.predict(stemmed_texts)
    stemmed_texts = [stemmed_texts[i] for i in range(len(texts)) if news_test[i]]
    texts = [texts[i] for i in range(len(texts)) if news_test[i]]
    files = [files[i] for i in range(len(files)) if news_test[i]]
    cat_test = net_cat.predict(stemmed_texts)
    return files, stemmed_texts, texts, cat_test, False


class CatExecutor(TOneWayExecutor):
    def __init__(self, base, max_workers, lang):
        self.base = base
        self.lang = lang
        self.result = queue.SimpleQueue()
        self.stemmer = get_stemmer(self.lang)
        super(CatExecutor, self).__init__(max_workers)

    def _work(self):
        net_news = TNet(self.base, 'news', self.lang)
        net_cat = TNet(self.base, 'categories', self.lang)
        res = []
        while True:
            files, stemmed_texts, texts, cat_test, end = news_cat_scan(self, net_news, net_cat)
            if end:
                break
            res += [[files[i], cat_test[i]] for i in range(len(texts))]
        for r in res:
            self.result.put(r)


class ThreadingExecutor(TOneWayExecutor):
    def __init__(self, base, max_workers, lang):
        self.base = base
        self.lang = lang
        self.result = []
        for _ in categories:
            self.result.append(queue.SimpleQueue())
        self.stemmer = get_stemmer(self.lang)
        super(ThreadingExecutor, self).__init__(max_workers)

    def _work(self):
        net_news = TNet(self.base, 'news', self.lang)
        net_cat = TNet(self.base, 'categories', self.lang)
        res = []
        for _ in categories:
            res.append([])
        while True:
            files, stemmed_texts, texts, cat_test, end = news_cat_scan(self, net_news, net_cat)
            if end:
                break
            for i in range(len(texts)):
                res[cat_test[i]].append([files[i], stemmed_texts[i], texts[i]])
        for i in range(len(res)):
            for x in res[i]:
                self.result[i].put(x)
