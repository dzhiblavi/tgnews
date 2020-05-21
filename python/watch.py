from flask import Flask
from flask_api import status
from tgutil import Executor
import sys

app = Flask(__name__)
executor = Executor(8)


@app.route('/html/<path:file_path>/')
def parse_html(file_path):
    executor.parse_html_dir(file_path, 'out/html/')
    return "", status.HTTP_200_OK


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=int(sys.argv[1]), debug=False)

