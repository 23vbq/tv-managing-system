.PHONY = all

CC = c++

SRCS := main.cpp
OUT := build/tmsd

all: tv-managing-server

tv-managing-server:
	${CC} ${SRCS} -o ${OUT}
	chmod 744 ${OUT}

install: tv-managing-server
	cp ${OUT} /usr/bin
	cp tmsd.service /etc/systemd/system

clean:
	rm ${OUT}