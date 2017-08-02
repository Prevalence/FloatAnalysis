CC = g++
CFLAGS = -I${DIR_INC}

DIR_INC = .
DIR_SRC = ./src
DIR_BIN = ./bin
DIR_OBJ = .

SRC = $(wildcard ${DIR_SRC}/*.cpp)
OBJ = $(patsubst %.cpp,${DIR_OBJ}/%.o,$(notdir ${SRC}))

TARGET = main

BIN_TARGET = ${DIR_BIN}/${TARGET}

${BIN_TARGET}:${OBJ}
	$(CC)  $(OBJ) -o $@

${DIR_OBJ}/%.o:${DIR_SRC}/%.cpp
	$(CC) ${CFLAGS} -c $< -o $@

.PHONY:clean
clean:
	
	-rm main.o
	
