from abc import ABC
import nltk
import queue
import threading
import pickle
import warnings
import json
from html.parser import HTMLParser
import os


warnings.filterwarnings("ignore", category=DeprecationWarning)
warnings.filterwarnings("ignore", category=UserWarning)
langs = ['ru', 'en']
categories = ["entertainment", "society", "technology", "sports", "science", "economy", "other"]


class HParser(HTMLParser):
    def __init__(self):
        super(HParser, self).__init__()
        self.result = ""
        self.header = ""
        self.is_header = False

    def handle_starttag(self, tag, attrs):
        if tag == 'h1':
            self.is_header = True

    def handle_endtag(self, tag):
        if tag == 'h1':
            self.is_header = False

    def handle_data(self, data):
        self.result += data
        if self.is_header:
            self.header += data

    def error(self, message):
        pass


def assets_path(base):
    return base + '../../assets/'


def out_path(base):
    return base + '../../out/'


def load_js(path):
    with open(path, 'r', encoding='utf-8') as file:
        return json.loads(file.read())


def flatten(lst):
    return [item for sublist in lst for item in sublist]


def form_path(base, lang, category, name):
    return out_path(base) + '/' + lang + '/' + category + '/' + name


def dump_info(base, js):
    path = form_path(base, js['lang'], js['category'], js['file_name'])
    with open(path, 'w') as file:
        file.write(json.dumps(js, indent=2, ensure_ascii=False))


def get_stemmer(lang):
    if lang == 'ru':
        return nltk.stem.snowball.RussianStemmer()
    elif lang == 'en':
        return nltk.stem.snowball.EnglishStemmer()


def get_vectorizer(base, lang, net):
    return load(assets_path(base) + '/' + net + '/' + lang + '/vectorizer.pickle')


def get_model(base, lang, net):
    return load(assets_path(base) + '/' + net + '/' + lang + '/model.pickle')


def load_file(path):
    with open(path, 'r') as file:
        parser = HParser()
        parser.feed(file.read())
        # print(parser.result)
        # print('initial header=' + parser.header)
        return parser
        # return file.read()


def load(path):
    try:
        with open(path, 'rb') as file:
            return pickle.load(file)
    except Exception:
        print("ERROR: Failed to load: " + path)


def tokenize(s):
    return s.split()


def collect_files_in_directory(path):
    files = []
    for r, d, f in os.walk(path):
        for file in f:
            files.append(os.path.join(r, file))
    return files


def get_file_json_chunk(chunk, result):
    local_res = []
    for file_path in chunk:
        local_res.append([file_path, load_js(file_path)])
    for r in local_res:
        result.put(r)


def get_files_jsons(files, min_time):
    n = int(len(files) / 8) + 1
    result = queue.SimpleQueue()
    chunks = [files[i:i + n] for i in range(0, len(files), n)]
    threads = [threading.Thread(target=get_file_json_chunk, args=[c, result]) for c in chunks]
    for t in threads:
        t.start()
    for t in threads:
        t.join()
    res = {}
    while not result.empty():
        [path, js] = result.get()
        if int(js['published_time']) >= min_time:
            res[path] = js
    return res
