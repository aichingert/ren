#/bin/sh

sudo docker run --rm=true \
  --name receipts \
  -v ${PWD}/pgdata:/var/lib/postgresql/data \
  -e POSTGRES_USER=app \
  -e POSTGRES_PASSWORD=app \
  -e POSTGRES_DB=db \
  -p 5432:5432 postgres
