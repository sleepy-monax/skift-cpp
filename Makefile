PATH := $(shell toolchain/UseIt.sh):$(PATH)

BUILD_ARCH?=x86
BUILD_CONFIG?=debug
BUILD_SYSTEM?=skift

BUILD_TARGET=$(BUILD_CONFIG)-$(BUILD_ARCH)-$(BUILD_SYSTEM)
BUILD_GITREF=$(shell git rev-parse --abbrev-ref HEAD || echo unknown)/$(shell git rev-parse --short HEAD || echo unknown)
BUILD_UNAME=$(shell uname -s -o -m -r)
BUILD_DIRECTORY=build/$(BUILD_TARGET)

IMAGE_DIRECTORY=$(BUILD_DIRECTORY)/image

SOURCES_DIRECTORY=sources

LOG=@echo
DIRECTORY_GUARD=@mkdir -p $(@D)

SYSTEM_IMAGE=$(BUILD_DIRECTORY)/image.iso

# --- Common configs --------------------------------------------------------- #

CINCLUDES=-I. \
		  -I$(SOURCES_DIRECTORY) \
		  -I$(LIBRARIES_DIRECTORY)

CDEFINES=-D__BUILD_TARGET__=\""$(BUILD_TARGET)"\" \
		 -D__BUILD_GITREF__=\""$(BUILD_GITREF)"\" \
		 -D__BUILD_UNAME__=\""$(BUILD_UNAME)"\"


# --- Common configs --------------------------------------------------------- #

COMMON_CXX=i686-elf-g++
COMMON_CXXFLAGS=-std=c++17 -MD -O3 -Wall -Wextra -Werror -fsanitize=undefined $(CINCLUDES) $(CDEFINES) -fno-rtti -fno-exceptions

COMMON_CC=i686-elf-gcc
COMMON_CFLAGS  =-std=c11   -MD -O3 -Wall -Wextra -Werror -fsanitize=undefined $(CINCLUDES) $(CDEFINES)

COMMON_LD=i686-elf-ld

COMMON_AS=nasm
COMMON_ASFLAGS=-f elf32

COMMON_AR=i686-elf-ar


# --- User space configs ----------------------------------------------------- #

USERSPACE_CXXFLAGS=-ffreestanding \
				   -fno-stack-protector \
				   -nostdlib \
				   -nostdinc++

USERSPACE_LDFLAGS=

USERSPACE_ASFLAGS=

USERSPACE_ARFLAGS=rcs

# --- Libraries -------------------------------------------------------------- #

LIBRARIES=libc \
		  libgraphic \
		  libmath \
		  libruntime \
		  libsystem \
		  libterminal \
		  libtest \
		  libwidget

LIBRARIES_DIRECTORY=$(SOURCES_DIRECTORY)/libraries

