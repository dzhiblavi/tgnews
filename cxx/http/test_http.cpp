#include "gtest.h"

#include "request.h"
#include "parser.h"

using namespace http;

TEST(http_request, to_string) {
    request<true> req{
            "GET",
            "/article.html",
            "HTTP/1.1",
            "text/html",
            10999,
            9,
            "<content>"
    };
    std::cout << req.to_string() << std::endl;
}

TEST(http_request, parse_put) {
    std::string s = "PUT /article.html HTTP/1.1\r\n"
                    "Content-Type: text/html\r\n"
                    "Cache-Control: max-age=10999\r\n"
                    "Content-Length: 9\r\n"
                    "\r\n"
                    "<content>";
    parser<true> pars;
    ASSERT_EQ(s.size(), pars.append(s.data(), 0, s.size()));
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}

TEST(http_request, parse_get) {
    std::string s = "GET /article.html HTTP/1.1";
    parser<true> pars;
    ASSERT_EQ(s.size(), pars.append(s.data(), 0, s.size()));
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}

TEST(http_request, parse_delete) {
    std::string s = "DELETE /article.html HTTP/1.1";
    parser<true> pars;
    ASSERT_EQ(s.size(), pars.append(s.data(), 0, s.size()));
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}

TEST(http_request, parse_parts) {
    std::vector<std::string> s = {
            "DELETE /artic",
            "le.html HTT",
            "P/1.1",
    };
    parser<true> pars;
    for (auto const &str : s)
        ASSERT_EQ(str.size(), pars.append(str.data(), 0, str.size()));
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}

TEST(http_request, parse_parts3) {
    std::vector<std::string> s = {
            "DELETE /a",
            "rtic",
            "le.html H",
            "T",
            "T",
            "P/1",
            ".1",
    };
    parser<true> pars;
    for (auto const &str : s)
        ASSERT_EQ(str.size(), pars.append(str.data(), 0, str.size()));
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}

TEST(http_request, parse_parts4) {
    std::vector<std::string> s = {
            "DELETE /art",
            "ic",
            "le.html ",
            "H",
            "T",
            "T",
            "P",
            "/",
            "1",
            ".",
            "1",
    };
    parser<true> pars;
    for (auto const &str : s)
        ASSERT_EQ(str.size(), pars.append(str.data(), 0, str.size()));
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}

TEST(http_request, parse_put_parts) {
    std::vector<std::string> s = {
            "PUT /article.html HTT",
            "P/1.1\r",
            "\n",
            "Content-Type: text/html\r",
            "\n",
            "Cache-Control: max-a",
            "ge=10999\r\n",
            "Content-Length: 9\r",
            "\n",
            "\r",
            "\n",
            "<content>"
    };
    parser<true> pars;
    for (auto const &str : s)
        ASSERT_EQ(str.size(), pars.append(str.data(), 0, str.size()));
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}

TEST(http_request, parse_put_parts2) {
    std::vector<std::string> s = {
            "PUT /article.html H",
            "T",
            "T",
            "P/1",
            ".1",
            "\r",
            "\n",
            "Content-Type: text/html",
            "\r",
            "\n",
            "Cache-Control: max-a",
            "ge=10999\r\n",
            "Content-Length: 9",
            "\r",
            "\n",
            "\r",
            "\n",
            "<content>"
    };
    parser<true> pars;
    for (auto const &str : s)
        ASSERT_EQ(str.size(), pars.append(str.data(), 0, str.size()));
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}

TEST(http_request, parse_long) {
    std::string s = "PUT /article1.html HTTP/1.1\r\n"
                    "Content-Type: AAAAA\r\n"
                    "Cache-Control: max-age=11111\r\n"
                    "Content-Length: 9\r\n"
                    "\r\n"
                    "<content>"
                    "PUT /article2.html HTTP/1.1\r\n"
                    "Content-Type: BBBBB\r\n"
                    "Cache-Control: max-age=99999\r\n"
                    "Content-Length: 9\r\n"
                    "\r\n"
                    "<content>";
    parser<true> pars;
    int x = pars.append(s.data(), 0, s.size());
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
    pars.append(s.data(), x, s.size());
    ASSERT_TRUE(pars.ready());
    std::cout << pars.get_request().to_string() << std::endl;
}