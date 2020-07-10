FROM ubuntu:20.04

WORKDIR /app

RUN export DEBIAN_FRONTEND=noninteractive; \
    export DEBCONF_NONINTERACTIVE_SEEN=true; \
	echo 'tzdata tzdata/Areas select Etc' | debconf-set-selections; \
	echo 'tzdata tzdata/Zones/Etc select UTC' | debconf-set-selections; \
	apt update && apt install -y --no-install-recommends \
	bash \
	meson \
	pkg-config \
	ninja-build \
	build-essential \
	postgresql \
	libpq5 \
	libpq-dev

COPY . .

RUN meson .build
RUN ninja -C .build

# RUN ./test/db-init.sh
