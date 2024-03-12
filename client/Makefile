.PHONY = all

CC = g++

MAIN := main.cpp
SRCS := src/logger.cpp
OUT := build/tmsc.exe
INCLUDES := -Iinclude

all: tmsc

tmsc:
	${CC} ${MAIN} ${INCLUDES} ${SRCS} -o ${OUT} 
# chmod 744 ${OUT}

debug:
	${CC} -g -D DEBUG ${MAIN} ${INCLUDES} ${SRCS} -o ${OUT}
# chmod 777 ${OUT}

# install: tmsd
# 	cp ${OUT} /usr/bin
# 	cp tmsd.service /etc/systemd/system

clean:
	del ${OUT}