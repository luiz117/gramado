# Compiling Gramado on Manjaro Linux.
# BSD License
# SPDX-License-Identifier: GPL-3.0+
# History:
#     2018 - Created by Matheus Castello.
#     2019 - Fred Nora.
#     2019 - Luciano Gonçalez.

# important
# We will have a new name just when the release changes.

VERSION = 1
PATCHLEVEL = 20
SUBLEVEL = 0
EXTRAVERSION = -rc14
NAME = 



# That's our default target when none is given on the command line
PHONY := _all
_all: xxx_x86


ARCH ?= x86


# Make variables (CC, etc...)
#AS		= as
#LD		= ld
#CC		= gcc
#AR		= ar
#OBJCOPY		= objcopy
#OBJDUMP		= objdump
#LEX		= flex
#YACC	= bison
#PERL		= perl
#PYTHON		= python
#PYTHON2		= python2
#PYTHON3		= python3
#RUBY		= ruby



#GRAMADOINCLUDE := -I include/


# Setup disk:
# For now we're using IDE disk on primary master.
# You can setup this options in:
# arch/x86/boot/bl/include/config/config.h for the boot loader and
# include/kernel/config/config.h for the kernel.


#funciona
#CFLAGS = -m32 \
#	--std=gnu89 \
#	-nodefaultlibs \
#	-nostdinc \
#	-nostdlib \
#	-static \
#	-fgnu89-inline \
#	-ffreestanding \
#	-fno-builtin \
#	-fno-pie \
#	-no-pie \
#	-fleading-underscore \
#	-fno-stack-protector \
#	-s   


#tests
CFLAGS = -m32 \
	--std=gnu89 \
	-nodefaultlibs \
	-nostdinc \
	-nostdlib \
	-static \
	-fgnu89-inline \
	-ffreestanding \
	-fno-builtin \
	-fno-pie \
	-no-pie \
	-fleading-underscore \
	-fno-stack-protector \
	-s \
	-Werror=strict-prototypes 


	#importante:
	##todo estamos incluindo essa flag, para isso temos que editar 
	#os argumentos em muitas funções ainda..
	#-Werror=strict-prototypes  



#testando esses
#	-Wno-trigraphs \
#	-Wno-format-security \
#	-fno-strict-aliasing \
#    -Werror=implicit-int \
#	-Werror=implicit-function-declaration \
#	-fshort-wchar \
#	-Wundef 	
#testing ...
#-Wall                        fail
#-fno-common                  fail
#-Werror=strict-prototypes    fail	 Warn for functions declared or defined without specified argument types.
#-E                           fail
#-Wunused-function
#-Wmissing-prototypes




##
## Defines
##

#
# DEFINES =  -DGRAMADO_VERSION=$(VERSION) \
#		-DGRAMADO_PATCHLEVEL=$(PATCHLEVEL) \
#		-DGRAMADO_SUBLEVEL=$(SUBLEVEL) \
#		-DGRAMADO_EXTRAVERSION=\"$(EXTRAVERSION)\" \
#		-DGRAMAD0_NAME=\"$(NAME)\"


##
## Objects
##

ifeq ($(ARCH),x86)

	#deveria ser headx86.o
	ENTRY_OBJECTS := head.o main.o x86main.o 
	
	EXECVE_OBJECTS := pipe.o socket.o ctype.o  stdio.o stdlib.o string.o unistd.o \
	devmgr.o \
	gde_serv.o \
	debug.o diskvol.o install.o object.o runtime.o \
	abort.o info.o io.o modules.o signal.o sm.o \
	init.o system.o \
	execve.o 
	
	HAL_OBJECTS := cpuamd.o portsx86.o syscall.o x86.o detect.o \
	hal.o 
	
	KDRIVERS_OBJECTS := ahci.o \
	ata.o atadma.o atainit.o atairq.o atapci.o hdd.o \
	channel.o network.o nicintel.o nsocket.o \
	pci.o pciinfo.o pciscan.o \
	tty.o pty.o\
	usb.o \
	video.o vsync.o screen.o xproc.o \
	i8042.o keyboard.o mouse.o ps2kbd.o ps2mouse.o ldisc.o \
	apic.o pic.o rtc.o serial.o timer.o  
	
	KSERVERS_OBJECTS := cf.o format.o fs.o read.o search.o write.o \
	cedge.o bg.o bmp.o button.o char.o createw.o dtext.o font.o grid.o \
	line.o menu.o menubar.o pixel.o rect.o sbar.o toolbar.o wm.o \
	logoff.o \
	logon.o \
	input.o output.o terminal.o \
	desktop.o room.o userenv.o usession.o \
	kgwm.o kgws.o \
	vfs.o 
	
	
	MK_OBJECTS := x86cont.o x86fault.o x86start.o \
	dispatch.o pheap.o process.o queue.o spawn.o \
	tasks.o theap.o thread.o threadi.o ts.o tstack.o \
	callout.o callfar.o ipc.o ipccore.o sem.o \
	memory.o mminfo.o mmpool.o pages.o \
	preempt.o priority.o sched.o schedi.o \
	create.o \
	mk.o 


	REQUEST_OBJECTS := request.o 
	
	OBJECTS := $(ENTRY_OBJECTS) \
	$(EXECVE_OBJECTS) \
	$(HAL_OBJECTS) \
	$(KDRIVERS_OBJECTS) \
	$(KSERVERS_OBJECTS) \
	$(MK_OBJECTS) \
	$(REQUEST_OBJECTS)
	
