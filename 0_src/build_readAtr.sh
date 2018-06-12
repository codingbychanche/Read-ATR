# Builds all binarys 
#
# Puts the binary's and  'manual' files in 
# the matching folder and removes all temporary
# files ('a.out', backup files etc...)
#
# After build, use 'install' to put the binary's
# into the 'roots' 'bin' folder (make them available as
# commands....) => Must have admin privileges!
#
# BF 4/2018

rm errorLog

echo --------------------------------------- Building 'atdir' for x86
cc -w -o ../0_bin/x86/Mac/atdir  atdir.c 2>>errorLog
cp atdir_man.md ../0_bin/x86/Mac

echo --------------------------------------- Bulding 'atdump' for x86
cc -w -o ../0_bin/x86/Mac/atdump atdump.c 2>>errorLog
cp atdump_man.md ../0_bin/x86/Mac

echo --------------------------------------- Bulding 'atshvtoc' for x86
cc -w -o ../0_bin/x86/Mac/atsvtoc atsvtoc.c 2>>errorLog
cp atshvtoc_man.md ../0_bin/x86/Mac

echo --------------------------------------- Cleaning root dir.....
echo -- Removing backup files
rm *.*~

echo --------------------------------------- Removing ".out"- files
rm *.out

# Test Build binary's for Atari Tos
# Needs 'm68k-atari-mint-gcc' to be installed....
#
# If you have already installed this compiler then you can either set the environment's 
# 'PATH' var or enter the full path below...
#
echo ---------------------------------------Building 'atdump' for 68000 Atari TOS
/opt/cross-mint/bin/m68k-atari-mint-gcc -o ../0_bin/68000/TOS/atdump.ttp atdump.c 2>>errorLog
cp atdump_man.md ../0_bin/x86/Mac

echo ---------------------------------------Building 'atdir' for 68000 Atari TOS
/opt/cross-mint/bin/m68k-atari-mint-gcc -o ../0_bin/68000/TOS/atdir.ttp atdir.c 2>>errorLog
cp atdir_man.md ../0_bin/x86/Mac

echo ---------------------------------------Building 'atsvotc' for 68000 Atari TOS
/opt/cross-mint/bin/m68k-atari-mint-gcc -o ../0_bin/68000/TOS/atsvtoc.ttp atsvtoc.c 2>>errorLog
cp atdir_man.md ../0_bin/x86/Mac

echo ---------------------------------------- DONE!

# Any errors, f so, show them
#
if [ $(egrep "error" errorLog) ]
then
    echo Errors found:
    egrep "error" errorLog
    echo For a comlete list of errors and/ or warnings: See 'errorlog'
else
    echo No errors! For a complete list of warnings: See 'errorLog'
fi


