import sys
from pathlib import Path
from executors import *
import rank


def process(base, path, netclass):
    js = load_js(base + '/../../' + path)
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


def process_threads_impl(base, path):
    executors, js = process(base, path, ThreadingExecutor)
    grouping = []
    en = 0 if js[0]['lang_code'] == 'en' else 1
    ru = 0 if js[0]['lang_code'] == 'ru' else 1
    ind = {'ru': ru, 'en': en}
    for lang in executors:
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
            base_ind = len(grouping)
            n_groups = max(cur_result) + 1
            for i in range(n_groups):
                grouping.append({"articles": []})
            for i in range(len(files)):
                if cur_result[i] != -1:
                    grouping[base_ind + cur_result[i]]["articles"].append({files[i][0]: js[li]['articles'][files[i][0]]})
                    grouping[base_ind + cur_result[i]]["title"] = files[i][2].header
    return grouping


def reparse_threads(result_js, m):
    result_js = rank.rank_threads(result_js, m)
    js = []
    for entry in result_js:
        js.append({'title': entry['title']})
        js[-1]['articles'] = []
        for art in entry['articles']:
            for file_name in art:
                js[-1]['articles'].append(file_name.split('/')[-1])
    return js


def process_get_impl(base, min_time, lang, cat, m):
    dir_path = out_path(base) + '/' + lang + '/' + cat
    files = collect_files_in_directory(dir_path)
    if len(files) == 0:
        return []
    fjs = get_files_jsons(files, min_time)
    if len(fjs) < 8:
        return []
    stemmed_texts = []
    files = []
    for path in fjs:
        stemmed_texts.append(fjs[path]['stemmed_text'])
        files.append(path)
    net = ThreadsNet(min(8, int(0.1 * len(files))), lang, cat)
    grouping = []
    cur_result = net.predict(stemmed_texts)
    n_groups = max(cur_result) + 1
    for i in range(n_groups):
        grouping.append({"articles": []})
    for i in range(len(files)):
        if cur_result[i] != -1:
            grouping[cur_result[i]]["articles"].append(
                {
                    files[i]: {
                        'og:url': fjs[files[i]]['og:url'],
                        'published_time': fjs[files[i]]['published_time'],
                    }
                })
            grouping[cur_result[i]]["title"] = fjs[files[i]]['header']
    return reparse_threads(grouping, m)


def thread_process_get(result, base, min_time, lang, cat, m):
    result.put(process_get_impl(base, min_time, lang, cat, m))


def process_get(base, min_time, lang, cat):
    m = rank.load_pagerank(assets_path(base) + '/pagerank.txt')
    if cat != 'any':
        return {'threads': process_get_impl(base, min_time, lang, cat, m)}

    result = queue.SimpleQueue()
    threads = [threading.Thread(target=thread_process_get,
                                args=[result, base, min_time, lang, cat, m]) for cat in categories]
    for t in threads:
        t.start()
    for t in threads:
        t.join()

    total = []
    while not result.empty():
        total += result.get()

    return {'threads': total}


if __name__ == '__main__':
    base = str(Path(sys.argv[0]).parent) + '/'
    net_type = sys.argv[1]
    path = sys.argv[2]
    if net_type == 'news':
        process_news(base, path)
    elif net_type == 'categories':
        process_cat(base, path)
    elif net_type == 'threads':
        m = rank.load_pagerank(assets_path(base) + '/pagerank.txt')
        print(json.dumps(reparse_threads(process_threads_impl(base, path), m), indent=2))

