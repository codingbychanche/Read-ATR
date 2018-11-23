Read ATR
=================
RetroZock 2014
www.retrozock.com
retrozock@hotmail.com

A collection of C- Language subroutines and tools for processing *.ATR image files. 

A sample output of each tool is included at the end of this file.

atdir
-----
Reads the directory of an Atari DOS 2.x formated disk and writes it to the standard output.
Deleted files are also displayed. All files are checked (sector chain, file size, file #).
'atdir' accepts single '*.ATR' image files as input and file list for batch processing.
'atdump' accepts it's output as input.

atdump
------
Dumps file contents of a file in an '*.ATR' image file to standard output.

atsvto
------c
Reads the VTOC (Volume Table of Contents) from an '*.ATR' image file and writes it to the standart output.


Version
=======
Execute binary without command line options, version and usage will be displplayed, or check individual source file.

License
=======

 	

Binary's (and how to build them....)
====================================
You will find the latest binar's of all the tools in:
    Read ATR/bin/x86/Mac 

ONLY THE MAC OS BINARY'S ARE MAINTAINED AND YOU CAN BE SHURE THAT THOSE REPRESENT THE COMPILER RESULT WITH LATEST SOURCE CODE. 
ALL OTHER BINARY'S ARE NOT! (E.G. Atari TOS, MS- DOS...).

    Build the binary's on your Mac for your Mac:
    --------------------------------------------   
    Use the 'build_readATR.sh' script to compile the binary's. The executables and the manuals will be written to
    the 'Mac- Os' 'bin'- folder'.

    Build the binary's for other platforms:
    ---------------------------------------
    Use your favourite compiler. 
    
    I have sucessfully build binary's for the Atari ST on my Mac with Vincent Riviere's binary's of the
    'm68k-atari-mint cross-tools' (see: vincent.riviere.fr).
    

Sample outputs
==============

Sample output of 'atdir'
------------------------

	atdir V2.8.3 // 12.10.2018

	Made by retrozock
	www.retrozock.com

	------------------------------------------------------------------------
	Image file: Samples/Design.ATR

	# of useable data sectors on disk/ unused data sectors on disk:1010/ 0

	FN	Stat	Filename	Start	#Sect.	#Bytes	Sector chain
	---	------	---------------	-----	------	------	------------
	0	*NU2	DOS    .SYS	4	37	4625	OK
	1	*NU2	AUTORUN.SYS	41	14	1724	OK
	2	*NU2	DM     .   	55	171	21364	OK
	3	*NU2	HC     .   	226	86	10663	OK
	4	*NU2	FASTEX8.SET	312	1	17	OK
	5	*NU2	FX80GR .SET	313	1	19	OK
	6	*NU2	FX80KL .SET	314	1	19	OK
	7	*NU2	MX80GR .SET	315	1	18	OK
	8	*NU2	DM     .FNT	316	9	1025	OK
	9	*NU2	ANTIK  .FNT	325	9	1025	OK
	10	*NU2	ECKIG  .FNT	334	9	1025	OK
	11	*NU2	ATARI  .FNT	343	9	1025	OK
	12	*NU2	ANTIK2 .FNT	352	9	1025	OK
	13	*NU2	HOHL   .FNT	370	9	1025	OK
	14	NU2	UNIVER .PIC	379	62	7680	OK
	15	*NU2	SCHALT .PIC	441	62	7680	OK
	16	*NU2	VOLIERE.PIC	503	62	7680	OK
	17	*NU2	BOOM   .PIC	565	62	7680	OK
	18	*NU2	KIPPEHO.PIC	627	62	7680	OK
	19	*NO2	CAMERA .PIC	689	62	7680	OK
	20	*NO2	SUNMOON.PIC	752	62	7680	OK
	21	*NO2	SNOOPY .PIC	814	62	7680	OK
	22	*NO2	TIGER  .PIC	876	62	7680	OK
	23	*NO2	MICROMA.PIC	938	62	7680	OK
 


