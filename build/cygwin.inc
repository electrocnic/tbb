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

####### Detections and Commands ###############################################

# minversion returns yes if $(1) is at least $(2) as version number strings, no otherwise
# $(call minversion,10.1,4.8) returns yes
# $(call minversion,3.1,4.8) returns no
# make sure parameters are passed without leading or trailing space
minversion=$(shell [[ $$(printf "$(1)\n$(2)" | sort -V | head -1) == $(2) ]] && echo yes || echo no)

# implib_from_dll transforms dll target to import library name
# for example cygtbb_debug.dll gives libtbb_debug.dll.a
implib_from_dll=$(if $(filter $(DLL_PREFIX)%,$@),$(patsubst $(DLL_PREFIX)%.$(DLL),lib%.$(LIBEXT),$@),$(patsubst %.$(DLL),%.$(LIBEXT),$@))

ifndef arch
    uname_m:=$(shell uname -m)
    ifeq ($(uname_m),i686)
        export arch:=ia32
    endif
    ifeq ($(uname_m),x86_64)
        export arch:=intel64
    endif
endif

ifndef runtime
    export gcc_version:=$(shell gcc -dumpfullversion -dumpversion)
    os_version:=$(shell uname -r)
    os_kernel_version:=$(shell uname -r | sed -e 's/-.*$$//')
    export runtime:=cc$(gcc_version)_kernel$(os_kernel_version)
endif

native_compiler := gcc
export compiler ?= gcc
debugger ?= gdb

CMD=sh -c
CWD=$(CURDIR)
CP=cp
RM?=rm -f
RD?=rmdir
MD?=mkdir -p
NUL= /dev/null
SLASH=/
MAKE_VERSIONS=sh $(tbb_root)/build/version_info_cygwin.sh $(VERSION_FLAGS) >version_string.ver
MAKE_TBBVARS=sh $(tbb_root)/build/generate_tbbvars.sh

ifdef LD_LIBRARY_PATH
    export LD_LIBRARY_PATH := .:$(LD_LIBRARY_PATH)
else
    export LD_LIBRARY_PATH := .
endif

####### Build settings ########################################################

OBJ = o
DLL = dll
MALLOC_DLL?=$(DLL)
LIBEXT = dll.a
DLL_PREFIX=cyg

ifeq (,$(def_prefix))
    ifeq (64,$(findstring 64,$(arch)))
        def_prefix = cyg64
    else
        def_prefix = cyg32
    endif
endif

TBB.DLL = $(DLL_PREFIX)tbb$(CPF_SUFFIX)$(DEBUG_SUFFIX).$(DLL)
TBB.LIB = libtbb$(CPF_SUFFIX)$(DEBUG_SUFFIX).$(LIBEXT)
LINK_TBB.LIB = $(TBB.LIB)

MALLOC.DLL = $(DLL_PREFIX)tbbmalloc$(DEBUG_SUFFIX).$(DLL)
MALLOC.LIB = libtbbmalloc$(DEBUG_SUFFIX).$(LIBEXT)
LINK_MALLOC.LIB = $(MALLOC.LIB)

MALLOCPROXY.DLL = $(DLL_PREFIX)tbbmalloc_proxy$(DEBUG_SUFFIX).$(DLL)
MALLOCPROXY.LIB = libtbbmalloc_proxy$(DEBUG_SUFFIX).$(LIBEXT)
LINK_MALLOCPROXY.LIB = $(MALLOCPROXY.LIB)

RML.DLL = $(DLL_PREFIX)irml$(DEBUG_SUFFIX).$(DLL)
RML.LIB = libirml$(DEBUG_SUFFIX).$(LIBEXT)

TEST_LAUNCHER=sh $(tbb_root)/build/test_launcher.sh $(largs)

