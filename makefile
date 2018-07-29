# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := NO

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

CC     := gcc
CXX    := g++
LD     := g++
AR     := ar rc
RANLIB := ranlib

DEBUG_CFLAGS     := -Wall -Wno-format -g -DDEBUG
RELEASE_CFLAGS   := -Wall -Wno-unknown-pragmas -Wno-format -O3

TCCBASE          := /usr/src/develop/BerlinRoofNet/brn-tools/click-brn-libs/
INCLUDE_FLAGS    := -I${TCCBASE}/include
LIB_FLAGS        := -L${TCCBASE}/lib -ltcc -ldl -lc


CFLAGS           := ${INCLUDE_FLAGS}
LDFLAGS          :=
EXTRA_LIBS       := ${LIB_FLAGS}

#****************************************************************************
# Makefile code common to all platforms
#****************************************************************************

CFLAGS   := ${CFLAGS}   ${DEFS}
CXXFLAGS := ${CXXFLAGS} ${DEFS}

#****************************************************************************
# Targets of the build
#****************************************************************************

OUTPUT := tiny_c
LIB_OUTPUT := libtiny_c.a

all: ${OUTPUT}

#****************************************************************************
# Source files
#****************************************************************************

SRCS := examples_test.c functions.c main.c pointer_test.c scan_type_test.c
LIB_SRCS := buffered_reader.c c_source_data_functions.c c_source_reader.c hashmap.c  read_from_line.c  reflection.c  scan_command_line.c  utile.c

# Add on the sources for libraries
SRCS := ${SRCS}

OBJS := $(addsuffix .o,$(basename ${SRCS}))
LIB_OBJS := $(addsuffix .o,$(basename ${LIB_SRCS}))

#****************************************************************************
# Output
#****************************************************************************

${LIB_OUTPUT}: ${LIB_OBJS}
	${AR} $@ $?

lib: ${LIB_OUTPUT}

${OUTPUT}: lib ${OBJS}
	${LD} -o $@ ${LDFLAGS} ${OBJS} ${LIB_OUTPUT} ${LIBS} ${EXTRA_LIBS}

#****************************************************************************

# Rules for compiling source files to object files
%.o : %.cpp
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

%.o : %.c
	${CC} -c ${CFLAGS} ${INCS} $< -o $@

# Rules for compiling source files to object files
%.o : %.cpp
	${CXX} -c ${CXXFLAGS} ${INCS} $< -o $@

%.o : %.c
	${CC} -c ${CFLAGS} ${INCS} $< -o $@

clean:
	-rm -f ${OBJS} ${LIB_OBJS} ${OUTPUT} ${LIBOUTPUT}
