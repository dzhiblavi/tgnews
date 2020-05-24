from flask import Flask
from flask import request
import sys
import os
from neural import *


app = Flask(__name__)


def make_dirs(base):
    for lang in langs:
        for cat in categories:
            try:
                os.makedirs(form_path(base, lang, cat, ''))
            except Exception:
                pass


@app.route('/<path:file_path>', methods=['PUT'])
def process_put_request(file_path):
    print("PyServer: processing put request: " + file_path)
    executors[request.headers.get("Language")].submit_data([file_path, str(request.data),
                                                            request.headers.get("header"),
                                                            request.headers.get("published_time"),
                                                            request.headers.get("og_url")])
    return ""


@app.route('/threads', methods=['GET'])
def process_get_request():
    period = request.headers.get('period')
    cat = request.headers.get('category')
    lang = request.headers.get('lang_code')
    return json.dumps(process_get(base, period, lang, cat), indent=2)


if __name__ == '__main__':
    base = str(Path(sys.argv[0]).parent) + '/'
    executors = {'ru': TGServerExecutor(base, 8, 'ru'), 'en': TGServerExecutor(base, 8, 'en')}
    make_dirs(base)
    app.run(host='0.0.0.0', port=int(sys.argv[1]), debug=False)
