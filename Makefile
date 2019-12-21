PATH := $(shell toolchain/UseIt.sh):$(PATH)

BUILD_ARCH?=x86
BUILD_CONFIG?=debug
BUILD_SYSTEM?=skift

BUILD_TARGET=$(BUILD_CONFIG)-$(BUILD_ARCH)-$(BUILD_SYSTEM)
BUILD_GITREF=$(shell git rev-parse --abbrev-ref HEAD || echo unknown)/$(shell git rev-parse --short HEAD || echo unknown)
BUILD_UNAME=$(shell uname -s -o -m -r)
BUILD_DIRECTORY=build/$(BUILD_TARGET)

BOOT_DIRECTORY=$(BUILD_DIRECTORY)/isodir

SOURCES_DIRECTORY=sources

LOG=@echo -e
DIRECTORY_GUARD=@mkdir -p $(@D)

SYSTEM_IMAGE=$(BUILD_DIRECTORY)/image.iso

# --- Common configs --------------------------------------------------------- #

CXX_INCLUDES=-I. \
			 -I$(SOURCES_DIRECTORY) \
			 -I$(LIBRARIES_DIRECTORY)

CXX_DEFINES=-D__BUILD_TARGET__=\""$(BUILD_TARGET)"\" \
			-D__BUILD_GITREF__=\""$(BUILD_GITREF)"\" \
			-D__BUILD_UNAME__=\""$(BUILD_UNAME)"\"

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

# --- Common configs --------------------------------------------------------- #

COMMON_CXXFLAGS=-std=c++17 -O3 -Wall -Wextra -Werror -fsanitize=undefined -fno-rtti -fno-exceptions
COMMON_CFLAGS  =-std=c11   -O3 -Wall -Wextra -Werror -fsanitize=undefined

# --- User space configs ----------------------------------------------------- #

USERSPACE_CXX=$(CXX)
USERSPACE_CXXFLAGS=-ffreestanding \
				-fno-stack-protector \
				-nostdlib \
				-nostdinc++

USERSPACE_LD=$(LD)
USERSPACE_LDFLAGS=

USERSPACE_AS=$(AS)
USERSPACE_ASFLAGS=

USERSPACE_AR=$(AR)
USERSPACE_ARFLAGS=rcs

# --- Libraries -------------------------------------------------------------- #

LIBRARIES=libc \
		  libgraphic \
		  libmath \
		  libruntime \
		  libsystem \
		  libtest \
		  libwidget 

LIBRARIES_DIRECTORY=$(SOURCES_DIRECTORY)/libraries

LIBRARIES_SOURCES=$(shell find $(LIBRARIES_DIRECTORY) -name "*.cpp")
LIBRARIES_OBJECTS=$(patsubst $(SOURCES_DIRECTORY)/%, $(BUILD_DIRECTORY)/%.o, $(LIBRARIES_SOURCES))
LIBRARIES_ARCHIVES=$(patsubst %, $(BUILD_DIRECTORY)/libraries/%.a, $(LIBRARIES))

# --- Kernel ----------------------------------------------------------------- #

ARCH_DIRECTORY=$(SOURCES_DIRECTORY)/arch/$(BUILD_ARCH)

SYSTEM_DIRECTORY=$(SOURCES_DIRECTORY)/system

