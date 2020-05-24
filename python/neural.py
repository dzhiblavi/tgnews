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
    return executors, js


def process_news(base, path):
    executors, js = process(base, path, NewsExecutor)
    list_art = []
    for ex in executors:
        q = executors[ex].result
        while not q.empty():
            item = q.get()
            if item[1]:
                list_art.append(item[0].split('/')[-1])
    print(json.dumps({'articles': list_art}, indent=2))


def process_cat(base, path):
    executors, js = process(base, path, CatExecutor)
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
            result.append({"category": cat, "articles": list(map(lambda x: x.split('/')[-1], mp[cat]))})
    print(json.dumps(result, indent=2))


def process_threads(base, path):
    executors, js = process(base, path, ThreadingExecutor)
    grouping = []
    en = 0 if js[0]['lang_code'] == 'en' else 1
    ru = 0 if js[0]['lang_code'] == 'ru' else 1
    ind = {'ru': ru, 'en': en}
    for lang in executors:
        print('LANG: ' + lang)
        li = ind[lang]
        exr = executors[lang]
        result = exr.result
        for i in range(len(categories)):
            files = []
            while not result[i].empty():
                files.append(result[i].get())
            if len(files) < 8:
                continue
            net = ThreadsNet(min(8, int(0.1 * len(files))), lang, i)
            cur_result = net.predict([t[1] for t in files])
            print(str(cur_result))
            base_ind = len(grouping)
            n_groups = max(cur_result) + 1
            for i in range(n_groups):
                grouping.append({"articles": []})
            for i in range(len(files)):
                if cur_result[i] != -1:
                    grouping[base_ind + cur_result[i]]["articles"].append({files[i][0]: js[li]['articles'][files[i][0]]})
                    grouping[base_ind + cur_result[i]]["title"] = files[i][2].header
                    print(files[i][2].header)
    print(json.dumps(grouping, indent=2))


if __name__ == '__main__':
    base = str(Path(sys.argv[0]).parent) + '/'
    net_type = sys.argv[1]
    path = sys.argv[2]
    if net_type == 'news':
        process_news(base, path)
    elif net_type == 'categories':
        process_cat(base, path)
    elif net_type == 'threads':
        process_threads(base, path)

