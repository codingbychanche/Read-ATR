atdump
======
Dumps the contents of an file, saved on an Atari Dos 2.x formated disk to the standart output.

Works for single density (720 sectors, 128 bytes/ sector) enhanched density (1040 sectors, 128
bytes/ sector) and double density (720 sectors/ 256 bytes/ sector). 

Debugging has been done, using the native 'Disk Wizzard II' tool on an emulated Atari 800 XL (Atari800MacX 13.26.26).

Usage
=====
Usage: [-ch][Filename]@[DOS2XFIL.EXT]

Filename	=	Valid *.ATR disk image, DOS 2.X formated. 
DOS2XFIL.EXX	=	Point's to the file on the disk image.
			Must be a valid DOS2X file name in UPPERCASE

-c			ascii- dump, for the time beeing, no atascii char's are shown
			and there is no hint if an atascii char was read from file but
			not displayed.

-h			Hex dump. 
			Bytes displayed, preceeded by the '.byte'- assembler directive,
			ready to copy and paste into your source code.

-r			Raw dump.
			Dumps a file as it is. Binarys can be run as '*.XEX' from your
			emulator.

If no parameters are passed, 'atdump' will show a copyright message and options.
If no options are passed but a filename was given, 'atdump' abords.

BF 6/2018






      

        

