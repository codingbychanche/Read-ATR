# Builds all binarys 
#
# Puts the binary's and  'manual' files in 
# the matching folder and removes all temporary
# files ('a.out', backup files etc...)
#
# After build, use 'install' to put the binary's
# into the 'roots' 'bin' folder (make them available as
# commands....

echo Building 'atdir' for x86
cc -w -o bin/x86/Mac/atdir  atdir.c 
cp atdir_man.md bin/x86/Mac

echo Bulding 'atdump' for x86
cc -w -o bin/x86/Mac/atdump atdump.c 
cp atdump_man.md bin/x86/Mac

echo Removing backup files
rm *.*~

echo Removing ".out"- files
rm *.out

# Test
echo Building 'atdump' for 68000 Atari TOS
/opt/cross-mint/bin/m68k-atari-mint-gcc -o atdump.prg atdump.c 