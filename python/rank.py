from math import exp, log

### rank utils

def load_pagerank(path):
    pagerank = {}
    with open(path) as pf:
        sc = pf.readlines()
        for site in sc:
            score, url = site.split('\t')
            url = url[:-1]
            pagerank[url] = float(score)
    return pagerank


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


def sort_by_score(scores, vals):
    return [x for y, x in sorted(zip(scores, vals), key=lambda x: x[0], reverse=True)]


def sigmoid(x):
    return 1 / (1 + exp(-x))


def find_max_time(groups):
    ans = 0

    for group in groups:
        for article in group["articles"]:
            for filename in article:
                ans = max(ans, int(article[filename]["published_time"]))

    return ans


def format_output(groups):
    result = []
    for group in groups:
        new_group = {"title": group["title"],
                     #"category": group["category"],
                     "articles": []}

        for article in group["articles"]:
            for filename in article:
                new_group["articles"].append(filename.split('/')[-1])

        result.append(new_group)
    return result

def get_title(articles):
    for filename in articles[0]:
        return articles[0][filename]["header"]

### calc score of unit
def calc_article_score(meta, agencies, now_time):
    url_w = url_rank_weight(meta["og:url"], agencies)
    return url_w * int(meta["published_time"]) / now_time


def calc_group_score(group, agencies, now_time):
    cnt = len(group["articles"])
    agencies_weight = 0

    total_time = 0

    for article in group["articles"]:
        for filename in article:
            meta = article[filename]
            agencies_weight = agencies_weight + url_rank_weight(meta["og:url"], agencies)
            total_time = total_time + int(meta["published_time"])

    mean_time = total_time / cnt
    time_coef = sigmoid((mean_time - now_time) / 3600)

    penalty_coef = 1
    if group["lang"] == 'ru':
        penalty_coef = 0.7
    if cnt > 20:
        penalty_coef = 1 / cnt

    return agencies_weight * time_coef * penalty_coef


### rank items in units

# sort articles in thread
def rank_articles(group, agencies, now_time):
    new_group = {}
    scores = []
    for article in group["articles"]:
        for filename in article:
            scores.append(calc_article_score(article[filename], agencies, now_time))
    new_group["articles"] = sort_by_score(scores, group["articles"])
    return new_group


# sort threads
def rank_threads(groups, agencies):
    now_time = 1
    for lg in groups:
        for lang in lg:
            now_time = max(now_time, find_max_time(lg[lang]))

    new_groups = []

    for lg in groups:
        for lang in lg:
            for group in lg[lang]:
                new_group = rank_articles(group, agencies, now_time)
                #new_group["category"] = group["category"]
                new_group["lang"] = lang
                new_group["title"] = get_title(new_group["articles"])
                new_groups.append(new_group)

    groups = new_groups
    scores = []

    for group in groups:
        scores.append(calc_group_score(group, agencies, now_time))
    groups = sort_by_score(scores, groups)

    return format_output(groups)


