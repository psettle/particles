#setup
SOURCES=
LIBS=
INCLUDE=
SHADERS=

#shaders
SHADERS += shaders/fragment_shader_3d_uv_no_light.glsl
SHADERS += shaders/vertex_shader_3d_uv_no_light.glsl

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
INCLUDE += src/texture
INCLUDE += out/shaders

#source includes
SOURCES += src/file/file_api.c

SOURCES += src/shader/shader.c

SOURCES += src/texture/texture.c

SOURCES += src/math/matrix_math.c

SOURCES += src/vector/vector.c
SOURCES += src/vector/vector_test.c


SOURCES += src/core/system.c
SOURCES += src/core/object_group_core.c
SOURCES += src/core/object.c
SOURCES += src/core/camera.c

#lib includes
LIBS += lib/libSOIL.a
LIBS += lib/libglew32.a
LIBS += lib/libglfw3.a
LIBS += lib/libgdi32.a
LIBS += lib/libopengl32.a


#more setup
EXECUTABLE=out/particles.exe
EXECUTABLE_MAIN=src/main.c
EXECUTABLE_MAIN_O=$(EXECUTABLE_MAIN:%.c=out/%.o)

OBJECT_COMPILER=ObjectCompiler.exe
OBJECT_COMPILER_MAIN=src/object_compiler/main.c
OBJECT_COMPILER_MAIN_O=$(OBJECT_COMPILER_MAIN:%.c=out/%.o)

SHADER_OUTPUT=$(SHADERS:%=out/%)
SHADER_CC=FileToCharArray

LDFLAGS= -Wall -m32 -ansi -pedantic -O2
CC=gcc
CFLAGS=-c -Wall -MMD -m32 -ansi -pedantic -O2
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_FINAL=$(OBJECTS:%.o=out/%.o)
OBJECTS_FINAL_PLUS_MAIN=$(OBJECTS_FINAL)
OBJECTS_FINAL_PLUS_MAIN+=$(EXECUTABLE_MAIN_O)
OBJECTS_FINAL_PLUS_MAIN+=$(OBJECT_COMPILER_MAIN_O)
DEPENDENCIES=$(OBJECTS_FINAL:.o=.d)

INCLUDE_FORMATTED=$(addprefix -I, $(INCLUDE))

out/src/core/object_group_core.o: $(SHADER_OUTPUT)

#targets
.PHONY: all
all: $(EXECUTABLE) $(OBJECT_COMPILER)

$(EXECUTABLE): $(OBJECTS_FINAL) $(EXECUTABLE_MAIN_O)
	$(CC) $(LDFLAGS) $(OBJECTS_FINAL) $(EXECUTABLE_MAIN_O) $(LIBS) -o $@

$(OBJECT_COMPILER): $(OBJECTS_FINAL) $(OBJECT_COMPILER_MAIN_O)
	$(CC) $(LDFLAGS) $(OBJECTS_FINAL) $(OBJECT_COMPILER_MAIN_O) $(LIBS) -o $@

$(OBJECTS_FINAL_PLUS_MAIN): out/%.o : %.c
	@mkdir -p out/$(dir $<)
	$(CC) $(CFLAGS) $(INCLUDE_FORMATTED) $< -o $@

$(SHADER_OUTPUT): out/% : %
	@mkdir -p out/$(dir $<)
	$(SHADER_CC) $< $@

.PHONY: clean
clean:
	@rm -rf out/*

-include $(DEPENDENCIES)