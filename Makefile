# @file Makefile
# @brief `make` instructions for building and testing FFTW-Age.
# @authors Mike S Wang (https://github.com/MikeSWang)
#

# ========================================================================
# Configuration
# ========================================================================

REPONAME := FFTW-Age
PROGNAME := fftw-age


# ------------------------------------------------------------------------
# Preamble
# ------------------------------------------------------------------------

# Escape characters
COMMA := ,


# ------------------------------------------------------------------------
# Directories
# ------------------------------------------------------------------------

# Repository root
DIR_ROOT := $(shell pwd)

# Package directories
DIR_INCLUDE := ${DIR_ROOT}/include
DIR_SRC := ${DIR_ROOT}/src
DIR_PROG := ${DIR_ROOT}/main

# Build (aub-)directories
DIR_BUILD := ${DIR_ROOT}/build

DIR_BUILDOBJ := ${DIR_BUILD}/obj
DIR_BUILDBIN := ${DIR_BUILD}/bin


# ------------------------------------------------------------------------
# Compilation
# ------------------------------------------------------------------------

# -- OS ------------------------------------------------------------------

OS := $(shell uname -s)


# -- Compiler ------------------------------------------------------------

# Assume GCC compiler by default. [adapt]
ifeq (${OS}, Linux)

CXX ?= g++

else ifeq (${OS}, Darwin)

# Use GCC compiler from Homebrew (brew formula 'gcc').
CXX ?= $(shell find $(brew --prefix gcc)/bin -type f -name 'g++*')
# Use LLVM compiler from Homebrew (brew formula 'llvm').
# CXX ?= $(shell brew --prefix llvm)/bin/clang++

else  # OS

CXX ?= g++

endif  # OS

# Assume default remover. [adapt]
RM ?= rm -f


# -- Dependencies --------------------------------------------------------

# Dependencies determined by `pkg-config`
DEPS := fftw3

DEP_INCLUDES := $(shell pkg-config --silence-errors --cflags-only-I ${DEPS})
DEP_CXXFLAGS := $(shell pkg-config --silence-errors --cflags-only-other ${DEPS})
DEP_LDFLAGS := $(shell pkg-config --silence-errors --libs-only-other --libs-only-L ${DEPS})
DEP_LDLIBS := $(shell pkg-config --silence-errors --libs-only-l ${DEPS})

# OpenMP dependency: assume GCC implementation by default [adapt]
ifeq (${OS}, Linux)

# Use GCC implementation.
OMP_CXXFLAGS ?= -fopenmp
OMP_LDFLAGS ?= -fopenmp
# OMP_LDLIBS ?= -lgomp

# Use Intel implementation.
# OMP_CXXFLAGS ?= -qopenmp
# OMP_LDFLAGS ?= -qopenmp
# # OMP_LDLIBS ?= -liomp5

else ifeq (${OS}, Darwin)

# Use GCC implementation.
OMP_CXXFLAGS ?= -fopenmp
OMP_LDFLAGS ?= -fopenmp
# OMP_LDLIBS ?= -lgomp

# Use LLVM implementation from Homebrew (brew formula 'libomp').
# OMP_CXXFLAGS ?= -I$(shell brew --prefix libomp)/include -Xpreprocessor -fopenmp
# OMP_LDFLAGS ?= -L$(shell brew --prefix libomp)/lib
# OMP_LDLIBS ?= -lomp

else  # OS

OMP_CXXFLAGS ?= -fopenmp
OMP_LDFLAGS ?= -fopenmp
# OMP_LDLIBS ?= -lgomp

endif  # OS


# -- Options -------------------------------------------------------------

INCLUDES += -I${DIR_INCLUDE} ${DEP_INCLUDES}
CPPFLAGS += -MMD -MP
CXXFLAGS += -std=c++17 -Wall -O3 ${DEP_CXXFLAGS} ${OMP_CXXFLAGS}
LDFLAGS += \
	$(addprefix -Wl${COMMA}-rpath${COMMA},$(patsubst -L%,%,${DEP_LDFLAGS})) \
	${DEP_LDFLAGS} \
	${OMP_LDFLAGS}