endif


ifeq ($(ARCH),arm)
   # NOTHING FOR NOW
endif


xxx_x86: /mnt/gramadovhd compile-kernel link-x86 vhd-x86 vhd-mount vhd-copy-files vhd-unmount clean

#final message.
	@echo "Gramado $(VERSION) $(PATCHLEVEL) $(SUBLEVEL) $(EXTRAVERSION) $(NAME) "
	@echo "Arch x86"


compile-kernel:

	@echo "Compiling kernel ..."

	# /entry
	
	nasm -I kernel/arch/x86/entry/head/  kernel/arch/x86/entry/head/head.asm -f elf -o head.o	
	
	gcc -c kernel/arch/x86/entry/x86main.c  -I include/ $(CFLAGS) -o x86main.o
	
    #main
	gcc -c kernel/main.c  -I include/ $(CFLAGS) -o main.o

	# /hal
	gcc -c kernel/hal/hal.c                -I include/ $(CFLAGS) -o hal.o
	gcc -c kernel/hal/arch/detect.c        -I include/ $(CFLAGS) -o detect.o
	gcc -c kernel/hal/arch/amd/cpuamd.c    -I include/ $(CFLAGS) -o cpuamd.o
	gcc -c kernel/hal/arch/x86/portsx86.c  -I include/ $(CFLAGS) -o portsx86.o
	gcc -c kernel/hal/arch/x86/syscall.c   -I include/ $(CFLAGS) -o syscall.o
	gcc -c kernel/hal/arch/x86/x86.c       -I include/ $(CFLAGS) -o x86.o

	# /mk
	gcc -c  kernel/mk/mk.c -I include/  $(CFLAGS) -o mk.o

	# /ps/arch
	gcc -c  kernel/mk/ps/arch/x86/x86cont.c   -I include/  $(CFLAGS) -o x86cont.o
	gcc -c  kernel/mk/ps/arch/x86/x86fault.c  -I include/  $(CFLAGS) -o x86fault.o
	gcc -c  kernel/mk/ps/arch/x86/x86start.c  -I include/  $(CFLAGS) -o x86start.o

	# /ps/action
	gcc -c  kernel/mk/ps/action/dispatch.c  -I include/  $(CFLAGS) -o dispatch.o
	gcc -c  kernel/mk/ps/action/pheap.c     -I include/  $(CFLAGS) -o pheap.o
	gcc -c  kernel/mk/ps/action/process.c   -I include/  $(CFLAGS) -o process.o
	gcc -c  kernel/mk/ps/action/queue.c     -I include/  $(CFLAGS) -o queue.o
	gcc -c  kernel/mk/ps/action/spawn.c     -I include/  $(CFLAGS) -o spawn.o
	gcc -c  kernel/mk/ps/action/tasks.c     -I include/  $(CFLAGS) -o tasks.o
	gcc -c  kernel/mk/ps/action/theap.c     -I include/  $(CFLAGS) -o theap.o
	gcc -c  kernel/mk/ps/action/thread.c    -I include/  $(CFLAGS) -o thread.o
	gcc -c  kernel/mk/ps/action/threadi.c   -I include/  $(CFLAGS) -o threadi.o
	gcc -c  kernel/mk/ps/action/ts.c        -I include/  $(CFLAGS) -o ts.o
	gcc -c  kernel/mk/ps/action/tstack.c    -I include/  $(CFLAGS) -o tstack.o

	# /ps/ipc
	gcc -c  kernel/mk/ps/ipc/callfar.c  -I include/  $(CFLAGS) -o callfar.o
	gcc -c  kernel/mk/ps/ipc/callout.c  -I include/  $(CFLAGS) -o callout.o
	gcc -c  kernel/mk/ps/ipc/ipc.c      -I include/  $(CFLAGS) -o ipc.o
	gcc -c  kernel/mk/ps/ipc/ipccore.c  -I include/  $(CFLAGS) -o ipccore.o
	gcc -c  kernel/mk/ps/ipc/sem.c      -I include/  $(CFLAGS) -o sem.o


	# /ps/mm (memory manager)

	#x86
	gcc -c  kernel/mk/ps/mm/x86/memory.c  -I include/ $(CFLAGS) -o memory.o
	gcc -c  kernel/mk/ps/mm/x86/mminfo.c  -I include/ $(CFLAGS) -o mminfo.o
	gcc -c  kernel/mk/ps/mm/x86/mmpool.c  -I include/ $(CFLAGS) -o mmpool.o
	gcc -c  kernel/mk/ps/mm/x86/pages.c   -I include/ $(CFLAGS) -o pages.o

	#arm

	# /ps/sched
	gcc -c  kernel/mk/ps/sched/preempt.c   -I include/ $(CFLAGS) -o preempt.o
	gcc -c  kernel/mk/ps/sched/priority.c  -I include/ $(CFLAGS) -o priority.o
	gcc -c  kernel/mk/ps/sched/sched.c     -I include/ $(CFLAGS) -o sched.o
	gcc -c  kernel/mk/ps/sched/schedi.c    -I include/ $(CFLAGS) -o schedi.o

	#mk
	gcc -c kernel/mk/create.c  -I include/  $(CFLAGS) -o create.o

	#kernel/request
	gcc -c  kernel/request.c  -I include/ $(CFLAGS) -o request.o


	# /execve
	gcc -c kernel/execve/execve.c  -I include/ $(CFLAGS) -o execve.o

	# crts
	
	#klibc/pipe
	gcc -c kernel/execve/crts/klibc/pipe/pipe.c  -I include/ $(CFLAGS) -o pipe.o
	
	#klibc/socket
	gcc -c kernel/execve/crts/klibc/socket/socket.c   -I include/ $(CFLAGS) -o socket.o

	# klibc
	gcc -c kernel/execve/crts/klibc/ctype.c   -I include/ $(CFLAGS) -o ctype.o
	gcc -c kernel/execve/crts/klibc/stdlib.c  -I include/ $(CFLAGS) -o stdlib.o
	gcc -c kernel/execve/crts/klibc/stdio.c   -I include/ $(CFLAGS) -o stdio.o
	gcc -c kernel/execve/crts/klibc/string.c  -I include/ $(CFLAGS) -o string.o
	gcc -c kernel/execve/crts/klibc/unistd.c  -I include/ $(CFLAGS) -o unistd.o

	
	gcc -c kernel/kdrivers/apic.c    -I include/ $(CFLAGS) -o apic.o
	gcc -c kernel/kdrivers/pic.c     -I include/ $(CFLAGS) -o pic.o
	gcc -c kernel/kdrivers/rtc.c     -I include/ $(CFLAGS) -o rtc.o
	gcc -c kernel/kdrivers/serial.c  -I include/ $(CFLAGS) -o serial.o
	gcc -c kernel/kdrivers/timer.c   -I include/ $(CFLAGS) -o timer.o


	# kdrivers/ahci 
	# todo
	gcc -c kernel/kdrivers/ahci/ahci.c  -I include/ $(CFLAGS) -o ahci.o
	
		
	#ide support
	gcc -c kernel/kdrivers/ide/hdd.c      -I include/ $(CFLAGS) -o hdd.o
	gcc -c kernel/kdrivers/ide/ata.c      -I include/ $(CFLAGS) -o ata.o
	gcc -c kernel/kdrivers/ide/atainit.c  -I include/ $(CFLAGS) -o atainit.o
	gcc -c kernel/kdrivers/ide/atairq.c   -I include/ $(CFLAGS) -o atairq.o
	gcc -c kernel/kdrivers/ide/atapci.c   -I include/ $(CFLAGS) -o atapci.o	
	gcc -c kernel/kdrivers/ide/atadma.c   -I include/ $(CFLAGS) -o atadma.o	
	
	# kdrivers/network
	gcc -c kernel/kdrivers/network/channel.c   -I include/ $(CFLAGS) -o channel.o
	gcc -c kernel/kdrivers/network/nicintel.c  -I include/ $(CFLAGS) -o nicintel.o
	gcc -c kernel/kdrivers/network/network.c   -I include/ $(CFLAGS) -o network.o
	gcc -c kernel/kdrivers/network/nsocket.c   -I include/ $(CFLAGS) -o nsocket.o

	# kdrivers/pci
	gcc -c kernel/kdrivers/pci/pci.c      -I include/ $(CFLAGS) -o pci.o
	gcc -c kernel/kdrivers/pci/pciscan.c  -I include/ $(CFLAGS) -o pciscan.o
	gcc -c kernel/kdrivers/pci/pciinfo.c  -I include/ $(CFLAGS) -o pciinfo.o

	# kdrivers/tty
	gcc -c kernel/kdrivers/tty/tty.c  -I include/ $(CFLAGS) -o tty.o
	gcc -c kernel/kdrivers/tty/pty.c  -I include/ $(CFLAGS) -o pty.o
	
	# kdrivers/usb
	gcc -c kernel/kdrivers/usb/usb.c  -I include/ $(CFLAGS) -o usb.o
	
	# kdrivers/x
	gcc -c kernel/kdrivers/x/xproc.c   -I include/ $(CFLAGS) -o xproc.o

	# kdrivers/x/video
	gcc -c kernel/kdrivers/x/video/screen.c  -I include/ $(CFLAGS) -o screen.o
	gcc -c kernel/kdrivers/x/video/video.c   -I include/ $(CFLAGS) -o video.o
	gcc -c kernel/kdrivers/x/video/vsync.c   -I include/ $(CFLAGS) -o vsync.o


	# kdrivers/x/i8042
	gcc -c kernel/kdrivers/x/i8042/i8042.c     -I include/ $(CFLAGS) -o i8042.o
	gcc -c kernel/kdrivers/x/i8042/keyboard.c  -I include/ $(CFLAGS) -o keyboard.o
	gcc -c kernel/kdrivers/x/i8042/mouse.c     -I include/ $(CFLAGS) -o mouse.o
	gcc -c kernel/kdrivers/x/i8042/ps2kbd.c    -I include/ $(CFLAGS) -o ps2kbd.o
	gcc -c kernel/kdrivers/x/i8042/ps2mouse.c  -I include/ $(CFLAGS) -o ps2mouse.o
	gcc -c kernel/kdrivers/x/i8042/ldisc.c     -I include/ $(CFLAGS) -o ldisc.o


	# devices/
	gcc -c kernel/devices/devmgr.c  -I include/ $(CFLAGS) -o devmgr.o



	# /fs
	gcc -c kernel/kservers/fs/fs.c      -I include/ $(CFLAGS) -o fs.o
	gcc -c kernel/kservers/fs/read.c    -I include/ $(CFLAGS) -o read.o
	gcc -c kernel/kservers/fs/write.c   -I include/ $(CFLAGS) -o write.o
	gcc -c kernel/kservers/fs/cf.c      -I include/ $(CFLAGS) -o cf.o
	gcc -c kernel/kservers/fs/search.c  -I include/ $(CFLAGS) -o search.o
	gcc -c kernel/kservers/fs/format.c  -I include/ $(CFLAGS) -o format.o
	
	# /vfs
	gcc -c kernel/kservers/vfs/vfs.c  -I include/ $(CFLAGS) -o vfs.o

	# /sci/gde
	gcc -c kernel/execve/sci/gde/gde_serv.c  -I include/ $(CFLAGS) -o gde_serv.o

	# /sm
	gcc -c kernel/execve/sm/init.c    -I include/ $(CFLAGS) -o init.o
	gcc -c kernel/execve/sm/system.c  -I include/ $(CFLAGS) -o system.o
	gcc -c kernel/execve/sm/debug/debug.c      -I include/ $(CFLAGS) -o debug.o
	gcc -c kernel/execve/sm/disk/diskvol.c     -I include/ $(CFLAGS) -o diskvol.o
	gcc -c kernel/execve/sm/install/install.c  -I include/ $(CFLAGS) -o install.o
	gcc -c kernel/execve/sm/ob/object.c        -I include/ $(CFLAGS) -o object.o
	gcc -c kernel/execve/sm/rt/runtime.c       -I include/ $(CFLAGS) -o runtime.o
	gcc -c kernel/execve/sm/sys/abort.c    -I include/ $(CFLAGS) -o abort.o	
	gcc -c kernel/execve/sm/sys/info.c     -I include/ $(CFLAGS) -o info.o
	gcc -c kernel/execve/sm/sys/io.c       -I include/ $(CFLAGS) -o io.o
	gcc -c kernel/execve/sm/sys/modules.c  -I include/ $(CFLAGS) -o modules.o
	gcc -c kernel/execve/sm/sys/signal.c   -I include/ $(CFLAGS) -o signal.o
	gcc -c kernel/execve/sm/sys/sm.c       -I include/ $(CFLAGS) -o sm.o


	# kservers/kgwm - Kernel Gramado Window Manager.

	gcc -c kernel/kservers/kgwm/kgwm.c  -I include/ $(CFLAGS) -o kgwm.o
	gcc -c kernel/kservers/kgwm/wm.c    -I include/ $(CFLAGS) -o wm.o


	# kservers/kgws - Kernel Gramado Window Server.

	gcc -c kernel/kservers/kgws/kgws/comp/bg.c       -I include/ $(CFLAGS) -o bg.o
	gcc -c kernel/kservers/kgws/kgws/comp/bmp.c      -I include/ $(CFLAGS) -o bmp.o
	gcc -c kernel/kservers/kgws/kgws/comp/button.c   -I include/ $(CFLAGS) -o button.o
	gcc -c kernel/kservers/kgws/kgws/comp/cedge.c    -I include/ $(CFLAGS) -o cedge.o
	gcc -c kernel/kservers/kgws/kgws/comp/char.c     -I include/ $(CFLAGS) -o char.o
	gcc -c kernel/kservers/kgws/kgws/comp/createw.c  -I include/ $(CFLAGS) -o createw.o
	gcc -c kernel/kservers/kgws/kgws/comp/dtext.c    -I include/ $(CFLAGS) -o dtext.o
	gcc -c kernel/kservers/kgws/kgws/comp/font.c     -I include/ $(CFLAGS) -o font.o
	gcc -c kernel/kservers/kgws/kgws/comp/grid.c     -I include/ $(CFLAGS) -o grid.o
	gcc -c kernel/kservers/kgws/kgws/comp/line.c     -I include/ $(CFLAGS) -o line.o
	gcc -c kernel/kservers/kgws/kgws/comp/menu.c     -I include/ $(CFLAGS) -o menu.o
	gcc -c kernel/kservers/kgws/kgws/comp/menubar.c  -I include/ $(CFLAGS) -o menubar.o
	gcc -c kernel/kservers/kgws/kgws/comp/pixel.c    -I include/ $(CFLAGS) -o pixel.o
	gcc -c kernel/kservers/kgws/kgws/comp/rect.c     -I include/ $(CFLAGS) -o rect.o
	gcc -c kernel/kservers/kgws/kgws/comp/sbar.c     -I include/ $(CFLAGS) -o sbar.o
	gcc -c kernel/kservers/kgws/kgws/comp/toolbar.c  -I include/ $(CFLAGS) -o toolbar.o

	
	gcc -c kernel/kservers/kgws/logon/logon.c    -I include/ $(CFLAGS) -o logon.o
	gcc -c kernel/kservers/kgws/logoff/logoff.c  -I include/ $(CFLAGS) -o logoff.o

	gcc -c kernel/kservers/kgws/terminal/input.c     -I include/ $(CFLAGS) -o input.o
	gcc -c kernel/kservers/kgws/terminal/output.c    -I include/ $(CFLAGS) -o output.o
	gcc -c kernel/kservers/kgws/terminal/terminal.c  -I include/ $(CFLAGS) -o terminal.o
	
	gcc -c kernel/kservers/kgws/user/userenv.c   -I include/ $(CFLAGS) -o userenv.o
	gcc -c kernel/kservers/kgws/user/desktop.c   -I include/ $(CFLAGS) -o desktop.o
	gcc -c kernel/kservers/kgws/user/room.c      -I include/ $(CFLAGS) -o room.o
	gcc -c kernel/kservers/kgws/user/usession.c  -I include/ $(CFLAGS) -o usession.o
	
	gcc -c kernel/kservers/kgws/kgws.c  -I include/ $(CFLAGS) -o kgws.o





