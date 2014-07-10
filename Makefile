CFLAGS = `pkg-config --cflags gtk+-3.0`
LIBS = `pkg-config --libs gtk+-3.0` -ljalali -export-dynamic

SRC = calendar.c
BIN = calendar.out

${BIN} : ${SRC}
	${CC} ${CFLAGS} ${SRC} ${LIBS} -o ${BIN}

clean:
	rm -f ${BIN}
