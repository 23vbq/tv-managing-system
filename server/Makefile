.PHONY = all

CC = c++

SRCS := main.cpp signalcallbacks.cpp
OUT := build/tmsd
INCLUDES := -Iinclude

all: tv-managing-server

tv-managing-server:
	${CC} ${INCLUDES} ${SRCS} -o ${OUT}
	chmod 744 ${OUT}

install: tv-managing-server
	cp ${OUT} /usr/bin
	cp tmsd.service /etc/systemd/system

clean:
	rm ${OUT}