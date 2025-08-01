# =====================================================
#
#      Makefile
#      Uinxed compile script
#
#      2024/6/23 By Rainy101112
#      Based on GPL-3.0 open source agreement
#      Copyright © 2020 ViudiraTech, based on the GPLv3 agreement.
#
# =====================================================

ifneq ($(wildcard .config),)
  include .config
else ifneq ($(wildcard .config-default),)
  include .config-default
else
  $(error No configuration file (.config or .config-default) found)
endif

ifeq ($(VERBOSE), 1)
  V=
  Q=
else
  V=@printf "\033[1;32m[Build]\033[0m $@ ...\n";
  Q=@
endif

ifeq ($(CONFIG_KERNEL_LOG), y)
  C_CONFIG += -DKERNEL_LOG
endif

ifneq ($(CONFIG_SERIAL_BAUD_RATE),)
  C_CONFIG += -DSERIAL_BAUD_RATE=$(CONFIG_SERIAL_BAUD_RATE)
endif

ifneq ($(CONFIG_SERIAL_DATA_BITS),)
  C_CONFIG += -DSERIAL_DATA_BITS=$(CONFIG_SERIAL_DATA_BITS)
endif

ifneq ($(CONFIG_SERIAL_STOP_BITS),)
  C_CONFIG += -DSERIAL_STOP_BITS=$(CONFIG_SERIAL_STOP_BITS)
endif

C_SOURCES      := $(shell find * -name "*.c")
S_SOURCES      := $(shell find * -name "*.s")
HEADERS        := $(shell find * -name "*.h")
OBJS           := $(C_SOURCES:%.c=%.o) $(S_SOURCES:%.s=%.o)
DEPS           := $(OBJS:%.o=%.d)
LIBS           := $(wildcard libs/lib*.a)
PWD            := $(shell pwd)

QEMU           := qemu-system-x86_64
QEMU_FLAGS     := -machine q35 -bios assets/ovmf-code.fd

CHECKS         := -quiet -checks=-*,clang-analyzer-*,bugprone-*,cert-*,misc-*,performance-*,portability-*,-misc-include-cleaner,-clang-analyzer-security.insecureAPI.*

# If you want to get more details of `dump_stack`, you need to replace `-O3` with `-O0` or '-Os'.
# `-fno-optimize-sibling-calls` is for `dump_stack` to work properly.
C_FLAGS        := -Wall -Wextra -O3 -g3 -m64 -fpie -ffreestanding -fno-optimize-sibling-calls -fno-stack-protector -fno-omit-frame-pointer \
                  -mno-red-zone -mno-80387 -mno-mmx -mno-sse -mno-sse2 -msoft-float -I include -MMD
LD_FLAGS       := -nostdlib -pie -T assets/linker.ld -m elf_x86_64
AS_FLAGS       := -g --64

all: info Uinxed-x64.iso

%.o: %.c
	$(V)$(CC) $(C_FLAGS) $(C_CONFIG) -c -o $@ $<

%.o: %.s
	$(V)$(AS) $(AS_FLAGS) -o $@ $<

%.fmt: %
	$(Q)printf "\033[1;32m[Format]\033[0m $< ...\n"
	$(Q)clang-format -i $<

%.tidy: %
	$(Q)printf "\033[1;32m[Checks]\033[0m $< ...\n"
	$(Q)clang-tidy $< $(CHECKS) -- $(C_FLAGS)

info:
	$(Q)printf "Uinxed-Kernel Compile Script.\n"
	$(Q)printf "Copyright 2020 ViudiraTech. Based on the GPLv3 license.\n"
	$(Q)printf "Based on the GPL-3.0 open source license.\n"
	$(Q)echo

UxImage: $(OBJS) $(LIBS)
	$(V)$(LD) $(LD_FLAGS) -o $@ $^

Uinxed-x64.iso: UxImage
	$(Q)echo
	$(Q)printf "\033[1;32m[ISO]\033[0m Packing ISO file...\n"
	$(Q)cp -a assets/Limine iso
	$(Q)cp $< iso/EFI/Boot
	$(Q)xorriso -as mkisofs -R -r -J -b Limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 \
                -boot-info-table -hfsplus -apm-block-size 2048 -efi-boot-part --efi-boot-image --protective-msdos-label \
                --efi-boot Limine/limine-uefi-cd.bin -o Uinxed-x64.iso iso

	$(Q)$(RM) -rf iso
	$(Q)printf "\033[1;32m[Done]\033[0m Compilation complete.\n\n"

.PHONY: help run clean gen.clangd menuconfig format check

help:
	$(Q)printf "Uinxed-Kernel Makefile Usage:\n"
	$(Q)printf "  make all         - Build the entire project.\n"
	$(Q)printf "  make run         - Run the Uinxed-x64.iso in QEMU.\n"
	$(Q)printf "  make clean       - Clean all generated files.\n"
	$(Q)printf "  make gen.clangd  - Generate .clangd configuration file.\n"
	$(Q)printf "  make menuconfig  - Run menuconfig to configure the kernel.\n"
	$(Q)printf "  make format      - Format all source files using clang-format.\n"
	$(Q)printf "  make check       - Run static code checks using clang-tidy.\n"
	$(Q)printf "  make help        - Display this help message.\n\n"

run: Uinxed-x64.iso
	$(QEMU) $(QEMU_FLAGS) -cdrom $<
	$(Q)echo

clean:
	$(Q)$(RM) $(OBJS) $(DEPS) UxImage Uinxed-x64.iso
	$(Q)printf "\033[1;32m[Done]\033[0m Clean completed.\n\n"

gen.clangd:
	$(Q)$(RM) -f .clangd
	$(Q)echo "# Generated by Makefile" >> .clangd
	$(Q)sed "s/\$${workspaceFolder}/$(subst /,\/,${PWD})/g" .clangd_template >> .clangd
	$(Q)printf "\033[1;32m[Done]\033[0m .clangd configuration generated.\n\n"

menuconfig:
	$(Q)kconfig-mconf Kconfig

format: $(C_SOURCES:%=%.fmt) $(S_SOURCES:%=%.fmt) $(HEADERS:%=%.fmt)
	$(Q)printf "\033[1;32m[Done]\033[0m Code Format complete.\n\n"

check: $(C_SOURCES:%=%.tidy) $(S_SOURCES:%=%.tidy) $(HEADERS:%=%.tidy)
	$(Q)printf "\033[1;32m[Done]\033[0m Code Checks complete.\n\n"

-include $(DEPS)
