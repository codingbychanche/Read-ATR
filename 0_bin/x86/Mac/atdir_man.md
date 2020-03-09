atdir
=====
Shows the directory of the specified disk image. It shows file #, start sector, # of sectors
and the size in bytes. it also checks the files sector chain.

Works for single density (720 sectors, 128 bytes/ sector) enhanched density (1040 sectors, 128
bytes/ sector) and double density (720 sectors/ 256 bytes/ sector). 

Debugging has been done, using the native 'Disk Wizard II' tool on an emulated Atari 800 XL (Atari800MacX 13.26.26).

Usage
=====
Usage: [-vfsa][Filename]

Filename=	Valid *.ATR disk image, DOS 2.X formated. 
-v:		Hide copyright message	  	
-f:		Filename contains path- list of *.ATR image files. Batch processing
-s:		Shows short version of the disk directory, like DUP.SYS does. 
-a:		Dumps dir in an 'atdump' compatible format, so that it can be used as an input for it.

Output:
=======

FN		 = File # = Order of file in directory

STAT	    	 = File Status *= write protected // 2= DOS 2.0 file // #= file deleted 
		   N= File has never been used // O= File is opened // U= File is in use

FILENAME    	 = Filename
EXT		 = The extension
START	      	 = First data sector
SECT	    	 = Number of sectors occupied by the file
Bytes	    	 = File size in Bytes (sum of data bytes/ sector of the file)

SECTOR CHAIN 	 
The 'adir' tool checks every file. Valid conditions to declare a file as OK are:

     File # (order in directory) matches file # in each data sector and file # from each
     data sector is the same!

     Number of sectors (from directory entry)= number of sectors actually read until last
     sector of file was reached.

Error codes:
     
      OK  
      No issues
      
      F#! 
      file number error=> File number does not match the files order in directory or
      not all file numbers from the data sectors match each other.

      Size! 
      Number of sectors read does not match number of sectors in directory.

      #
      File was deleted.

If a deleted file (marked with '#') 'sector chain' is marked with 'OK' it can easily be restored 
(undeleted). You can use native Atari 8- Bit Software to do so or, in the near future, a tool from
this collection.  

PLEASE NOTE: An important difference between popoular disk tools (Like 'Disk Wizard II')for the 8- Atari 
and 'adir' is: The 'adir' tool  reads reads and counts all sectors of a file - deleted or not - 
until the last sector (indicated by 'next sector pointer' is 'sector zero'). 

In other words, it counts the errors and displays the sum rather than stopping and showing just the 
first error encountered.

This will help the advanched user to determine if the file might be recoverable
or not (this might require using an disk monitor to check sectors contents semantically
and repair file links manually).




      

        