link-x86:
	ld -m elf_i386 -T kernel/link.ld -o KERNEL.BIN $(OBJECTS) -Map docs/kernel.map

#move
	mv KERNEL.BIN bin/boot/



#
# ======== HDD ========
#

danger-hdd-clone-vhd:
	sudo dd if=./GRAMADO.VHD of=/dev/sda
#	sudo dd if=./GRAMADO.VHD of=/dev/sdb
	
hdd-mount:
	-sudo umount /mnt/gramadohdd
	sudo mount -t vfat -o loop,offset=32256 /dev/sda /mnt/gramadohdd/
#	sudo mount -t vfat -o loop,offset=32256 /dev/sdb /mnt/gramadohdd/
	
hdd-unmount:
	-sudo umount /mnt/gramadohdd
	
hdd-copy-kernel:
	sudo cp bin/boot/KERNEL.BIN /mnt/gramadohdd/BOOT 


#
# ======== VHD ========
#

vhd-mount:
	@echo "Mounting VHD ..."
	-sudo umount /mnt/gramadovhd
	sudo mount -t vfat -o loop,offset=32256 GRAMADO.VHD /mnt/gramadovhd/

# umount
vhd-unmount:
	@echo "Unmounting VHD ..."
	sudo umount /mnt/gramadovhd

