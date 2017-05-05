PROGRAM = carousel

OBJ = $(patsubst %.c, %.o, $(wildcard *.c */*.c))

CFLAGS = -g -O2 -Wall -Werror -Wextra -std=c11 "-I$(CURDIR)" -D_POSIX_C_SOURCE=200112L

LDLIBS = -lm

ifeq ($(OS), Windows_NT)
  LDLIBS += -lglut -lGLEW -IGL -lopengl32
else
  ifeq ($(shell uname -s), Darwin)
    CFLAGS += -Wno-deprecated-declarations
    LDLIBS += -framework GLUT -framework OpenGL
  else
    LDLIBS += -lglut -lGLEW -lGL
  endif
endif

$(PROGRAM): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDLIBS)

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
