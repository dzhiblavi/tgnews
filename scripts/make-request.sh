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
		curl --dump-header - -X PUT \
            -H "Content-Type: text/html" \
            -H "Cache-Control: max-age=30000000" --data "@${FILE}" \
			"localhost:${PORT}/${FILE}"
	} ;;

	DELETE) {
		curl --dump-header - -X DELETE \
			"localhost:${PORT}/${FILE}"
	} ;;
esac
