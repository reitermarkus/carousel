PROGRAM = carousel

OBJ = $(PROGRAM).o load_shader.o matrix.o
CFLAGS = -g -Wall -Wextra

LDLIBS=-lm

ifneq ($(OS),Windows_NT)
	UNAME_S = $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		CFLAGS += -Wno-deprecated-declarations
		LDLIBS += -framework GLUT -framework OpenGL
	else
		LDLIBS += -lglut -lGLEW -lGL
	endif
endif

.PHONY: all clean run

$(PROGRAM): $(OBJ)
	 $(CC) -o $@ $^ -O2 $(CFLAGS) $(LDLIBS)

clean:
	rm -f *.o $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM)
