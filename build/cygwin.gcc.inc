# Copyright (c) 2005-2019 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

#------------------------------------------------------------------------------
# Compiler-specific variables
#------------------------------------------------------------------------------

CPLUS ?= g++
COMPILE_ONLY = -c -MMD
PREPROC_ONLY = -E -x c++
INCLUDE_KEY = -I
DEFINE_KEY = -D
OUTPUT_KEY = -o #
OUTPUTOBJ_KEY = -o #
PIC_KEY =
WARNING_AS_ERROR_KEY = -Werror
WARNING_KEY = -Wall
TEST_WARNING_KEY = -Wextra -Wshadow -Wcast-qual -Woverloaded-virtual -Wnon-virtual-dtor -Wno-uninitialized
WARNING_SUPPRESS = -Wno-parentheses -Wno-uninitialized -Wno-non-virtual-dtor
DYLIB_KEY = -shared
LIBDL = -ldl
LIBS += -lrt
BIGOBJ_KEY = -Wa,-mbig-obj

#------------------------------------------------------------------------------
# End of compiler-specific variables
#------------------------------------------------------------------------------
# Command lines
#------------------------------------------------------------------------------

DLL_IMPORTLIB_OPT=-Wl,--out-implib=$(implib_from_dll)
DLL_LINK_FLAGS = -Wl,--enable-auto-import -Wl,--export-all-symbols $(DLL_IMPORTLIB_OPT)
LIB_LINK_FLAGS = -pthread $(DLL_LINK_FLAGS) $(DYLIB_KEY)

# gcc 4.8 and later support RTM intrinsics, but require command line switch to enable them
ifeq ($(call minversion,$(gcc_version),4.8),yes)
    RTM_KEY = -mrtm
endif

# gcc 6.0 and later have -flifetime-dse option that controls
# elimination of stores done outside the object lifetime
ifeq ($(call minversion,$(gcc_version),6.0),yes)
    # keep pre-contruction stores for zero initialization
    DSE_KEY = -flifetime-dse=1
endif

ifeq ($(cfg), release)
        CPLUS_FLAGS = -g -O2
endif
ifeq ($(cfg), debug)
        CPLUS_FLAGS = -g -O0 -DTBB_USE_DEBUG
endif

CPLUS_FLAGS += -DUSE_PTHREAD -pthread

# for setenv
CPLUS_FLAGS += -D_POSIX_C_SOURCE=200112L

CONLY = gcc
debugger = gdb
C_FLAGS = $(CPLUS_FLAGS)

ifeq (intel64,$(arch))
    CPLUS_FLAGS += -m64 $(RTM_KEY)
    LIB_LINK_FLAGS += -m64
endif

ifeq (ia32,$(arch))
    CPLUS_FLAGS += -m32 -march=i686 $(RTM_KEY)
    LIB_LINK_FLAGS += -m32
endif

# For examples
export UNIXMODE = 1

#------------------------------------------------------------------------------
# End of command lines
#------------------------------------------------------------------------------
# Setting assembler data
#------------------------------------------------------------------------------

ASM=
ASM_FLAGS=
TBB_ASM.OBJ=
ASSEMBLY_SOURCE=$(arch)-gas

#------------------------------------------------------------------------------
# End of setting assembler data
#------------------------------------------------------------------------------
# Setting tbbmalloc data
#------------------------------------------------------------------------------

M_CPLUS_FLAGS = $(CPLUS_FLAGS) -fno-rtti -fno-exceptions

#------------------------------------------------------------------------------
# End of setting tbbmalloc data
#------------------------------------------------------------------------------
