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
# BF 12/2020

rm errorLog

# Grey text
printf  "\e[90m"

# Let's build

echo --------------------------------------- Building 'atdir' for x86
cc -o ../bin/x86/Mac/atdir  atdir.c 2>>errorLog
cp atdir_man.md ../bin/x86/Mac

echo --------------------------------------- Bulding 'atdump' for x86
cc -o ../bin/x86/Mac/atdump atdump.c 2>>errorLog
cp atdump_man.md ../bin/x86/Mac

echo --------------------------------------- Bulding 'atshvtoc' for x86
cc -o ../bin/x86/Mac/atsvtoc atsvtoc.c 2>>errorLog
cp atshvtoc_man.md ../bin/x86/Mac

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
/opt/cross-mint/bin/m68k-atari-mint-gcc -o ../bin/68000/TOS/atdump.ttp atdump.c 2>>errorLog
cp atdump_man.md ../bin/x86/Mac

echo ---------------------------------------Building 'atdir' for 68000 Atari TOS
/opt/cross-mint/bin/m68k-atari-mint-gcc -o ../bin/68000/TOS/atdir.ttp atdir.c 2>>errorLog
cp atdir_man.md ../bin/x86/Mac

echo ---------------------------------------Building 'atsvotc' for 68000 Atari TOS
/opt/cross-mint/bin/m68k-atari-mint-gcc -o ../bin/68000/TOS/atsvtoc.ttp atsvtoc.c 2>>errorLog
cp atdir_man.md ../bin/x86/Mac

echo ---------------------------------------- DONE!

# Display result's
#
# Any errors, if so, show them

lines=0
while read errorLog
do
    let "lines=lines+1"
done < errorLog

if [ $lines -gt 0 ]
then
    echo Error log contains $lines lines. 
else
    echo Error log contains no lines. You lucky bastard!
fi

# Reset all escape squences...

printf "\e[m"
