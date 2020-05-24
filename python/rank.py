from utility import *

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



def calc_article_score(meta, agencies, nowtime):
    url_w = url_rank_weight(meta["og:url"], agencies)
    return url_w * int(meta["published_time"]) / nowtime

# sort articles in each thread
def rank_articles(groups, agencies):
    nowtime = current_time()

    result = []

    for group in groups:
        newgroup = {"title": group["title"]}
        scores = []
        for article in group["articles"]:
            for filename in article:
                scores.append(calc_article_score(article[filename], agencies, nowtime))
        print(scores)
        newgroup["articles"] = [x for y, x in sorted(zip(scores, group["articles"]), key=lambda x: x[0], reverse=True)]
        result.append(newgroup)
    return result


# TODO: nowtime usage
# TODO: time percentile
def calc_group_score(group, agencies, nowtime):
    print(group)
    cnt = len(group["articles"])
    agencies_weight = 0
    # index = int(0.9 * (cnt - 1))
    total_time = 0  # replace with percentile
    for article in group["articles"]:
        for filename in article:
            meta = article[filename]
            agencies_weight = agencies_weight + url_rank_weight(meta["og:url"], agencies)
            total_time = total_time + int(meta["published_time"])
    return agencies_weight * total_time * cnt

# sort threads
def rank_threads(groups, agencies):
    nowtime = current_time()

    scores = []
    for group in groups:
        scores.append(calc_group_score(group, agencies, nowtime))
    groups = [x for y, x in sorted(zip(scores, groups), key=lambda x: x[0], reverse=True)]
    return rank_articles(groups, agencies)


def load_pagerank(path):
    pagerank = {}
    with open(path) as pf:
        sc = pf.readlines()
        for site in sc:
            score, url = site.split('\t')
            url = url[:-1]
            pagerank[url] = float(score)
    return pagerank


if __name__ == '__main__':
    sample = \
        [
            {
                "title": "TITLE1",
                "articles": [
                    {
                        "../Samples/tes/4938117986046875841.html": {
                            "og:url": "https://car.ru/news/research/72509-pochemu-maz-proigral-gaz-66-v-gonke-za-pervoe-mesto-sredi-voennoy-tehniki-sssr/",
                            "published_time": "1587945600"
                        }
                    },
                    {
                        "../Samples/tes/4321729651107501953.html": {
                            "og:url": "https://hi-tech.ua/article/asus-rog-strix-impact-ii-test/",
                            "published_time": "1587945677"
                        }
                    }
                ]
            },
            {
                "title": "TITLE2",
                "articles": [
                    {
                        "../Samples/tes/4321729651117334193.html": {
                            "og:url": "https://hi-tech.ua/miui-12-oficzialno-predstavlena-obyavleny-sroki-vyhoda-na-smartfonah-xiaomi-i-redmi/",
                            "published_time": "1587945600"
                        }
                    },
                    {
                        "../Samples/tes/4536220075826701708.html": {
                            "og:url": "https://andro-news.com/news/anons-xiaomi-mi-10-youth-edition-poluflagman-kompanii.html",
                            "published_time": "1587945621"
                        }
                    }
                ]
            },
            {
                "title": "TITLE3",
                "articles": [
                    {
                        "../Samples/tes/461959166760319816.html": {
                            "og:url": "https://www.onlinetambov.ru/news/society/v-tambovskoy-oblasti-vyyavlen-61-novyy-sluchay-zabolevaniya-koronavirusom/",
                            "published_time": "1587945600"
                        }
                    },
                    {
                        "../Samples/tes/461959166722026228.html": {
                            "og:url": "https://www.onlinetambov.ru/news/society/tambovchanam-zapretili-provodit-pikniki-na-prirode-v-mayskie-prazdniki/",
                            "published_time": "1587945633"
                        }
                    }
                ]
            }
        ]

    m = load_pagerank("assets/pagerank.txt")
    print(rank_threads(sample, m))
    #print(rank_articles(sample, m))
