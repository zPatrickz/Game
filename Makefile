CC = gcc
AS = gas
LD = ld
# CFLAGS option generates a static file for running on raw machine
# we use highest warning level to prevent potential errors
CFLAGS = -static -fno-builtin -fno-stack-protector \
		 -Wall -Werror -O2 -I./include

LDFLAGS = 
OBJCOPY = objcopy
OBJDUMP = objdump

# list all the target files below, they will be automatically compiled
# with $(CFLAGS) option, with header dependencies correctly resolved
OBJS = main.o \
	   irq/i8259.o \
	   irq/idt.o \
	   irq/irq_handle.o \
	   irq/do_irq.o \
	   game.o \
	   timer.o \
	   video/video.o \

# our game will be at physical memory position 0x100000,
# and does not enable paging, entry point is game_init
# (in main.c)
game: $(OBJS)
	$(LD) $(LDFLAGS) -e game_init -Ttext 0x00100000 -o game $(OBJS)
	@echo ""
	@echo -e "\e[0;31mCongratulations!\e[0m"
	@echo "You have successfully *Make* it work!"
	@echo  -e "Why not \e[0;31mcommit \e[0mit now?"
	@echo "(or you may wanna test it first, anyway, good luck!)"

-include *.d

# if you want to remove useless files, just "make clean"
# next time "make" will have a full compilation
clean:
	rm -f *.d *.o *.asm game
	rm -f irq/*.d irq/*.o
	rm -f video/*.d video/*.o
