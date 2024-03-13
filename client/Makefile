.PHONY = all

CC = g++

MAIN := main.cpp
SRCS := src/logger.cpp src/clientsocket_win.cpp
OUT := build/tmsc.exe
INCLUDES := -Iinclude
LIBS := -L"ws2_32"

all: tmsc

tmsc:
	${CC} ${MAIN} ${LIBS} ${INCLUDES} ${SRCS} -o ${OUT} 
# chmod 744 ${OUT}

debug:
	${CC} -g -D DEBUG ${MAIN} ${LIBS} ${INCLUDES} ${SRCS} -o ${OUT}
# chmod 777 ${OUT}

# install: tmsd
# 	cp ${OUT} /usr/bin
# 	cp tmsd.service /etc/systemd/system

clean:
	del ${OUT}