PROGRAM = carousel

OBJ = $(PROGRAM).o helper/load_file.o helper/matrix.o shape/cylinder.o shape/cube.o shape/polygon.o helper/draw.o helper/create_shader_program.o
CPPFLAGS = -g -O2 -Wall -Wextra -std=c++11 -I$(CURDIR)

LDLIBS = -lm
CC = g++

ifneq ($(OS), Windows_NT)
	UNAME_S = $(shell uname -s)
	ifeq ($(UNAME_S), Darwin)
		CPPFLAGS += -Wno-deprecated-declarations
		LDLIBS += -framework GLUT -framework OpenGL
	else
		LDLIBS += -lglut -lGLEW -lGL
	endif
endif

$(PROGRAM): $(OBJ)
	$(CC) -o $@ $^ $(CPPFLAGS) $(LDLIBS)

.PHONY: all
all: $(PROGRAM)

.PHONY: clean
clean:
	$(RM) $(OBJ)
	$(RM) $(PROGRAM)

.PHONY: run
run: $(PROGRAM)
	./$(PROGRAM)

.PHONY: test
test: $(PROGRAM)
	valgrind ./$(PROGRAM) --leak-check=full -v
