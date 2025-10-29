call clean.bat

copy ..\..\..\Bin.QT\Release\*.dll .
copy ..\..\..\Bin.QT\Release\*.exe .
cp -r ..\..\..\Bin.QT\Release\Help .

c:\cygwin64\bin\find . -name "*.dll" | xargs C:\Qt\6.10.0\msvc2022_64\bin\windeployqt
c:\cygwin64\bin\find . -name "*.exe" | xargs C:\Qt\6.10.0\msvc2022_64\bin\windeployqt
