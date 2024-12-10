#!/bin/sh
docker build -t microdevtools-image .
docker run --name microdevtools -d -p 2310:2310 -p 2443:2443 -v $(pwd)/src:/service/src microdevtools-image
