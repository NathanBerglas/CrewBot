EXEC = bin/CrewBot.exe
CC = clang
DEF = "-DNODEF"
OPT = -O0
CFLAGS = -fdiagnostics-color=always -fansi-escape-codes -g -std=c17 -Wall -Werror -I include/ -MMD ${OPT} ${DEF} -DVERBOSE -DSKIPSORT
LIB = $(wildcard lib/*.o)
SRC = $(wildcard src/*.c)
OBJECTS = $(SRC:src/%.c=bin/%.o)
DEPENDS = $(OBJECTS:.o=.d)

${EXEC}: ${OBJECTS}
		${CC} ${CFLAGS} ${OBJECTS} ${LIB} ${DEF} -o ${EXEC}

bin/%.o: src/%.c
	${CC} ${CFLAGS} -c $< -o $@

# copy the generated .d files which provides dependencies for each .c file
-include ${DEPENDS}

run: ${EXEC}
	./${EXEC}

.PHONY: run clean

clean:
		rm -f bin/*.o bin/*.d ${EXEC}