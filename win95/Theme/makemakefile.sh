#!/bin/sh

THEME="Win95"
FILES="ICON.png README.html"
SUBDIRS="gtk"

echo    'EXTRA_DIST = makemakefile.sh'                   > Makefile.am
echo    'dummy ='                                        >> Makefile.am
echo    'SUBDIRS = '$SUBDIRS                             >> Makefile.am
echo    'themedir = $(datadir)/themes/'$THEME            >> Makefile.am
echo -n 'theme_DATA = '                                  >> Makefile.am
for i in $FILES ; do 
echo -n $i' '                                            >> Makefile.am 
done
echo    ' '                                              >> Makefile.am
