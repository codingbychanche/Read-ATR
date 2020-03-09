Read ATR
=================
RetroZock 2014
www.retrozock.com
retrozock@hotmail.com

A collection of C- Language subroutines and tools for processing '*.ATR' image files. 

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

atsvtoc
-------
Reads the VTOC (Volume Table of Contents) from an '*.ATR' image file and writes it to the standard output.


Version
=======
Execute binary without command line options, version and usage will be displplayed, or check individual source file.

License
=======
This work is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License:
https://creativecommons.org/licenses/by-nc-sa/4.0/
 	

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
Long version, showing full status of each file:
	
	bertholsappleii:Read ATR Berthold$ atdir Samples/FMeine.atr 

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


Short version, showing the directory like 'DUP.SYS' does:

	bertholsappleii:Read ATR Berthold$ atdir -s Samples/DOS25.ATR

	atdir V2.8.4 // 19.11.2018

	Made by retrozock
	www.retrozock.com

	------------------------------------------------------------------------
	Imagefile: Samples/DOS25.ATR
		   DOS     .SYS
	  	   DUP     .SYS
	   	   TEST    .BAS
	   	   TEST    .DAT
	   	   TEST    .DAT
		   PIA2    .BAS

 

Sample output of 'atdump'
-------------------------

Hex dump:

	bertholsappleii:Read ATR Berthold$ atdump -h Samples/FMeine.atr@ZEICHEN.TXT

	;------------------
	; Dump of:ZEICHEN.TXT

	.byte  $2a,$2d,$2d,$2d,$2d,$2d,$2d,$2d
	.byte  $2d,$2d,$2d,$2d,$2d,$2d,$2d,$2d
	.byte  $2d,$2d,$2d,$2d,$2d,$2d,$2d,$2d
	.byte  $2d,$2d,$2d,$2d,$2d,$2d,$2d,$2d
	.byte  $2d,$2d,$2d,$2d,$2d,$0d,$2a,$20
	.byte  $5a,$45,$49,$43,$48,$45,$4e,$53
	.byte  $41,$54,$5a,$0d,$2a,$2d,$2d,$2d
	.byte  $2d,$2d,$2d,$2d,$2d,$2d,$2d,$2d



