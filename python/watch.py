from flask import Flask
from flask import request
import sys
from neural import *


app = Flask(__name__)
executor = TGExecutor(16)


@app.route('/<path:file_path>', methods=['PUT'])
def process_put_request(file_path):
    print("processing put request: " + file_path)
    executor.submit_data([file_path, request.headers.get("Language"), request.data])
    return ""


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=int(sys.argv[1]), debug=False)
