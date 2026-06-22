#!/bin/bash
docker build --no-cache -t  api-rainhas .
#docker run -d -p 18080:18080 --name servidor-rainhas api-rainhas

docker tag api-rainhas pedromujica1/api-rainhas:latest
docker push pedromujica1/api-rainhas:latest