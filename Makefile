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

SOURCES_KERNEL=$(wildcard $(DIRECTORY_SYSTEM)/*.cpp) \
			   $(wildcard $(DIRECTORY_ARCH)/*.cpp) \
			   $(wildcard $(DIRECTORY_LIBRARIES)/libc/*.cpp) \
			   $(wildcard $(DIRECTORY_LIBRARIES)/libruntime/*.cpp) \
			   $(wildcard $(DIRECTORY_LIBRARIES)/libsystem/*.cpp) \
			   $(wildcard $(DIRECTORY_ARCH)/*.s)

OBJECTS_KERNEL=$(patsubst $(DIRECTORY_SOURCES)/%, $(DIRECTORY_BUILD)/%.kernel.o, $(SOURCES_KERNEL))

BINARY_KERNEL=$(DIRECTORY_BUILD)/kernel.elf

LOG=@echo
DIRCETORY_GUARD=@mkdir -p $(@D)

CXX=i686-elf-g++
CXXFLAGS=-O3 -fno-rtti -fno-exceptions -std=c++17 -ffreestanding -mno-80387 -mno-mmx -mno-sse -mno-sse2 -nostdlib -nostdinc++ -I$(DIRECTORY_SOURCES) -I$(DIRECTORY_LIBRARIES)
AS=nasm
LD=i686-elf-ld


.PHONY: dump all

dump:
	$(LOG) Kernel binary: $(BINARY_KERNEL)
	$(LOG) Kernel arch directory: $(DIRECTORY_ARCH)
	$(LOG) Kernel system directory: $(DIRECTORY_SYSTEM)
	$(LOG) Kernel sources: $(SOURCES_KERNEL)
	$(LOG) Kernel objects: $(OBJECTS_KERNEL)
	$(LOG) Target: $(BUILD_TARGET)
	$(LOG) Source directory: $(DIRECTORY_SOURCES)
	$(LOG) Build directory: $(DIRECTORY_BUILD)

all: $(BINARY_KERNEL)

run: $(BINARY_KERNEL)
	qemu-system-i386 -serial mon:stdio -kernel $(BINARY_KERNEL)

clean:
	rm -rf $(DIRECTORY_BUILD)

# --- Kernel recipies -------------------------------------------------------- #

$(BINARY_KERNEL): $(OBJECTS_KERNEL)
	$(DIRCETORY_GUARD)
	$(LD) -m elf_i386 -T $(DIRECTORY_ARCH)/system.ld -o $@ $^

$(DIRECTORY_BUILD)/%.cpp.kernel.o: $(DIRECTORY_SOURCES)/%.cpp
	$(DIRCETORY_GUARD)
	$(CXX) $(CXXFLAGS) -fno-stack-protector -c -o $@ $^

$(DIRECTORY_BUILD)/%.s.kernel.o: $(DIRECTORY_SOURCES)/%.s
	$(DIRCETORY_GUARD)
	$(AS) -f elf32 $^ -o $@
