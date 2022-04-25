CC := i686-w64-mingw32-gcc

OculusDash.exe:
	${CC} -s -O3 main.c -lcomdlg32 -o $@
.PHONY: OculusDash.exe

vrstartup.exe:
	${CC} vrstartup.c -o $@
.PHONY: vrstartup.exe

clean:
	rm -f *.exe
.PHONY: clean

all: OculusDash.exe vrstartup.exe
