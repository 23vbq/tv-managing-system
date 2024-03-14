.PHONY = all

CC = g++

MAIN := main.cpp
SRCS := src/logger.cpp src/clientsocket_win.cpp
OUT := build/tmsc.exe
INCLUDES := -Iinclude
LIBS := -lws2_32

all: tmsc

tmsc:
	${CC} ${MAIN} ${INCLUDES} ${SRCS} -o ${OUT} ${LIBS}
# chmod 744 ${OUT}

debug:
	${CC} -g -D DEBUG ${MAIN} ${INCLUDES} ${SRCS} -o ${OUT} ${LIBS}
# chmod 777 ${OUT}

# install: tmsd
# 	cp ${OUT} /usr/bin
# 	cp tmsd.service /etc/systemd/system

clean:
	del ${OUT}