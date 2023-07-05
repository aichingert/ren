podman run --pod=postgre-sql \
-v ~/recipes/server/data:/var/lib/postgresql/data:Z \
-e POSTGRES_PASSWORD=app \
-e POSTGRES_USER=app \
--name db \
-d postgres