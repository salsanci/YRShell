docker stop $(docker ps -a -q)
docker rm $(docker ps -a -f status=exited -q)
docker rmi $(docker images -a -q)
