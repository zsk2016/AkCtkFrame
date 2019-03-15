set AKCTKDIR=D:\project\AkCtkFrame\AkCtk-vs2015-qt5.7.1-32
set QTDIR=C:\Qt\Qt5.7.1_2015\5.7\msvc2015

set path="%path%;C:\Program Files (x86)\MySQL\MySQL Server 5.7\lib;"

::echo %path%

echo %QTDIR%


start /d "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE"  "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe" D:\project\AkCtkFrame\AkCtkFrame.sln

::pause