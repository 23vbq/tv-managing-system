.PHONY = all

CC = g++

MAIN := main.cpp
SRCS := src/signalcallbacks.cpp
SHAREDSRC := ../shared/src/serializer.cpp ../shared/src/endpointsettings.cpp
OUT := build/tmse
INCLUDES := -Iinclude -I../shared/include

all: tmse

tmsd: builddir
	${CC} ${MAIN} ${INCLUDES} ${SHAREDSRC} ${SRCS} -o ${OUT}
	chmod 744 ${OUT}

debug: builddir
	${CC} -g -D DEBUG ${MAIN} ${INCLUDES} ${SHAREDSRC} ${SRCS} -o ${OUT}
	chmod 777 ${OUT}

install: tmse
	cp ${OUT} /usr/bin

clean:
	rm ${OUT}

builddir:
	@mkdir -p build