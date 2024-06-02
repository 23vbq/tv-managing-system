.PHONY = all

CC = g++

MAIN := main.cpp
SRCS := src/commandfunctions.cpp src/endpointmanager.cpp src/clientsocket.cpp
SHAREDSRC := ../shared/src/serializer.cpp ../shared/src/endpointsettings.cpp ../shared/src/serversocket.cpp ../shared/src/configloader.cpp ../shared/src/config.cpp ../shared/src/commandhandler.cpp ../shared/src/authmanager.cpp ../shared/src/signalcallbacks.cpp
OUT := build/tmsd
INCLUDES := -Iinclude -I../shared/include

CONFIGDIR := /etc/tmsd

all: tmsd

tmsd: builddir
	@echo "[#] Building PRODUCTION version"
	${CC} ${MAIN} ${INCLUDES} ${SHAREDSRC} ${SRCS} -o ${OUT}
	chmod 744 ${OUT}

debug: builddir
	@echo "[#] Building DEBUG version"
	${CC} -g -D DEBUG ${MAIN} ${INCLUDES} ${SHAREDSRC} ${SRCS} -o ${OUT}
	chmod 777 ${OUT}

install: tmsd defaults
	@echo "[#] Copying binaries and service"
	cp ${OUT} /usr/bin
	cp tmsd.service /etc/systemd/system

clean:
	@echo "[#] Cleaning build"
	rm ${OUT}

builddir:
	@echo "[#] Creating build dir"
	@mkdir -p build

defaults:
	@echo "[#] Copying default configurations"
	@mkdir -p ${CONFIGDIR}
	cp settings.cfg.def ${CONFIGDIR}/settings.cfg
	cp authkey.def ${CONFIGDIR}/authkey
	@mkdir -p ${CONFIGDIR}/epconf
