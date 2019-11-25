PATH := $(shell toolchain/UseIt.sh):$(PATH)

BUILD_ARCH?=x86
BUILD_CONFIG?=debug

BUILD_TARGET=$(BUILD_CONFIG)-$(BUILD_ARCH)

DIRECTORY_BUILD=build/$(BUILD_TARGET)
DIRECTORY_SOURCES=sources

# --- Libraries -------------------------------------------------------------- #

DIRECTORY_LIBRARIES=$(DIRECTORY_SOURCES)/libraries

# --- Kernel ----------------------------------------------------------------- #

DIRECTORY_ARCH=$(DIRECTORY_SOURCES)/arch/$(BUILD_ARCH)
DIRECTORY_SYSTEM=$(DIRECTORY_SOURCES)/system

SOURCES_KERNEL= $(wildcard $(DIRECTORY_ARCH)/*.s) \
				$(wildcard $(DIRECTORY_SYSTEM)/*.cpp) \
				$(wildcard $(DIRECTORY_SYSTEM)/**/*.cpp) \
				$(wildcard $(DIRECTORY_ARCH)/*.cpp) \
				$(wildcard $(DIRECTORY_LIBRARIES)/libc/*.cpp) \
				$(wildcard $(DIRECTORY_LIBRARIES)/libruntime/*.cpp) \
				$(wildcard $(DIRECTORY_LIBRARIES)/libsystem/*.cpp)

OBJECTS_KERNEL=$(patsubst $(DIRECTORY_SOURCES)/%, $(DIRECTORY_BUILD)/%.kernel.o, $(SOURCES_KERNEL))

BINARY_KERNEL=$(DIRECTORY_BUILD)/kernel.elf

LOG=@echo -e
DIRECTORY_GUARD=@mkdir -p $(@D)

# --- Common configs --------------------------------------------------------- #

CXX_INCLUDES=-I$(DIRECTORY_SOURCES) \
			 -I$(DIRECTORY_LIBRARIES)

CXX_DEFINES=-D__BUILD_TARGET__=\"$(BUILD_TARGET)\" \
			-D__BUILD_UNAME__=\""$(shell uname -a)"\"

CXX=i686-elf-g++
CXXFLAGS=-std=c++17 \
		 -O3 \
		 -Wall \
		 -Wextra \
		 -Werror \
		 -fno-rtti \
		 -fno-exceptions \
		 $(CXX_INCLUDES) \
		 $(CXX_DEFINES)

AS=nasm
ASFLAGS=-f elf32

LD=i686-elf-ld

# --- Kernel configs --------------------------------------------------------- #

KCXX=$(CXX)
KCXXFLAGS=-ffreestanding \
		  -fsanitize=undefined \
		  -fno-stack-protector \
		  -nostdlib \
		  -nostdinc++ \
		  -mno-80387 \
		  -mno-mmx \
		  -mno-sse \
		  -mno-sse2

KLD=$(LD)
KLDFLAGS=-m elf_i386 -T $(DIRECTORY_ARCH)/system.ld

# --- User space configs ----------------------------------------------------- #

UCXX=$(CXX)
UCXXFLAGS=

-include $(DIRECTORY_ARCH)/config.mk

# --- Phony rules ------------------------------------------------------------ #

.PHONY: dump all

dump:
	$(LOG) Target: $(BUILD_TARGET)
	$(LOG) Source directory: $(DIRECTORY_SOURCES)
	$(LOG) Build directory: $(DIRECTORY_BUILD)
	$(LOG) CXX: $(CXX)
	$(LOG) CXXFLAGS: $(CXXFLAGS)
	$(LOG)
	$(LOG) Kernel:
	$(LOG) --------------------------------------------------------------------
	$(LOG) Kernel binary: $(BINARY_KERNEL)
	$(LOG) Kernel arch directory: $(DIRECTORY_ARCH)
	$(LOG) Kernel system directory: $(DIRECTORY_SYSTEM)
	$(LOG) Kernel sources:"\n"$(SOURCES_KERNEL)
	$(LOG) Kernel objects:"\n"$(OBJECTS_KERNEL)

all: $(BINARY_KERNEL)

run: $(BINARY_KERNEL)
	qemu-system-i386 -serial mon:stdio -kernel $(BINARY_KERNEL)

clean:
	rm -rf $(DIRECTORY_BUILD)

# --- Kernel recipies -------------------------------------------------------- #

$(BINARY_KERNEL): $(OBJECTS_KERNEL)
	$(DIRECTORY_GUARD)
	$(KLD) $(KLDFLAGS) -o $@ $^

$(DIRECTORY_BUILD)/%.cpp.kernel.o: $(DIRECTORY_SOURCES)/%.cpp
	$(DIRECTORY_GUARD)
	$(KCXX) $(CXXFLAGS) $(KCXXFLAGS) -c -o $@ $^

$(DIRECTORY_BUILD)/%.s.kernel.o: $(DIRECTORY_SOURCES)/%.s
	$(DIRECTORY_GUARD)
	$(AS) $(ASFLAGS) -o $@ $^
