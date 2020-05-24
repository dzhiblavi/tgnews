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
            with open(art, 'r') as art_file:
                texts.append(art_file.read())
        return files, texts

    @abstractmethod
    def _work(self):
        pass


class NewsExecutor(TOneWayExecutor):
    def __init__(self, base, max_workers, lang):
        self.base = base
        self.lang = lang
        self.result = queue.SimpleQueue()
        super(NewsExecutor, self).__init__(max_workers)

    def _work(self):
        net = TNet(self.base, 'news', self.lang)
        res = []
        while True:
            files, texts = self.get_texts(10)
            if len(files) == 0:
                break
            rs = net.predict(texts)
            res += [[files[i], rs[i]] for i in range(len(texts))]
        for r in res:
            self.result.put(r)


class CatExecutor(TOneWayExecutor):
    def __init__(self, base, max_workers, lang):
        self.base = base
        self.lang = lang
        self.result = queue.SimpleQueue()
        super(CatExecutor, self).__init__(max_workers)

    def _work(self):
        net_news = TNet(self.base, 'news', self.lang)
        net_cat = TNet(self.base, 'categories', self.lang)
        res = []
        while True:
            files, texts = self.get_texts(10)
            if len(files) == 0:
                break
            news_test = net_news.predict(texts)
            texts = [texts[i] for i in range(len(texts)) if news_test[i]]
            files = [files[i] for i in range(len(files)) if news_test[i]]
            cat_test = net_cat.predict(texts)
            res += [[files[i], cat_test[i]] for i in range(len(texts))]
        for r in res:
            self.result.put(r)