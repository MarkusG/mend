FROM ubuntu:20.04

WORKDIR /app

RUN export DEBIAN_FRONTEND=noninteractive; \
    export DEBCONF_NONINTERACTIVE_SEEN=true; \
	echo 'tzdata tzdata/Areas select Etc' | debconf-set-selections; \
	echo 'tzdata tzdata/Zones/Etc select UTC' | debconf-set-selections; \
	apt update && apt install -y --no-install-recommends \
	meson \
	pkg-config \
	ninja-build \
	build-essential \
	libpq5 \
	libpq-dev

COPY . .

RUN meson .build
RUN ninja -C .build
