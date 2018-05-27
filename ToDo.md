27.5.2018
	atdump
	When Atari file name has 8 chars + extension, file will not be found! Reason:
	'EXAMPLE1.TXT' will be translated to 'EXAMPLETXT.'

26.4.2018
	atdump
	Only hex- dump with '.byte' directive and textfiles are aupported. 
	Add 'raw' binary' option to create 'xex' which can be executed by an emulator....

02.12.2017
	atdir/ atdump
	Pathnames send to stdout should be readable by the system => whitspaces 
	should be preceeded by '\'

11.3.2016
	atdump:
	Textfiles are not shown correctly when '-c' option is choosen.
	Assembler Source files from 'AtmassII' are shown quite right...

17.2.2016
	atdir:
	Follwing the file links gets 'atdir' stuck in an endless loop if next sector is pointing to
	itself. 'atdir' should chek for this.

Solved
------

22.11.2017
	7.3.2016
	atdir:
	Opening 'atdir' with one ore more options set but no given pathname results in
	an segmentation fault. 