call clean.bat

copy ..\..\Bin.QT\Debug\*.dll .
copy ..\..\Bin.QT\Debug\*.exe .
copy ..\..\Bin.QT\Debug\*.pdb .

c:\cygwin64\bin\find . -name "*.dll" | xargs C:\Qt\6.8.0\msvc2022_64\bin\windeployqt
c:\cygwin64\bin\find . -name "*.exe" | xargs C:\Qt\6.8.0\msvc2022_64\bin\windeployqt
