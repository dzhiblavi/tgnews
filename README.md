# Project description

1) Clustering algorithms:

- Isolate articles in English and Russian

- Isolate news articles

- Group articles into categories

- Group articles into threads

2) Analyze, store and index articles.

3) Return news threads for a particular period, sorted by relative importance.

Full description (ENG): https://contest.com/docs/data_clustering2/

# How to start

## Stage 1. Clustering

1.1. Grouping by language

```bash
tgnews languages <source_dir>
```

1.2. Isolating news articles

```bash
tgnews news <source_dir>
```

1.3. Grouping by category

TODO

1.4. Grouping similar news into threads

TODO

## Stage 2. Indexing and Ranking

2.0. Start server

```bash
tgnews server <port>
```

2.1. Indexing

HTTP request:
```
PUT /article.html HTTP/1.1
Content-Type: text/html
Cache-Control: max-age=<seconds>
Content-Length: 9

<content>
```

2.2. Removing from the index

HTTP request:

```
DELETE /article.html HTTP/1.1
```

2.3. Thread ranking

TODO