KERNEL_SOURCES=$(wildcard $(ARCH_DIRECTORY)/*.s) \
			   $(wildcard $(ARCH_DIRECTORY)/**/*.s) \
			   $(wildcard $(ARCH_DIRECTORY)/*.cpp) \
			   $(wildcard $(ARCH_DIRECTORY)/**/*.cpp) \
			   $(wildcard $(SYSTEM_DIRECTORY)/*.cpp) \
			   $(wildcard $(SYSTEM_DIRECTORY)/**/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libc/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libruntime/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libsystem/*.cpp)

KERNEL_OBJECTS=$(patsubst $(SOURCES_DIRECTORY)/%, $(BUILD_DIRECTORY)/%.kernel.o, $(KERNEL_SOURCES))

KERNEL_BINARY=$(BUILD_DIRECTORY)/kernel.elf

KERNEL_CXX=$(CXX)
KERNEL_CXXFLAGS=-ffreestanding \
				-fno-stack-protector \
				-nostdlib \
				-nostdinc++

KERNEL_LD=$(LD)
KERNEL_LDFLAGS=-m elf_i386 -T $(ARCH_DIRECTORY)/system.ld

-include $(ARCH_DIRECTORY)/config.mk

# --- Phony rules ------------------------------------------------------------ #

.PHONY: dump all run run-headless clean

dump:
	$(LOG) Target: $(BUILD_TARGET)
	$(LOG) Source directory: $(SOURCES_DIRECTORY)
	$(LOG) Build directory: $(BUILD_DIRECTORY)
	$(LOG) CXX: $(CXX)
	$(LOG) CXXFLAGS: $(CXXFLAGS)
	$(LOG)
	$(LOG) Libraries:
	$(LOG) --------------------------------------------------------------------
	$(LOG) Libraries sources:"\n"$(LIBRARIES_SOURCES)
	$(LOG) Libraries objects:"\n"$(LIBRARIES_OBJECTS)
	$(LOG) Libraries archives: $(LIBRARIES_ARCHIVES)
	$(LOG)
	$(LOG) Kernel:
	$(LOG) --------------------------------------------------------------------
	$(LOG) Kernel binary: $(KERNEL_BINARY)
	$(LOG) Kernel arch directory: $(ARCH_DIRECTORY)
	$(LOG) Kernel system directory: $(SYSTEM_DIRECTORY)
	$(LOG) Kernel sources:"\n"$(KERNEL_SOURCES)
	$(LOG) Kernel objects:"\n"$(KERNEL_OBJECTS)

all: $(SYSTEM_IMAGE)

run: $(SYSTEM_IMAGE)
	qemu-system-i386 -serial mon:stdio -cdrom $(SYSTEM_IMAGE)

run-headless: $(SYSTEM_IMAGE)
	qemu-system-i386 -nographic -serial mon:stdio -cdrom $(SYSTEM_IMAGE)

clean:
	rm -rf $(BUILD_DIRECTORY)
	rm -rf $(BOOT_DIRECTORY)
	rm -f $(SYSTEM_IMAGE)


$(SYSTEM_IMAGE): $(KERNEL_BINARY) $(LIBRARIES_ARCHIVES) grub.cfg
	@mkdir -p $(BOOT_DIRECTORY)/boot/grub/
	@cp grub.cfg $(BOOT_DIRECTORY)/boot/grub/
	@cp $(KERNEL_BINARY) $(BOOT_DIRECTORY)/boot/
	grub-mkrescue -o $(SYSTEM_IMAGE) $(BOOT_DIRECTORY) || \
	grub2-mkrescue -o $(SYSTEM_IMAGE) $(BOOT_DIRECTORY)

# --- Libraries recipies ----------------------------------------------------- #

$(BUILD_DIRECTORY)/libraries/libc.a: $(filter $(BUILD_DIRECTORY)/libraries/libc/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libgraphic.a: $(filter $(BUILD_DIRECTORY)/libraries/libgraphic/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libmath.a: $(filter $(BUILD_DIRECTORY)/libraries/libmath/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libruntime.a: $(filter $(BUILD_DIRECTORY)/libraries/libruntime/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libsystem.a: $(filter $(BUILD_DIRECTORY)/libraries/libsystem/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libtest.a: $(filter $(BUILD_DIRECTORY)/libraries/libtest/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libwidget.a: $(filter $(BUILD_DIRECTORY)/libraries/libwidget/%.o, $(LIBRARIES_OBJECTS))

$(LIBRARIES_ARCHIVES): 
	$(LOG) [USER] [AR] $@ 
	$(DIRECTORY_GUARD)
	@$(USERSPACE_AR) $(USERSPACE_ARFLAGS) $@ $^

$(BUILD_DIRECTORY)/%.cpp.o: $(SOURCES_DIRECTORY)/%.cpp
	$(LOG) [USER] [CXX] $<
	$(DIRECTORY_GUARD)
	@$(USERSPACE_CXX) $(CXXFLAGS) $(USERSPACE_CXXFLAGS) -c -o $@ $<

# --- Kernel recipies -------------------------------------------------------- #

$(KERNEL_BINARY): $(KERNEL_OBJECTS)
	$(LOG) [KERNEL] [LD] $@ 
	$(DIRECTORY_GUARD)
	@$(KERNEL_LD) $(KERNEL_LDFLAGS) -o $@ $^

$(BUILD_DIRECTORY)/%.cpp.kernel.o: $(SOURCES_DIRECTORY)/%.cpp
	$(LOG) [KERNEL] [CXX] $<
	$(DIRECTORY_GUARD)
	@$(KERNEL_CXX) $(CXXFLAGS) $(KERNEL_CXXFLAGS) -c -o $@ $<

$(BUILD_DIRECTORY)/%.s.kernel.o: $(SOURCES_DIRECTORY)/%.s
	$(LOG) [KERNEL] [AS] $<
	$(DIRECTORY_GUARD)
	@$(AS) $(ASFLAGS) -o $@ $<

-include $(KERNEL_OBJECTS:.o=.d)
