from abc import ABC
import html.parser as hp
import sys
import concurrent.futures as cf
from os import listdir, mkdir
from os.path import isdir, isfile


class HTMLParser(hp.HTMLParser, ABC):
    def __init__(self):
        super(HTMLParser, self).__init__(convert_charrefs=True)
        self.collected = ""

    def handle_starttag(self, tag, attrs):
        pass

    def handle_endtag(self, tag):
        pass

    def handle_data(self, data):
        self.collected += data + " "

    def cleanup(self):
        self.collected = ""


class Executor(cf.ThreadPoolExecutor):
    def __init__(self, max_workers):
        super().__init__(max_workers=max_workers)

    def parse_html_dir(self, path, out_path):
        self.submit(parse_html_dir, path, out_path)


def parse_html(path, outpath):
    parser = HTMLParser()
    with open(path, 'r') as file:
        parser.feed(file.read())
    with open(outpath, 'w') as outfile:
        outfile.write(parser.collected)


def parse_html_dir_exec(dir_executor, file_executor, path, outpath):
    if isfile(path):
        file_executor.submit(parse_html, path, outpath)
    else:
        for val in listdir(path):
            subpath = path + "/" + val
            subout = outpath + "/" + val
            if isfile(subpath):
                file_executor.submit(parse_html, subpath, subout)
            elif isdir(subpath):
                mkdir(outpath)
                dir_executor.submit(parse_html_dir_exec, dir_executor, file_executor, subpath, subout);


def parse_html_dir(path, out_path):
    dir_executor = Executor(2)
    file_executor = Executor(8)
    mkdir(out_path)
    parse_html_dir_exec(dir_executor, file_executor, path, out_path)
    dir_executor.shutdown()
    file_executor.shutdown()


if __name__ == '__main__':
    parse_html_dir(sys.argv[1], sys.argv[1])
