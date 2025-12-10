call clean.bat

copy ..\..\..\Bin.QT\Debug\*.dll .
copy ..\..\..\Bin.QT\Debug\*.exe .
copy ..\..\..\Bin.QT\Debug\*.pdb .
cp -r ..\..\..\Bin.QT\Debug\Help .

set QT_BIN_DIR=C:\Qt\6.10.1\msvc2022_64\bin

c:\cygwin64\bin\find . -name "*.dll" | xargs %QT_BIN_DIR%\windeployqt
c:\cygwin64\bin\find . -name "*.exe" | xargs %QT_BIN_DIR%\windeployqt

rem del Qt*.dll
rem We do several passes to pick up transitive dpendencies
rem ldd *.dll | grep -h "not found" | cut -d" " -f1 | gawk '{$1=$1; print}' | grep -h -v "api-ms-win-" | sort | uniq | sed 's/\r$//' | xargs -I DLLNAME cp %QT_BIN_DIR%\DLLNAME .
rem ldd *.dll | grep -h "not found" | cut -d" " -f1 | gawk '{$1=$1; print}' | grep -h -v "api-ms-win-" | sort | uniq | sed 's/\r$//' | xargs -I DLLNAME cp %QT_BIN_DIR%\DLLNAME .
rem ldd *.dll | grep -h "not found" | cut -d" " -f1 | gawk '{$1=$1; print}' | grep -h -v "api-ms-win-" | sort | uniq | sed 's/\r$//' | xargs -I DLLNAME cp %QT_BIN_DIR%\DLLNAME .
rem ldd *.dll | grep -h "not found" | cut -d" " -f1 | gawk '{$1=$1; print}' | grep -h -v "api-ms-win-" | sort | uniq | sed 's/\r$//' | xargs -I DLLNAME cp %QT_BIN_DIR%\DLLNAME .
