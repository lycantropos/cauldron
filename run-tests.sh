#!/usr/bin/env bash

docker-compose up --exit-code-from cauldron
STATUS=$?

docker-compose down

if [ "$STATUS" -eq "0" ]; then
	echo "Tests passed";
else
	echo "Tests failed to pass"
fi

exit ${STATUS}
