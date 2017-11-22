11.3.2016
	atdump:
	Textfiles are not shown correctly when '-c' option is choosen.
	Assembler Source files from 'AtmassII' are shown quite right...
7.3.2016
	atdir:
	Openeing 'atdir' with one ore more options set but no given pathname results in
	an segmentation fault. 

17.2.2016
	atdir:
	Follwing the file links gets 'atdir' stuck in an endless loop if next sector is pointing to
	itself. 'atdir' should chek for this.
 