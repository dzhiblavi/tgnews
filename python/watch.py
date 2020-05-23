from flask import Flask
from flask import request
import sys
import os
from neural import *


app = Flask(__name__)
executors = {'ru': TGExecutor(8, 'ru'), 'en': TGExecutor(8, 'en')}


def make_dirs():
    for lang in langs:
        for cat in categories:
            try:
                os.makedirs(form_path(lang, cat, ''))
            except Exception:
                pass


@app.route('/<path:file_path>', methods=['PUT'])
def process_put_request(file_path):
    print("PyServer: processing put request: " + file_path)
    executors[request.headers.get("Language")].submit_data([file_path, str(request.data)])
    return ""


if __name__ == '__main__':
    make_dirs()
    app.run(host='0.0.0.0', port=int(sys.argv[1]), debug=False)
