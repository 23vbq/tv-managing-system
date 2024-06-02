.PHONY = all

CC = g++

MAIN := main.cpp
SRCS := src/commandfunctions.cpp src/settingsmanager.cpp src/windowmanager.cpp src/slideshowmanager.cpp
SHAREDSRC := ../shared/src/serializer.cpp ../shared/src/endpointsettings.cpp ../shared/src/configloader.cpp ../shared/src/config.cpp ../shared/src/serversocket.cpp ../shared/src/authmanager.cpp ../shared/src/commandhandler.cpp ../shared/src/signalcallbacks.cpp
OUT := build/tmse
INCLUDES := -Iinclude -I../shared/include
LIBS := -lX11

CONFIGDIR := /etc/tmse

all: tmse

tmsd: builddir
	@echo "[#] Building PRODUCTION version"
	${CC} ${MAIN} ${INCLUDES} ${LIBS} ${SHAREDSRC} ${SRCS} -o ${OUT}
	chmod 744 ${OUT}

debug: builddir
	@echo "[#] Building DEBUG version"
	${CC} -g -D DEBUG ${MAIN} ${INCLUDES} ${LIBS} ${SHAREDSRC} ${SRCS} -o ${OUT}
	chmod 777 ${OUT}

install: tmse defaults
	@echo "[#] Copying binaries"
	cp ${OUT} /usr/bin

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