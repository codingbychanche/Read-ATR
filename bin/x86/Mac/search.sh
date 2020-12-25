# search.sh
# 
# This sample shows how one can use the 'find' and 'grep'
# comands in conjunction with the 'atdir' tool
#

# Start searching all '*.atr' files in current dir
# and write result in 'result' test file

find . -name *.atr > result

# search 'result' for any lines containing 'TXT'

grep 'TXT' result