call clean.bat

copy ..\..\..\Bin.QT\Release\*.dll .
copy ..\..\..\Bin.QT\Release\*.exe .
cp -r ..\..\..\Bin.QT\Release\Help .

set QT_BIN_DIR=C:\Qt\6.10.1\msvc2022_64\bin

c:\cygwin64\bin\find . -name "tt3*.dll" | xargs %QT_BIN_DIR%\windeployqt
c:\cygwin64\bin\find . -name "tt3*.exe" | xargs %QT_BIN_DIR%\windeployqt
