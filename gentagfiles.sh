#! /bin/bash
#gentagfiles file list build script
set -x
find | grep "\.c$\|\.h$" > cscope.files
sort cscope.files > cscope.files.sorted
mv cscope.files.sorted cscope.files
cscope -b -q
ctags -L cscope.files
