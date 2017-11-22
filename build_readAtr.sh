# Builds all binarys 

echo Building 'atdir' for x86
cc -w -o bin/x86/Mac/atdir  atdir.c 
cp atdir_man.md bin/x86/Mac

echo Bulding 'atdump' for x86
cc -w -o bin/x86/Mac/atdump atdump.c 
cp atdump_man.md bin/x86/Mac

echo Removing backup files
rm *.*~