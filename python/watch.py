from flask import Flask
from flask import request
from flask_api import status
from tgutil import Executor
import sys

app = Flask(__name__)
executor = Executor(8)


@app.route('/<path:file_path>', methods=['PUT'])
def process_put_request(file_path):
    print("processing put request: " + file_path)
    print(str(request.data))
    return ""


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=int(sys.argv[1]), debug=False)