Sample output of 'atsvtoc'
--------------------------

	bertholsappleii:0_src Berthold$ atsvtoc ../Samples/FMeine.atr 


	Checking disk image:../Samples/FMeine.atr

	Volume Table of Contents (VTOC)
	-------------------------------
	Dos Version:2
	Bytes/ sector:128 and # of usable sectors=707 =>Single density

	1      BSC  BSC  BSC  T00  T00  T00  T00  T00  	       9	T00  T00  T00  T00  T00  T00  T00  T00		17	T00  T00  T00  T00  T00  T00  T00  T00   24
	25     T00  T00  T00  T00  T00  T00  T00  T00  	       33	T00  T00  T00  T00  T00  T00  T00  T00  	41	T01  T01  T01  T01  T01  T01  T01  T01   48
	49     T01  T01  T01  T01  T01  T01  T01  T01  	       57	T01  T01  T01  T01  T01  T01  T01  T01  	65	T01  T01  T01  T01  T01  T01  T01  T01   72
	73     T01  T01  T01  T01  T01  T01  T01  T01  	       81	T01  T01  T02  T02  T02  T02  T02  T02  	89	T02  T02  T02  T02  T02  T02  T02  T02   96
	97     T02  T02  T02  T02  T02  T02  T02  T02  	       105	T02  T02  T02  T02  T02  T02  T02  T02  	113	T02  T02  T02  T02  T02  T02  T02  T02   120
	121    T02  T02  T02  T02  T02  T02  T02  T02  	       129	T02  T02  T02  T02  T02  T02  T02  T02  	137	T02  T02  T02  T02  T02  T02  T02  T02   144
	145    T02  T02  T02  T02  T02  T02  T02  T02  	       153	T02  T02  T02  T02  T02  T02  T02  T02  	161	T02  T02  T02  T02  T02  T02  T02  T02   168
	169    T02  T02  T02  T02  T02  T02  T02  T02  	       177	T02  T02  T02  T02  T02  T02  T02  T02  	185	T02  T02  T02  T02  T02  T02  T02  T02   192
	193    T02  T02  T02  T02  T02  T02  T02  T02  	       201	T02  T02  T02  T02  T02  T02  T02  T02  	209	T02  T02  T02  T02  T02  T02  T02  T03   216
	217    T03  T03  T03  T03  T03  T03  T03  T04  	       225	T04  T04  T04  T04  T05  T05  T05  T05  	233	T05  T05  T05  T05  T06  T06  T06  T06   240
	241    T06  T06  T06  T06  T06  T06  T06  T06  	       249	T06  T06  T06  T07  T07  T07  T08  T08  	257	T09  T10  T10  T10  T10  T10  T10  T10   264
	265    T10  T10  T10  T10  T10  T10  T10  T10  	       273	T10  T10  T10  T10  T10  T10  T10  T10  	281	T10  T10  T10  T10  T10  T10  T10  T10   288
	289    T10  T10  T10  T10  T10  T11  T12  T12  	       297	T12  T12  T12  T12  T12  T12  T12  T12  	305	T12  T13  T13  T13  T13  T14  T14  T14   312
	313    T14  T14  T14  T14  T14  T14  T14  T14  	       321	T14  T14  T14  T14  T14  T14  T14  T14  	329	T14  T14  T14  T15  T15  T15  T15  T15   336
	337    T15  T15  T15  T15  T15  T15  T15  T15  	       345	T15  T15  T15  T15  T15  T15  T15  T15  	353	T15  T15  T15  T16  T16  T16  T16  VTC   360
	361    DIR  DIR  DIR  DIR  DIR  DIR  DIR  DIR  	       369	T16  T16  T16  T16  T16  T16  T16  T16  	377	T16  T16  T16  T16  T16  T16  T16  T16   384
	385    T16  T16  T16  T16  T24  E24  E24  E24  	       393	E24  E24  E24  E24  E24  E24  E24  E24  	401	E24  E24  E24  E24  E24  E25  E25  E25   408
	409    E25  E25  E25  E25  E25  E25  E25  E25  	       417	E25  E25  E26  E26  E26  E26  E26  E26  	425	E27  E27  E27  E27  E27  E28  E28  E28   432
	433    E28  E28  E28  E28  E28  E28  E28  E28  	       441	E28  E28  E28  E28  E28  E28  E28  E29  	449	E29  E29  E29  E29  E29  E29  E29  E29   456
	457    E29  E29  E29  E29  E29  E29  E29  E29  	       465	E29  E29  E29  E29  E29  E29  E29  E29  	473	E29  E29  E29  E29  E29  E29  E29  E29   480
	481    E29  E29  E29  E29  E29  E29  E29  E29  	       489	E29  E29  E29  E29  E29  E29  E29  E29  	497	E29  E29  E29  E29  E29  E29  E29  E30   504
	505    E30  E30  E30  E30  E30  E30  E30  E30  	       513	E30  E30  E30  E30  E30  E30  E30  E30  	521	E30  E30  E30  E30  E30  E30  E30  E30   528
	529    E30  E30  E30  E30  E30  E30  E30  E30  	       537	E30  E30  E30  E30  E30  E30  E30  E30  	545	E30  E30  E30  E30  E30  E30  E30  E31   552
	553    E31  E31  E31  E31  E31  E31  E32  E32  	       561	E32  E32  E32  E32  E32  E32  E32  E32  	569	E32  E32  E32  E32  E32  E32  E32  E32   576
	577    E32  E32  E32  E32  E32  E32  E32  E32  	       585	E32  E33  E33  E33  E33  E33  E33  E33  	593	E33  E33  E33  E34  E34  E34  E34  E34   600
	601    E34  E34  E34  E34  E34  E34  E34  E34  	       609	E34  E34  E34  E34  E34  E34  E34  E34  	617	E34  E35  E35  E35  E35  E35  E35  E35   624
	625    E35  E35  E35  E35  E35  E35  E35  E35  	       633	E35  E35  E35  E35  E35  E35  E35  E35  	641	E35  E35  E35  E35  E35  E35  E35  E35   648
	649    E35  E35  E35  E35  E35  E35  E35  E35  	       657	E35  E36  E36  E36  E36  E36  E36  E36  	665	E36  E36  E36  E36  E36  E36  E36  E36   672
	673    E36  E36  E36  E36  E36  E36  E36  E36  	       681	E36  E36  E37  E37  E37  E37  E37  E37  	689	E37  E37  E38  E38  E38  E38  E38  E38   696
	697    E38  E38  E38  E38  E00  E00  E00  E00  	       705	E00  E00  E00  E00  E00  E00  E00  E00  	713	E00  E00  E00  E00  E00  E00  E00  ---   720

	Total number of free Sectors:331