LIBRARIES_SOURCES=$(shell find $(LIBRARIES_DIRECTORY) -name "*.cpp") \
				  $(wildcard $(SOURCES_DIRECTORY)/target/$(BUILD_SYSTEM)/*.cpp) \
				  $(wildcard $(SOURCES_DIRECTORY)/target/$(BUILD_SYSTEM)/$(BUILD_ARCH)/*.cpp) \
				  $(wildcard $(SOURCES_DIRECTORY)/target/$(BUILD_SYSTEM)/$(BUILD_ARCH)/*.s)

LIBRARIES_OBJECTS=$(patsubst $(SOURCES_DIRECTORY)/%, $(BUILD_DIRECTORY)/%.o, $(LIBRARIES_SOURCES))
LIBRARIES_ARCHIVES=$(patsubst %, $(BUILD_DIRECTORY)/libraries/%.a, $(LIBRARIES))

# --- Kernel ----------------------------------------------------------------- #

ARCH_DIRECTORY=$(SOURCES_DIRECTORY)/arch/$(BUILD_ARCH)
SYSTEM_DIRECTORY=$(SOURCES_DIRECTORY)/system
TARGET_DIRECTORY=$(SOURCES_DIRECTORY)/targets

KERNEL_SOURCES=$(wildcard $(ARCH_DIRECTORY)/*.s) \
			   $(wildcard $(ARCH_DIRECTORY)/**/*.s) \
			   $(wildcard $(ARCH_DIRECTORY)/*.cpp) \
			   $(wildcard $(ARCH_DIRECTORY)/**/*.cpp) \
			   $(wildcard $(SYSTEM_DIRECTORY)/*.cpp) \
			   $(wildcard $(SYSTEM_DIRECTORY)/**/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libc/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libruntime/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libsystem/*.cpp) \
			   $(wildcard $(LIBRARIES_DIRECTORY)/libterminal/*.cpp)

KERNEL_OBJECTS=$(patsubst $(SOURCES_DIRECTORY)/%, $(BUILD_DIRECTORY)/%.k.o, $(KERNEL_SOURCES))

KERNEL_BINARY=$(BUILD_DIRECTORY)/kernel.elf

KERNEL_CXXFLAGS=-ffreestanding \
				-fno-stack-protector \
				-nostdlib \
				-nostdinc++

KERNEL_LDFLAGS=-m elf_i386 -T $(ARCH_DIRECTORY)/system.ld

KERNEL_ASFLAGS=-f elf32


-include $(ARCH_DIRECTORY)/config.mk \
		 $(TARGET_DIRECTORY)/$(BUILD_SYSTEM)/config.mk \
		 $(TARGET_DIRECTORY)/$(BUILD_SYSTEM)/$(BUILD_ARCH)/config.mk

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

userspace: $(LIBRARIES_ARCHIVES)

run: $(SYSTEM_IMAGE)
	qemu-system-i386 -serial mon:stdio -cdrom $(SYSTEM_IMAGE)

run-headless: $(SYSTEM_IMAGE)
	qemu-system-i386 -nographic -serial mon:stdio -cdrom $(SYSTEM_IMAGE)

clean:
	rm -rf $(BUILD_DIRECTORY)
	rm -rf $(IMAGE_DIRECTORY)
	rm -f $(SYSTEM_IMAGE)


$(SYSTEM_IMAGE): $(KERNEL_BINARY) $(LIBRARIES_ARCHIVES) grub.cfg
	@mkdir -p $(IMAGE_DIRECTORY)/boot/grub/
	@cp grub.cfg $(IMAGE_DIRECTORY)/boot/grub/
	@cp $(KERNEL_BINARY) $(IMAGE_DIRECTORY)/boot/
	grub-mkrescue -o $(SYSTEM_IMAGE) $(IMAGE_DIRECTORY) || \
	grub2-mkrescue -o $(SYSTEM_IMAGE) $(IMAGE_DIRECTORY)

# --- Libraries recipies ----------------------------------------------------- #

$(BUILD_DIRECTORY)/libraries/libc.a: $(filter $(BUILD_DIRECTORY)/libraries/libc/%.o, $(LIBRARIES_OBJECTS)) \
								     $(filter $(BUILD_DIRECTORY)/libraries/targets/%.o, $(LIBRARIES_OBJECTS))

$(BUILD_DIRECTORY)/libraries/libgraphic.a: $(filter $(BUILD_DIRECTORY)/libraries/libgraphic/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libmath.a: $(filter $(BUILD_DIRECTORY)/libraries/libmath/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libruntime.a: $(filter $(BUILD_DIRECTORY)/libraries/libruntime/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libsystem.a: $(filter $(BUILD_DIRECTORY)/libraries/libsystem/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libterminal.a: $(filter $(BUILD_DIRECTORY)/libraries/libterminal/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libtest.a: $(filter $(BUILD_DIRECTORY)/libraries/libtest/%.o, $(LIBRARIES_OBJECTS))
$(BUILD_DIRECTORY)/libraries/libwidget.a: $(filter $(BUILD_DIRECTORY)/libraries/libwidget/%.o, $(LIBRARIES_OBJECTS))

$(LIBRARIES_ARCHIVES):
	$(LOG) [$(BUILD_SYSTEM)] [USER] [AR] $@
	$(DIRECTORY_GUARD)
	@$(COMMON_AR) $(USERSPACE_ARFLAGS) $@ $^

$(BUILD_DIRECTORY)/%.cpp.o: $(SOURCES_DIRECTORY)/%.cpp
	$(LOG) [$(BUILD_SYSTEM)] [USER] [CXX] $<
	$(DIRECTORY_GUARD)
	@$(COMMON_CXX) $(COMMON_CXXFLAGS) $(USERSPACE_CXXFLAGS) -c -o $@ $<

# --- Kernel recipies -------------------------------------------------------- #

$(KERNEL_BINARY): $(KERNEL_OBJECTS)
	$(LOG) [KERNEL] [LD] $@
	$(DIRECTORY_GUARD)
	@$(COMMON_LD) $(KERNEL_LDFLAGS) -o $@ $^

$(BUILD_DIRECTORY)/%.cpp.k.o: $(SOURCES_DIRECTORY)/%.cpp
	$(LOG) [KERNEL] [CXX] $<
	$(DIRECTORY_GUARD)
	@$(COMMON_CXX) $(COMMON_CXXFLAGS) $(KERNEL_CXXFLAGS) -c -o $@ $<

$(BUILD_DIRECTORY)/%.s.k.o: $(SOURCES_DIRECTORY)/%.s
	$(LOG) [KERNEL] [AS] $<
	$(DIRECTORY_GUARD)
	@$(COMMON_AS) $(KERNEL_ASFLAGS) -o $@ $<

-include $(KERNEL_OBJECTS:.o=.d)
-include $(LIBRARIES_OBJECTS:.o=.d)
