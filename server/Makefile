.PHONY = all

CC = g++

MAIN := main.cpp
SRCS := src/signalcallbacks.cpp src/configloader.cpp src/config.cpp src/serversocket.cpp src/commandhandler.cpp src/commandfunctions.cpp src/endpointmanager.cpp
SHAREDSRC := ../shared/src/serializer.cpp ../shared/src/endpointsettings.cpp
OUT := build/tmsd
INCLUDES := -Iinclude -I../shared/include

all: tmsd

tmsd: builddir
	${CC} ${MAIN} ${INCLUDES} ${SHAREDSRC} ${SRCS} -o ${OUT}
	chmod 744 ${OUT}

debug: builddir
	${CC} -g -D DEBUG ${MAIN} ${INCLUDES} ${SHAREDSRC} ${SRCS} -o ${OUT}
	chmod 777 ${OUT}

install: tmsd
	cp ${OUT} /usr/bin
	cp tmsd.service /etc/systemd/system

clean:
	rm ${OUT}

builddir:
	@mkdir -p build