/mnt/gramadovhd:
	sudo mkdir /mnt/gramadovhd

vhd-x86:

	@echo "Creating VHD ..."

	nasm -I arch/x86/boot/vhd/stage1/ \
	-I arch/x86/boot/vhd/vbr/ \
	-I arch/x86/boot/vhd/footer/ arch/x86/boot/vhd/main.asm  -o  GRAMADO.VHD

# Copy content to disk
# 1) BM, BL 
# 2) KERNEL 
# 3) INIT, SHELL, TASKMAN
vhd-copy-files:





#
# ======== Files in the root dir. ========
#


#init/sys
	sudo cp bin/GRAMMGR.BIN    /mnt/gramadovhd


# bin/boot
	sudo cp bin/boot/BM.BIN       /mnt/gramadovhd
	sudo cp bin/boot/BL.BIN       /mnt/gramadovhd

# user/config
	sudo cp user/config/USER.TXT     /mnt/gramadovhd
	sudo cp user/config/INIT.TXT     /mnt/gramadovhd
	sudo cp user/config/GUI.TXT      /mnt/gramadovhd
	sudo cp user/config/GRAMADO.TXT  /mnt/gramadovhd



# bitmaps
	sudo cp kernel/kservers/kgws/bmp/APP.BMP       /mnt/gramadovhd
	sudo cp kernel/kservers/kgws/bmp/BMP1.BMP      /mnt/gramadovhd
	sudo cp kernel/kservers/kgws/bmp/CURSOR.BMP    /mnt/gramadovhd
	-sudo cp kernel/kservers/kgws/bmp/DENNIS.BMP   /mnt/gramadovhd
	sudo cp kernel/kservers/kgws/bmp/FILE.BMP      /mnt/gramadovhd
	sudo cp kernel/kservers/kgws/bmp/FOLDER.BMP    /mnt/gramadovhd
	-sudo cp kernel/kservers/kgws/bmp/GRAMADO.BMP  /mnt/gramadovhd
	sudo cp kernel/kservers/kgws/bmp/MOUSE.BMP     /mnt/gramadovhd
	sudo cp kernel/kservers/kgws/bmp/TERMINAL.BMP  /mnt/gramadovhd



