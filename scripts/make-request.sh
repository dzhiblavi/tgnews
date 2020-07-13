#!/bin/bash
set -e

while [ -n "$1" ]; do
	case "$1" in
		-get) {
			MODE="GET"
		} ;;

	    -put) {
			MODE="PUT"
		} ;;

		-del) {
			MODE="DELETE"
		} ;;

		-file) {
			shift
			FILE="$1"
		} ;;

		-port) {
			shift
			PORT="$1"
		} ;;

		-what) {
			shift
			WHAT="$1"
		} ;;

        -keep) {
            KEEP="TRUE"
        } ;;

		*) {
			echo "invalid key: $1"
			exit 1
		}
	esac
	shift
done

case "$MODE" in
	GET) {
		curl --dump-header - -X GET \
			"localhost:${PORT}/${WHAT}"
	} ;;

	PUT) {
        if ! [ -n "${KEEP}" ]; then
		    curl --dump-header - -X PUT \
                -H "Content-Type: text/html" \
                -H "Cache-Control: max-age=30000000" --data "@${FILE}" \
			    "localhost:${PORT}/${FILE}"
        else
		    curl --dump-header - -X PUT \
                -H "Connection: Keep-Alive" \
                -H "Keep-Alive: 10" \
                -H "Content-Type: text/html" \
                -H "Cache-Control: max-age=30000000" --data "@${FILE}" \
			    "localhost:${PORT}/${FILE}"
        fi
	} ;;

	DELETE) {
		curl --dump-header - -X DELETE \
			"localhost:${PORT}/${FILE}"
	} ;;
esac
