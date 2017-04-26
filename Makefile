PROGRAM = carousel

HELPERS = $(addprefix helper/, load_file.o matrix.o draw.o create_shader_program.o keymap.o)
SHAPES = $(addprefix shape/, abstract_shape.o cylinder.o cube.o cuboid.o cone.o flattened_cone.o hyper_rectangle.o polygon.o)
OBJ_PARSER = $(addprefix parser/, obj_parser.o list.o)

OBJ = $(PROGRAM).o $(HELPERS) $(SHAPES) $(OBJ_PARSER)

CFLAGS = -g -O2 -Wall -Werror -Wextra -std=c11 -I$(CURDIR)

LDLIBS = -lm

ifneq ($(OS),Windows_NT)
  UNAME_S = $(shell uname -s)
  ifeq ($(UNAME_S),Darwin)
    CFLAGS += -Wno-deprecated-declarations
    LDLIBS += -framework GLUT -framework OpenGL
  else
    LDLIBS += -lglut -lGLEW -lGL
  endif
else
  LDLIBS += -lglut -lGLEW -IGL -lopengl32
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
