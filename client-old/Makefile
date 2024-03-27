.PHONY = all

CC = g++

MAIN := main.cpp
SRCS := src/logger.cpp src/clientsocket_win.cpp src/windowmanager.cpp
SHAREDSRC := ../shared/src/serializer.cpp ../shared/src/endpointsettings.cpp
OUT := build/tmsc.exe
INCLUDES := -Iinclude -I../shared/include
LIBS := -lws2_32 -lgdi32

all: tmsc

tmsc:
	${CC} ${MAIN} ${INCLUDES} ${SHAREDSRC} ${SRCS} -o ${OUT} ${LIBS}
# chmod 744 ${OUT}

debug:
	${CC} -g -D DEBUG ${MAIN} ${INCLUDES} ${SHAREDSRC} ${SRCS} -o ${OUT} ${LIBS}
# chmod 777 ${OUT}

# install: tmsd
# 	cp ${OUT} /usr/bin
# 	cp tmsd.service /etc/systemd/system

clean:
	del ${OUT}