# fonts
	sudo cp bin/NC2.FON /mnt/gramadovhd




# Get available apps.


# garden/apps

	#gwin
	-sudo cp ../garden/bin/GWINMGR.BIN   /mnt/gramadovhd
	-sudo cp ../garden/bin/GWS.BIN       /mnt/gramadovhd
	-sudo cp ../garden/bin/GWM.BIN       /mnt/gramadovhd

	#ili
	-sudo cp ../garden/bin/ILIINIT.BIN   /mnt/gramadovhd
	-sudo cp ../garden/bin/GLOGON.BIN    /mnt/gramadovhd

	#apps
	-sudo cp ../garden/bin/NORATERM.BIN  /mnt/gramadovhd
	-sudo cp ../garden/bin/GDETERM.BIN   /mnt/gramadovhd 
	-sudo cp ../garden/bin/GDESHELL.BIN  /mnt/gramadovhd 
	-sudo cp ../garden/bin/GRAMCODE.BIN  /mnt/gramadovhd 
	-sudo cp ../garden/bin/GRAMTEXT.BIN  /mnt/gramadovhd 
	-sudo cp ../garden/bin/GFE.BIN       /mnt/gramadovhd 
	-sudo cp ../garden/bin/SPR.BIN       /mnt/gramadovhd 
	-sudo cp ../garden/bin/SYSMON.BIN    /mnt/gramadovhd
	-sudo cp ../garden/bin/REBOOT2.BIN   /mnt/gramadovhd
	-sudo cp ../garden/bin/LAUNCHER.BIN  /mnt/gramadovhd
	-sudo cp ../garden/bin/WINTEST.BIN   /mnt/gramadovhd 
