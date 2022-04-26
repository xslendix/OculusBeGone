CC := i686-w64-mingw32-gcc

OculusDash.exe:
	${CC} -s -O3 -std=c99 OculusDash.c -lcomdlg32 -o $@
.PHONY: OculusDash.exe

clean:
	rm -f *.exe
.PHONY: clean

all: OculusDash.exe vrstartup.exe
