#include "gtest/gtest.h"

#include "parser.h"

using namespace html;

TEST(html_parser, empty) {
    std::string e = "";
    html::parser::extract(e);
    ASSERT_EQ(e, "");

    e = "text";
    html::parser::extract(e);
    ASSERT_EQ(e, "text");

    e = "<a href=\"\"></a>";
    html::parser::extract(e);
    ASSERT_EQ(e, "");
}

TEST(html_parser, simple) {
    std::string h1 = "<h1>abc abd eeeee ebwrbwb  132423   []*  </h1>";
    html::parser::extract(h1);
    ASSERT_EQ(h1, "abc abd eeeee ebwrbwb 132423 []* ");
}

TEST(html_parser, empty_meta) {
    std::unordered_map<std::string, std::string> meta;
    std::string e = "";
    html::parser::extract(e, meta);
    ASSERT_EQ(e, "");
    ASSERT_TRUE(meta.empty());

    e = "text";
    html::parser::extract(e, meta);
    ASSERT_EQ(e, "text");
    ASSERT_TRUE(meta.empty());

    e = "<h1>abc abd eeeee ebwrbwb  132423   []*  </h1>";
    html::parser::extract(e, meta);
    ASSERT_EQ(e, "abc abd eeeee ebwrbwb 132423 []* ");
    ASSERT_TRUE(meta.empty());

    e = "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\"/>";
    html::parser::extract(e, meta);
    ASSERT_EQ(e, "");
    ASSERT_TRUE(meta.empty());
}

TEST(html_parser, simple_meta) {
    std::unordered_map<std::string, std::string> meta;
    const std::string article =
        "   <meta charset=\"utf-8\"/>"
        "   <meta property=\"og:url\" content=\"http://www.irishnews.com/news/northernirelandnews/2020/04/27/news/prescription-drugs-stolen-during-co-antrim-burglary-1916724/\"/>\n"
        "   <meta property=\"og:site_name\" content=\"The Irish News\"/>\n"
        "   <meta property=\"article:published_time\" content=\"2020-05-03T04:28:11+00:00\"/>\n"
        "   <meta property=\"og:title\" content=\"Prescription drugs stolen during Co Antrim burglary\"/>\n"
        "   <meta property=\"og:description\" content=\"PRESCRIPTION drugs have been stolen during a burglary at a chemist in Co Antrim.\"/>";
    std::string e = article;
    html::parser::extract(e, meta);
    ASSERT_EQ(e, "");
    ASSERT_EQ(meta.size(), 5);
    for (auto &field : {"og:url", "og:site_name", "article:published_time", "og:title", "og:description"}) {
        ASSERT_TRUE(meta.find(field) != meta.end());
    }
    ASSERT_EQ(meta["og:url"], "http://www.irishnews.com/news/northernirelandnews/2020/04/27/news/prescription-drugs-stolen-during-co-antrim-burglary-1916724/");
    ASSERT_EQ(meta["og:site_name"], "The Irish News");
    ASSERT_EQ(meta["article:published_time"], "2020-05-03T04:28:11+00:00");
    ASSERT_EQ(meta["og:title"], "Prescription drugs stolen during Co Antrim burglary");
    ASSERT_EQ(meta["og:description"], "PRESCRIPTION drugs have been stolen during a burglary at a chemist in Co Antrim.");

    e = article;
    uint64_t pt = html::parser::extract_time_from_html(e);
    ASSERT_EQ(pt, 1588480091);
}

TEST(html_parser, full_meta) {
    const std::string article = "<!DOCTYPE html>\n"
                            "<html>\n"
                            "  <head>\n"
                            "    <meta charset=\"utf-8\"/>\n"
                            "    <meta property=\"og:url\" content=\"http://www.irishnews.com/news/northernirelandnews/2020/04/27/news/prescription-drugs-stolen-during-co-antrim-burglary-1916724/\"/>\n"
                            "    <meta property=\"og:site_name\" content=\"The Irish News\"/>\n"
                            "    <meta property=\"article:published_time\" content=\"2020-04-27T01:00:00+00:00\"/>\n"
                            "    <meta property=\"og:title\" content=\"Prescription drugs stolen during Co Antrim burglary\"/>\n"
                            "    <meta property=\"og:description\" content=\"PRESCRIPTION drugs have been stolen during a burglary at a chemist in Co Antrim.\"/>\n"
                            "  </head>\n"
                            "  <body>\n"
                            "    <article>\n"
                            "      <h1>Prescription drugs stolen during Co Antrim burglary</h1>\n"
                            "      <address><time datetime=\"2020-04-27T01:00:00+00:00\">27 Apr 2020, 01:00</time> by <a rel=\"author\">SUZANNE MCGONAGLE</a></address>\n"
                            "      <p>A PSNI spokesman said: \"It ... .\"</p>\n"
                            "    </article>\n"
                            "  </body>\n"
                            "</html>";
    const std::string answer = "Prescription drugs stolen during Co Antrim burglary\n27 Apr 2020, 01:00 by SUZANNE MCGONAGLE\nA PSNI spokesman said: \"It ... .\"\n";

    std::string e = article;
    std::unordered_map<std::string, std::string> meta;
    html::parser::extract(e, meta);
    ASSERT_EQ(e, answer);
    ASSERT_EQ(meta.size(), 5);
    for (auto &field : {"og:url", "og:site_name", "article:published_time", "og:title", "og:description"}) {
        ASSERT_TRUE(meta.find(field) != meta.end());
    }
    ASSERT_EQ(meta["og:url"], "http://www.irishnews.com/news/northernirelandnews/2020/04/27/news/prescription-drugs-stolen-during-co-antrim-burglary-1916724/");
    ASSERT_EQ(meta["og:site_name"], "The Irish News");
    ASSERT_EQ(meta["article:published_time"], "2020-04-27T01:00:00+00:00");
    ASSERT_EQ(meta["og:title"], "Prescription drugs stolen during Co Antrim burglary");
    ASSERT_EQ(meta["og:description"], "PRESCRIPTION drugs have been stolen during a burglary at a chemist in Co Antrim.");

    e = article;
    uint64_t pt = html::parser::extract_time_from_html(e);
    ASSERT_EQ(pt, 1587949200);
}

TEST(html_parser, empty_time) {
    std::string s;
    uint64_t t1 = html::parser::extract_time(s);
    ASSERT_EQ(t1, 0);

    s = "2020-04-27T12:01:";
    uint64_t t2 = html::parser::extract_time(s);
    ASSERT_EQ(t2, 0);
}

TEST(html_parser, simple_time) {
    std::string s = "2020-04-27T12:24:37+00:00";
    uint64_t t1 = html::parser::extract_time(s);
    ASSERT_EQ(t1, 1587990277);

    s = "2020-05-21T19:52:23+00:00";
    uint64_t t2 = html::parser::extract_time(s);
    ASSERT_EQ(t2, 1590090743);

    s = "2020-05-21T19:52:23-02:15";
    uint64_t t3 = html::parser::extract_time(s);
    ASSERT_EQ(t3, 1590098843);

    s = "2020-05-21T19:52:23+02:15";
    uint64_t t4 = html::parser::extract_time(s);
    ASSERT_EQ(t4, 1590082643);
}