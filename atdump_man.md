atdump
======
Vx.x // 3/2016

Dumps the contents of an file, saved on an Atari Dos 2.x formated disc to the standart output.

Works for single density (720 sectors, 128 bytes/ sector) enhanched density (1040 sectors, 128
bytes/ sector) and double density (720 sectors/ 256 bytes/ sector). 

Debugging has been done, using the native 'Disk Wizzard II' tool on an emulated Atari 800 XL (Atari800MacX 13.26.26).

Usage
=====
Usage: [-options][Filename]@[DOS2XFIL.EXX]

Filename	=	Valid *.ATR disk image, DOS 2.X formated. 
DOS2XFIL.EXX	=	Point's to the file on the disk image.
			Must be a valid DOS2X file name in UPPERCASE

If no parameters are passed, 'atdump' will show a copyright message and options.
If no options are passed but a filename was given, 'atdump' abords.

Output:
=======
Either a raw dump of the files contents (ascii or whatever) or a hexdump with a header and rows of eight bytes each preceded by 
the '.byte' assembler directive.





      

        

