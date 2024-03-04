.PHONY = all

CC = g++

SRCS := main.cpp signalcallbacks.cpp configloader.cpp config.cpp
OUT := build/tmsd
INCLUDES := -Iinclude

all: tmsd

tmsd:
	${CC} ${INCLUDES} ${SRCS} -o ${OUT}
	chmod 744 ${OUT}

debug:
	${CC} -g ${INCLUDES} ${SRCS} -o ${OUT}
	chmod 777 ${OUT}

install: tmsd
	cp ${OUT} /usr/bin
	cp tmsd.service /etc/systemd/system

clean:
	rm ${OUT}