#setup
SOURCES=
LIBS=
INCLUDE=

#header includes
INCLUDE += include
INCLUDE += include/GL
INCLUDE += include/GLFW

INCLUDE += src
INCLUDE += src/file
INCLUDE += src/general
INCLUDE += src/shader
INCLUDE += src/math
INCLUDE += src/core
INCLUDE += src/vector

#source includes
SOURCES += src/main.c

SOURCES += src/file/file_api.c

SOURCES += src/shader/shader.c

SOURCES += src/math/matrix_math.c

SOURCES += src/vector/vector.c

SOURCES += src/core/system.c

#lib includes
LIBS += lib/libglew32.a
LIBS += lib/libglfw3.a
LIBS += lib/libgdi32.a
LIBS += lib/libopengl32.a


#more setup
EXECUTABLE=out/particles.exe
LDFLAGS= -Wall -m32 -ansi -pedantic -O2
CC=gcc
CFLAGS=-c -Wall -MMD -m32 -ansi -pedantic -O2
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_FINAL=$(OBJECTS:%.o=out/%.o)
DEPENDENCIES=$(OBJECTS_FINAL:.o=.d)

INCLUDE_FORMATTED=$(addprefix -I, $(INCLUDE))

#targets
.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS_FINAL)
	$(CC) $(LDFLAGS) $(OBJECTS_FINAL) $(LIBS) -o $@


$(OBJECTS_FINAL): out/%.o : %.c
	@mkdir -p out/$(dir $<)
	$(CC) $(CFLAGS) $(INCLUDE_FORMATTED) $< -o $@

.PHONY: clean
clean:
	@rm -rf out/*

-include $(DEPENDENCIES)