if __name__ == '__main__':
    sample = \
        [
            {
                "ru": [
                    {
                        "articles": [
                            {
                                "../Samples/20200427/01/1688417002070796469.html": {
                                    "header": "В Украине начнут тестировать на антитела к COVID-19",
                                    "og:url": "https://korrespondent.net/ukraine/4222359-v-ukrayne-nachnut-testyrovat-na-antytela-k-COVID-19",
                                    "published_time": "1587952260"
                                }
                            },
                            {
                                "../Samples/20200427/01/1688417002363616594.html": {
                                    "header": "В МОЗ готовятся к пику коронавируса через неделю",
                                    "og:url": "https://korrespondent.net/ukraine/4222357-v-moz-hotoviatsia-k-pyku-koronavyrusa-cherez-nedelui",
                                    "published_time": "1587950220"
                                }
                            }
                        ],
                        "category": "society"
                    },
                    {
                        "articles": [
                            {
                                "../Samples/20200427/01/5477671248385717911.html": {
                                    "header": "Российские генетики выведут чувствительных к коронавирусу мышей",
                                    "og:url": "https://www.vedomosti.ru/society/news/2020/04/27/829026-chuvstvitelnih-k-koronavirusu-mishei?utm_campaign=vedomosti_public&amp;utm_content=829026-chuvstvitelnih-k-koronavirusu-mishei&amp;utm_medium=social&amp;utm_source=telegram_ved",
                                    "published_time": "1587950502"
                                }
                            },
                            {
                                "../Samples/20200427/01/6639103543939820029.html": {
                                    "header": "Россия создаст собственных мышей для тестирования вакцины против COVID",
                                    "og:url": "https://www.interfax.ru/russia/706160",
                                    "published_time": "1587949740"
                                }
                            }
                        ],
                        "category": "society"
                    },
                    {
                        "articles": [
                            {
                                "../Samples/20200427/01/1688417000605172155.html": {
                                    "header": "В КНДР сообщили, что Ким Чен Ын работает с документами",
                                    "og:url": "https://korrespondent.net/world/4221905-v-kndr-soobschyly-chto-kym-chen-yn-rabotaet-s-dokumentamy",
                                    "published_time": "1587950220"
                                }
                            },
                            {
                                "../Samples/20200427/01/6639103544018600539.html": {
                                    "header": "Советник президента Южной Кореи заявил о хорошем самочувствии Ким Чен Ына",
                                    "og:url": "https://www.interfax.ru/world/706161",
                                    "published_time": "1587952140"
                                }
                            }
                        ],
                        "category": "society"
                    }
                ]
            },
            {
                "en": [
                    {
                        "articles": [
                            {
                                "../Samples/20200427/01/2885715104644774382.html": {
                                    "header": "Kim Jong Un mystery grows on reports of train, medical team",
                                    "og:url": "https://indianexpress.com/article/world/kim-jong-un-mystery-grows-on-reports-of-train-medical-team-6380888/",
                                    "published_time": "1587951363"
                                }
                            },
                            {
                                "../Samples/20200427/01/3439335387883642015.html": {
                                    "header": "Kim Jong Un’s absence and North Korea’s silence keep rumor mill churning | Honolulu Star-Advertiser",
                                    "og:url": "https://www.staradvertiser.com/2020/04/26/breaking-news/kim-jong-uns-absence-and-north-koreas-silence-keep-rumor-mill-churning/",
                                    "published_time": "1587949500"
                                }
                            }
                        ],
                        "category": "society"
                    },
                    {
                        "articles": [
                            {
                                "../Samples/20200427/01/6060062399378778418.html": {
                                    "header": "Trump calls for journalists who covered the Russia investigation to return their 'Noble Prizes' in Twitter rant before deleting it",
                                    "og:url": "https://www.businessinsider.com/trump-deletes-tweet-calling-for-journalists-to-return-noble-prizes-2020-4",
                                    "published_time": "1587949655"
                                }
                            },
                            {
                                "../Samples/20200427/01/5090408755610020146.html": {
                                    "header": "Trump calls for journalists who covered the Russia investigation to return their 'Noble Prizes' in Twitter rant before deleting it, Business Insider - Business Insider Singapore",
                                    "og:url": "https://www.businessinsider.sg/trump-deletes-tweet-calling-for-journalists-to-return-noble-prizes-2020-4",
                                    "published_time": "1587949655"
                                }
                            }
                        ],
                        "category": "society"
                    },
                    {
                        "articles": [
                            {
                                "../Samples/20200427/01/3439335388892497494.html": {
                                    "header": "Spain lets children play as U.S. states move at various speeds | Honolulu Star-Advertiser",
                                    "og:url": "https://www.staradvertiser.com/2020/04/26/breaking-news/spain-lets-children-play-as-u-s-states-move-at-various-speeds/",
                                    "published_time": "1587950700"
                                }
                            },
                            {
                                "../Samples/20200427/01/3480199993138462393.html": {
                                    "header": "Care home left waiting 10 days for coronavirus tests",
                                    "og:url": "https://www.shropshirestar.com/news/uk-news/2020/04/27/care-home-left-waiting-10-days-for-coronavirus-tests/",
                                    "published_time": "1587952036"
                                }
                            }
                        ],
                        "category": "society"
                    }
                ]
            }
        ]

    m = load_pagerank("assets/pagerank.txt")

    sample2 = [
        {
            "ru": []
        }
    ]

    #print(rank_threads(sample, m))
    print(rank_threads(sample, m))

