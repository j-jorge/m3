CFLAGS := -Wall -Wno-sign-compare -fdiagnostics-color=always
CXXFLAGS := $(CFLAGS) --std=c++11
LDFLAGS := 

BUILD_TYPE_TAG := build/linux/last_build_type

BUILD ?= $(shell [ -f $(BUILD_TYPE_TAG) ] && cat $(BUILD_TYPE_TAG))

CMAKE_BUILD_TYPE := 

ifeq ($(BUILD),release)
  CMAKE_BUILD_TYPE := release
else ifeq ($(BUILD),profile)
  CMAKE_BUILD_TYPE := release
  CXXFLAGS += -pg
else
  CMAKE_BUILD_TYPE := debug
endif

BUILD_DIR := build/linux/$(CMAKE_BUILD_TYPE)
CMAKE_ARGS := -DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) \
	-DBEAR_ROOT_DIRECTORY=$(shell pwd)/../bear

all: test

install: all
	cd $(BUILD_DIR) && $(MAKE) install

uninstall:
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && $(MAKE) uninstall

test: target
	cd $(BUILD_DIR) && GTEST_COLOR=1 ctest --verbose

target: .cmake
	cd $(BUILD_DIR) \
	  && /usr/bin/time -f '\n-- Build time: %U seconds --\n' $(MAKE)

.cmake: .build_marker
	cd $(BUILD_DIR) \
	  && CFLAGS="$(CFLAGS)" CXXFLAGS="$(CXXFLAGS)" LDFLAGS="$(LDFLAGS)" \
	     cmake ../../../cmake $(CMAKE_ARGS)

.build_marker: $(BUILD_DIR)
	echo $(BUILD) > $(BUILD_TYPE_TAG)

$(BUILD_DIR):
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

clean:
	rm -fr $(BUILD_DIR)
