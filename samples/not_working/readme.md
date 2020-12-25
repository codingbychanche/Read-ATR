Here you will find *.atr images that crash the current version of 'atdir'. 

TINY-C_program_disk.atr
	Last file ('NL') is deleted and ist stored on sector 416 and 538.
	Sector 538 points back to start sector 416 making 'trace' loop
	infinitly.... Versions 2.6 and lower of 'atdir' can not cope with this
	and crash!

	16.3.2016: This error should be detected by version 2.7 and above of 'atdir'

