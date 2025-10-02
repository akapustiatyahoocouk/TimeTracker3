if exist *.dll del *.dll
if exist *.exe del *.exe
if exist *.pdb del *.pdb

c:\cygwin64\bin\find . -type d | grep "./" | tr -d "\r" | xargs rm -r
