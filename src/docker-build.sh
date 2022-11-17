#!/bin/sh

cd ../..
docker build --no-cache -t acme-onem2m-cse -f tools/Docker/Dockerfile .
docker tag acme-onem2m-cse <my-container-registry>
docker push <my-container-registry>
