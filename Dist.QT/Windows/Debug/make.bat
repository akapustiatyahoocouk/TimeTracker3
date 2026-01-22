call clean.bat

copy ..\..\..\Bin.QT\Debug\*.dll .
copy ..\..\..\Bin.QT\Debug\*.exe .
copy ..\..\..\Bin.QT\Debug\*.pdb .
cp -r ..\..\..\Bin.QT\Debug\Help .

set QT_BIN_DIR=C:\Qt\6.10.1\msvc2022_64\bin

c:\cygwin64\bin\find . -name "tt3*.dll" | xargs %QT_BIN_DIR%\windeployqt
c:\cygwin64\bin\find . -name "tt3*.exe" | xargs %QT_BIN_DIR%\windeployqt
