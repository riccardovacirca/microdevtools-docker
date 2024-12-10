FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y --no-install-recommends \
    clang make curl git python3 autoconf libtool-bin libexpat1-dev \
    cmake libssl-dev libmariadb-dev libpq-dev libsqlite3-dev \
    unixodbc-dev libapr1-dev libaprutil1-dev libaprutil1-dbd-mysql \
    libaprutil1-dbd-pgsql libaprutil1-dbd-sqlite3 libjson-c-dev \
    sqlite3 memcached ca-certificates \
    && apt-get clean && rm -rf /var/lib/apt/lists/*
WORKDIR /service
COPY ./Makefile .
CMD ["sh", "-c", "memcached -u memcache -d && exec bash"]
