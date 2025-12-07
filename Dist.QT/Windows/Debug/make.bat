call clean.bat

copy ..\..\..\Bin.QT\Debug\*.dll .
copy ..\..\..\Bin.QT\Debug\*.exe .
copy ..\..\..\Bin.QT\Debug\*.pdb .
cp -r ..\..\..\Bin.QT\Debug\Help .

c:\cygwin64\bin\find . -name "*.dll" | xargs C:\Qt\6.10.1\msvc2022_64\bin\windeployqt
c:\cygwin64\bin\find . -name "*.exe" | xargs C:\Qt\6.10.1\msvc2022_64\bin\windeployqt