LDLIBS += -lfftw3_omp \
	$(if ${DEP_LDLIBS},${DEP_LDLIBS},-lfftw3) \
	$(if ${OMP_LDLIBS},${OMP_LDLIBS},)


# -- Environment ---------------------------------------------------------

# NERSC computer cluster: an example environment [adapt]
ifdef NERSC_HOST

# FFTW library
ifdef FFTW_ROOT
INCLUDES += -I${FFTW_INC}
LDFLAGS += -L${FFTW_DIR}
endif  # FFTW_ROOT

endif  # NERSC_HOST


# -- Parsing -------------------------------------------------------------

CPPFLAGS := $(strip ${CPPFLAGS}) $(strip ${INCLUDES})
CXXFLAGS := $(strip ${CXXFLAGS})
LDFLAGS := $(strip ${LDFLAGS})
LDLIBS := $(strip ${LDLIBS})


# ========================================================================
# Recipes
# ========================================================================

# ------------------------------------------------------------------------
# Building
# ------------------------------------------------------------------------

SRCS := $(wildcard ${DIR_SRC}/*.cpp)
OBJS := $(SRCS:${DIR_SRC}/%.cpp=${DIR_BUILDOBJ}/%.o)
DEPS := $(OBJS:.o=.d)

PROGSRC := ${DIR_PROG}/${PROGNAME}.cpp

PROGOBJ := ${DIR_BUILDOBJ}/${PROGNAME}.o
PROGEXE := ${DIR_BUILDBIN}/${PROGNAME}


# -- Installation --------------------------------------------------------

.PHONY: install uninstall build_

install: install_ build_

install_:
	@echo "Installing FFTW-Age C++ program..."

build_: executable

uninstall: uninstall_ buildclean

uninstall_:
	@echo "Uninstalling FFTW-Age C++ program..."


# -- Components ----------------------------------------------------------

.PHONY: executable objects_

executable: ${PROGEXE}

${PROGEXE}: $(OBJS) ${PROGOBJ}
	@echo "Compiling FFTW-Age C++ program..."
	@if [ ! -d ${DIR_BUILDBIN} ]; then \
	    echo "  making bin subdirectory in build directory..."; \
	    mkdir -p ${DIR_BUILDBIN}; \
	fi
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS) $(LDLIBS)

${PROGOBJ}: ${PROGSRC}
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

$(OBJS): ${DIR_BUILDOBJ}/%.o: ${DIR_SRC}/%.cpp | objects_
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $< -o $@

objects_:
	@echo "Creating FFTW-Age C++ object files..."
	@if [ ! -d ${DIR_BUILDOBJ} ]; then \
	    echo "  making obj subdirectory in build directory..."; \
	    mkdir -p ${DIR_BUILDOBJ}; \
	fi

-include $(DEPS)


# -- Configuration -------------------------------------------------------

.PHONY: checkopts

checkopts:
	@echo "Checking options parsed by Makefile..."
	@echo "  CXX=${CXX}"
	@echo "  INCLUDES=${INCLUDES}"
	@echo "  CPPFLAGS=${CPPFLAGS}"
	@echo "  CXXFLAGS=${CXXFLAGS}"
	@echo "  LDFLAGS=${LDFLAGS}"
	@echo "  LDLIBS=${LDLIBS}"
	@echo "  OMP_CXXFLAGS=${OMP_CXXFLAGS}"
	@echo "  OMP_LDFLAGS=${OMP_LDFLAGS}"
	@echo "  OMP_LDLIBS=${OMP_LDLIBS}"
	@echo "  AR=${AR}"
	@echo "  ARFLAGS=${ARFLAGS}"
	@echo "  RM=${RM}"


# ------------------------------------------------------------------------
# Cleaning
# ------------------------------------------------------------------------

.PHONY: clean buildclean

clean: buildclean

buildclean:
	@echo "Cleaning up FFTW-Age C++ build..."
	@find ${DIR_BUILD} -mindepth 1 -maxdepth 1 \
	    ! -name "bin" ! -name ".gitkeep" -exec rm -r {} +
