#!/bin/sh

THEME="Pixmap"
FILES="gtkrc *.png"

echo    'EXTRA_DIST = makemakefile.sh'                    >  Makefile.am
echo    'dummy ='                                         >> Makefile.am
echo    'themedir = $(datadir)/themes/'$THEME'/gtk'       >> Makefile.am
echo -n 'theme_DATA = '                                   >> Makefile.am
for i in $FILES ; do 
echo -n $i' '                                             >> Makefile.am 
done
echo    ' '                                               >> Makefile.am
