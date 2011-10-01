mema=memoryA.c
memb=memoryB.c
mema_exe=memA
memb_exe=memB
ext_lib=rt

all:
	@echo "Compiling sources files..."
	gcc -o $(mema_exe) $(mema) -l$(ext_lib)
	gcc -o $(memb_exe) $(memb) -l$(ext_lib)
clean:
	rm -f $(mema_exe) $(memb_exe)