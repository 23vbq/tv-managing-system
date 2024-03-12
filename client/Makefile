.PHONY = all

CC = g++

MAIN := main.cpp
# SRCS := src/signalcallbacks.cpp src/configloader.cpp src/config.cpp src/serversocket.cpp src/commandhandler.cpp src/commandfunctions.cpp
OUT := build/tmsc.exe
# INCLUDES := -Iinclude

all: tmsd

tmsd:
# ${INCLUDES} ${SRCS} 
	${CC} ${MAIN} -o ${OUT} 
	chmod 744 ${OUT}

debug:
	${CC} -g -D DEBUG ${MAIN} -o ${OUT}
	chmod 777 ${OUT}

# install: tmsd
# 	cp ${OUT} /usr/bin
# 	cp tmsd.service /etc/systemd/system

clean:
	rm ${OUT}