#	-sudo cp ../garden/bin/GDEINIT.BIN   /mnt/gramadovhd 
#	-sudo cp ../garden/bin/GDETM.BIN     /mnt/gramadovhd 


# atacama/cmd
	-sudo cp ../atacama/bin/CAT.BIN       /mnt/gramadovhd
#	-sudo cp ../atacama/bin/DUMPIT.BIN    /mnt/gramadovhd
	-sudo cp ../atacama/bin/HELLO.BIN     /mnt/gramadovhd
	-sudo cp ../atacama/bin/HELLO2.BIN    /mnt/gramadovhd
	-sudo cp ../atacama/bin/HELLO3.BIN    /mnt/gramadovhd
	-sudo cp ../atacama/bin/JACKPOT.BIN   /mnt/gramadovhd
#	-sudo cp ../atacama/bin/LISP.BIN      /mnt/gramadovhd
	-sudo cp ../atacama/bin/REBOOT.BIN    /mnt/gramadovhd
	-sudo cp ../atacama/bin/TASCII.BIN    /mnt/gramadovhd
#	-sudo cp ../atacama/bin/GLIBCT1.BIN   /mnt/gramadovhd 
	-sudo cp ../atacama/bin/SHELL3.BIN    /mnt/gramadovhd


# atacama/gt/gramcc
	-sudo cp ../atacama/bin/GRAM.BIN      /mnt/gramadovhd 
	-sudo cp ../atacama/bin/GRAMC.BIN     /mnt/gramadovhd 

