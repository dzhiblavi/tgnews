import sys
from pathlib import Path
from executors import *


def process(base, path, netclass):
    js = load_js(path)
    executors = {'ru': netclass(base, 8, 'ru'), 'en': netclass(base, 8, 'en')}
    for entry in js:
        lang = entry['lang_code']
        for article in entry['articles']:
            executors[lang].submit(article)
    for ex in executors:
        executors[ex].start()
    for ex in executors:
        executors[ex].join()
    return executors


def process_news(base, path):
    executors = process(base, path, NewsExecutor)
    list_art = []
    for ex in executors:
        q = executors[ex].result
        while not q.empty():
            item = q.get()
            if item[1]:
                list_art.append(item[0].split('/')[-1])
    print(json.dumps({'articles': list_art}, indent=2))


def process_cat(base, path):
    executors = process(base, path, CatExecutor)
    result = []
    mp = {}
    for cat in categories:
        mp[cat] = []
    for ex in executors:
        q = executors[ex].result
        while not q.empty():
            item = q.get()
            mp[categories[item[1]]].append(item[0])
    for cat in categories:
        if len(mp[cat]) > 0:
            result.append({"category": cat, "articles": mp[cat]})
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    base = str(Path(sys.argv[0]).parent) + '/'
    net_type = sys.argv[1]
    path = sys.argv[2]
    if net_type == 'news':
        process_news(base, path)
    else:
        process_cat(base, path)
