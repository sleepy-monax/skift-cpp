PATH := $(shell toolchain/UseIt.sh):$(PATH)

BUILD_ARCH?=x86
BUILD_CONFIG?=debug

BUILD_TARGET=$(BUILD_CONFIG)-$(BUILD_ARCH)

DIRECTORY_BUILD=build/$(BUILD_TARGET)
DIRECTORY_ISO=$(DIRECTORY_BUILD)/isodir
DIRECTORY_SOURCES=sources

# --- Libraries -------------------------------------------------------------- #

DIRECTORY_LIBRARIES=$(DIRECTORY_SOURCES)/libraries

# --- Kernel ----------------------------------------------------------------- #

DIRECTORY_ARCH=$(DIRECTORY_SOURCES)/arch/$(BUILD_ARCH)
DIRECTORY_SYSTEM=$(DIRECTORY_SOURCES)/system

SOURCES_KERNEL= $(wildcard $(DIRECTORY_ARCH)/*.s) \
				$(wildcard $(DIRECTORY_ARCH)/**/*.s) \
				$(wildcard $(DIRECTORY_ARCH)/*.cpp) \
				$(wildcard $(DIRECTORY_ARCH)/**/*.cpp) \
				$(wildcard $(DIRECTORY_SYSTEM)/*.cpp) \
				$(wildcard $(DIRECTORY_SYSTEM)/**/*.cpp) \
				$(wildcard $(DIRECTORY_LIBRARIES)/libc/*.cpp) \
				$(wildcard $(DIRECTORY_LIBRARIES)/libruntime/*.cpp) \
				$(wildcard $(DIRECTORY_LIBRARIES)/libsystem/*.cpp)

OBJECTS_KERNEL=$(patsubst $(DIRECTORY_SOURCES)/%, $(DIRECTORY_BUILD)/%.kernel.o, $(SOURCES_KERNEL))

BINARY_KERNEL=$(DIRECTORY_BUILD)/kernel.elf

LOG=@echo -e
DIRECTORY_GUARD=@mkdir -p $(@D)

SYSTEM_ISO=$(DIRECTORY_BUILD)/image.iso

# --- Common configs --------------------------------------------------------- #

CXX_INCLUDES=-I. \
			 -I$(DIRECTORY_SOURCES) \
			 -I$(DIRECTORY_LIBRARIES)

CXX_DEFINES=-D__BUILD_TARGET__=\"$(BUILD_TARGET)\" \
			-D__BUILD_GITREF__=\"$(shell git rev-parse --abbrev-ref HEAD || echo unknown)/$(shell git rev-parse --short HEAD || echo unknown)\" \
			-D__BUILD_UNAME__=\""$(shell uname -s -o -m -r)"\"

CXX=i686-elf-g++
CXXFLAGS=-std=c++17 \
		 -MD \
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
		  -nostdinc++

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

all: $(SYSTEM_ISO)

run: $(SYSTEM_ISO)
	qemu-system-i386 -serial mon:stdio -cdrom $(SYSTEM_ISO)

run-headless: $(SYSTEM_ISO)
	qemu-system-i386 -nographic -serial mon:stdio -cdrom $(SYSTEM_ISO)

clean:
	rm -rf $(DIRECTORY_BUILD)
	rm -rf $(DIRECTORY_ISO)
	rm -f $(SYSTEM_ISO)


$(SYSTEM_ISO): $(BINARY_KERNEL)
	@mkdir -p $(DIRECTORY_ISO)/boot/grub/
	@cp grub.cfg $(DIRECTORY_ISO)/boot/grub/
	@cp $(BINARY_KERNEL) $(DIRECTORY_ISO)/boot/
	grub-mkrescue -o $(SYSTEM_ISO) $(DIRECTORY_ISO) || \
	grub2-mkrescue -o $(SYSTEM_ISO) $(DIRECTORY_ISO)

# --- Kernel recipies -------------------------------------------------------- #

$(BINARY_KERNEL): $(OBJECTS_KERNEL)
	$(DIRECTORY_GUARD)
	$(KLD) $(KLDFLAGS) -o $@ $^

$(DIRECTORY_BUILD)/%.cpp.kernel.o: $(DIRECTORY_SOURCES)/%.cpp
	$(DIRECTORY_GUARD)
	$(KCXX) $(CXXFLAGS) $(KCXXFLAGS) -c -o $@ $<

$(DIRECTORY_BUILD)/%.s.kernel.o: $(DIRECTORY_SOURCES)/%.s
	$(DIRECTORY_GUARD)
	$(AS) $(ASFLAGS) -o $@ $<

-include $(OBJECTS_KERNEL:.o=.d)
