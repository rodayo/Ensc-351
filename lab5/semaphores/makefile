sema=semaa.c
semb=semab.c
sema_exe=semA
semb_exe=semB
ext_lib=rt

all:
	@echo "Compiling source files..."
	gcc -o $(sema_exe) $(sema) -l$(ext_lib)
	gcc -o $(semb_exe) $(semb) -l$(ext_lib)
	
clean:
	rm -f $(sema_exe) $(semb_exe)