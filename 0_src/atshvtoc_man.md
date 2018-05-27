Show Volume Table of Contents (VTOC)
====================================
DOS 2.x VTOC is located at sector 369 and contains 10 bytes status information (total # of sectors and
available # of sectors) and 90 bytes (90 x 8 bit = 720 bit= 720 sectors) which are the sector map. Each bit set marks an 
sector that my be overwritten (data of an deleted file) and each zero bit a sector that may not be overwritten (
sectors data belong toan active file).

The tool maps the disc by reading the VTOC and checking which sector belongs to a deleted file or an active file.
File # and status are marked as follows:

- active file:	   
       03T     03= File number // T= Taken, means that this sector has data for an active file and my not be overwritten.

- deleted file:	   
	10E	   10= File number // E= Empty, means that this sector belongs to the deleted file #10 and my be overwritten.

- System 	
	DIR	   Sectors used by dos to store the disc's directory (sectors 361 - 368)
	VTC	   Volume table of contents 10 + 90 bytes (sector 369)

	Both are marked as "Taken" and my not be overwritten by the system.


BF 5/2018