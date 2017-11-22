# example.sh
#
# Shows how 'adir' can be used in conection with 'grep'
#
# Usage: example [filename] [search term]
#
# Where 'filename' represents the name of the *.ATR disk image and
# 'search term' any compination  of characters you want to find within the
# output of 'adir'
#
# Very simple

adir $1 | grep $2 -