# 3rd party
# Each app has your own folder.
#	-sudo cp ../atacama/bin/LUA.BIN       /mnt/gramadovhd
#	-sudo cp ../atacama/bin/BISON125.BIN  /mnt/gramadovhd
#	-sudo cp ../atacama/bin/FASM.BIN      /mnt/gramadovhd
#...


# c test suite
	-sudo cp ../atacama/gt/tests/TEST1.C    /mnt/gramadovhd
	-sudo cp ../atacama/gt/tests/TEST2.C    /mnt/gramadovhd
	-sudo cp ../atacama/gt/tests/TEST3.C    /mnt/gramadovhd
	-sudo cp ../atacama/gt/tests/*.LUA      /mnt/gramadovhd


#
# ======== Creating the all the folders in root dir ========
#		

# Creating standard folders
	-sudo mkdir /mnt/gramadovhd/BIN
	-sudo mkdir /mnt/gramadovhd/BOOT
	-sudo mkdir /mnt/gramadovhd/DEV
	-sudo mkdir /mnt/gramadovhd/DEV/PTS
	-sudo mkdir /mnt/gramadovhd/EFI
	-sudo mkdir /mnt/gramadovhd/EFI/BOOT
	-sudo mkdir /mnt/gramadovhd/ETC
	-sudo mkdir /mnt/gramadovhd/GARDEN
	-sudo mkdir /mnt/gramadovhd/GARDEN/BIN
	-sudo mkdir /mnt/gramadovhd/HOME
	-sudo mkdir /mnt/gramadovhd/LIB
	-sudo mkdir /mnt/gramadovhd/MNT
	-sudo mkdir /mnt/gramadovhd/SBIN
	-sudo mkdir /mnt/gramadovhd/TMP


#
# ======== Files in the BIN/ folder. ========
#

# bugbug
# Suspendendo isso por falta de espaço na partição.

	#-sudo cp ../garden/bin/*         /mnt/gramadovhd/BIN 
	#-sudo cp ../atacama/bin/*        /mnt/gramadovhd/BIN 
	-sudo cp ../atacama/bin/HELLO3.BIN  /mnt/gramadovhd/BIN 
	
#
# ======== Files in the /BOOT/ folder. ========
#

# principais.
# Serão carregados por bl.bin. 
	sudo cp bin/boot/KERNEL.BIN   /mnt/gramadovhd/BOOT
	sudo cp bin/boot/INIT.BIN     /mnt/gramadovhd/BOOT
	sudo cp bin/boot/SHELL.BIN    /mnt/gramadovhd/BOOT
	sudo cp bin/boot/TASKMAN.BIN  /mnt/gramadovhd/BOOT


#colocaremos drivers e servidores na pasta boot/
#	sudo cp bin/drivers/??.BIN       /mnt/gramadovhd/BOOT	
#	sudo cp bin/servers/??.BIN       /mnt/gramadovhd/BOOT


#
# ======== Files in the /DEV/ folder. ========
#

# TTY SERIAL DEVICES
	-sudo cp user/config/TTYS0     /mnt/gramadovhd/DEV
	-sudo cp user/config/TTYS1     /mnt/gramadovhd/DEV
	-sudo cp user/config/TTYS2     /mnt/gramadovhd/DEV
	-sudo cp user/config/TTYS3     /mnt/gramadovhd/DEV

# VIRTUAL CONSOLES
	-sudo cp user/config/TTY0     /mnt/gramadovhd/DEV
	-sudo cp user/config/TTY1     /mnt/gramadovhd/DEV
	-sudo cp user/config/TTY2     /mnt/gramadovhd/DEV
	-sudo cp user/config/TTY3     /mnt/gramadovhd/DEV
# ...

# NULL
	-sudo cp user/config/NULL     /mnt/gramadovhd/DEV

# PSEUDO TERMINAL
	-sudo cp user/config/0        /mnt/gramadovhd/DEV/PTS
	-sudo cp user/config/1        /mnt/gramadovhd/DEV/PTS
	-sudo cp user/config/PTMX     /mnt/gramadovhd/DEV/PTS


#
# ======== Files in the /EFI/ folder. ========
#
	
#test efi
#	-sudo cp arch/x86/boot/efi/BOOTIA32.EFI  /mnt/gramadovhd/EFI/BOOT

#
# ======== Files in the /GARDEN/ folder. ========
#

#garden
	-sudo cp arch/x86/boot/vhd/tests/TEST1.ASM  /mnt/gramadovhd/GARDEN
#	-sudo cp ../garden/bin/*                    /mnt/gramadovhd/GARDEN/BIN 


#
# ======== Files in the /LIB/ folder. ========
#	
	
	-sudo cp arch/x86/boot/vhd/tests/TEST1.ASM  /mnt/gramadovhd/LIB
	

#
# ======== Files in the SBIN/ folder. ========
#
	-sudo cp ../atacama/bin/HELLO3.BIN  /mnt/gramadovhd/SBIN 

#
# ======== Files in the /TMP/ folder. ========
#	

	-sudo cp arch/x86/boot/vhd/tests/TEST1.ASM  /mnt/gramadovhd/TMP

	
#
# ======== ISO ======== 
#

	
	
# test
# todo
# Create a .ISO file using nasm.
makeiso-x86:
	#todo:  
	#nasm -I arch/x86/boot/iso/stage1/ \
	#-I arch/x86/boot/iso/???/  arch/x86/boot/iso/main.asm  -o  GRAMADO.ISO
	
	@echo "#todo Create ISO using nasm"
	
# ISO
# Mount stage1.bin file with nasm.
# Create a .ISO file and move all the content of the /bin folder
# into the .ISO file.
geniso-x86:
	
	#stage1
	nasm arch/x86/boot/iso/stage1/stage1.asm -f bin -o stage1.bin
	cp stage1.bin bin/boot/gramado/
	rm stage1.bin

	#.ISO
	mkisofs -R -J -c boot/gramado/boot.catalog -b boot/gramado/stage1.bin -no-emul-boot -boot-load-size 4 -boot-info-table -o GRAMADO.ISO bin
	
	@echo "iso Success?"	


#
# ======== Oracle Virtual Box ======== 
#
oracle-virtual-box-test:
	VBoxManage startvm "Gramado"


#
# ======== qemu ======== 
#
qemu-test:
#	-debugcon stdio
	qemu-system-x86_64 -hda GRAMADO.VHD -m 128 -device e1000 -show-cursor

#install-kvm-qemu:
#	sudo pacman -S virt-manager qemu vde2 ebtables dnsmasq bridge-utils openbsd-netcat

#
# ======== Clean ======== 
#

clean:
	-rm *.o
	@echo "Success?"
	
clean2:
	-rm *.ISO
	-rm *.VHD
	

serial-debug:
	cat ./docs/sdebug.txt
	
