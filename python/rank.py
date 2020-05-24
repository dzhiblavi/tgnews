def extract_url_domain(site):
    dom = site.split('/')[2]
    if dom.startswith("www."):
        dom = dom[4:]
    return dom


def url_rank_weight(site, agencies):
    dom = extract_url_domain(site)
    if dom in agencies:
        return agencies[dom]
    return 0.000015


def calc_rank_score(group, agencies):
    print(group)
    cnt = len(group["articles"])
    agencies_weight = 0
    # index = int(0.9 * (cnt - 1))
    index = cnt - 1
    total_time = 0
    for article in group["articles"]:
        agencies_weight = agencies_weight + url_rank_weight(article["og:url"], agencies)
        total_time = total_time + int(group["articles"][index]["time"])
    return agencies_weight * total_time * cnt


def rank(groups, agencies):
    scores = []
    for group in groups:
        scores.append(calc_rank_score(group, agencies))
    print(scores)
    return [x for y, x in sorted(zip(scores, groups), key=lambda x: x[0], reverse=True)]


def load_pagerank(path):
    pagerank = {}
    with open(path) as pf:
        sc = pf.readlines()
        for site in sc:
            score, url = site.split('\t')
            url = url[:-1]
            pagerank[url] = score
    return pagerank


if __name__ == '__main__':
    sample = \
        [
            {
                "title": "Apple reveals new AirPods Pro",
                "articles": [
                    {
                        "filename": "9436743547232134.html",
                        "time": "123123",
                        "og:url": "http://saharareporters.com/2020/04/27/update-president-buhari-extends-lockdown-lagos-ogun-fct-one-week-curfew-begin-after-then"
                    }
                ]
            },
            {
                "title": "Apple sucks",
                "articles": [
                    {
                        "filename": "1337.html",
                        "time": "2020",
                        "og:url": "https://www.uefa.com/uefaeuro-2020/news/025b-0f07234151fa-fe167d581509-1000--watch-classic-games-on-uefa-tv/"
                    }
                ]
            }
        ]

    m = load_pagerank("assets/pagerank.txt")
    print(rank(sample, m))

