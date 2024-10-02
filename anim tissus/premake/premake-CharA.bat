
cd ..

.\premake\premake5.exe --os=windows --file=./master_CharAnim.lua gmake
.\premake\premake5.exe --os=windows --file=./master_CharAnim.lua codeblocks
rem .\premake\premake5.exe --os=windows --file=./master_CharAnim.lua vs2013
rem .\premake\premake5.exe --os=windows --file=./master_CharAnim.lua vs2015
rem .\premake\premake5.exe --os=windows --file=./master_CharAnim.lua vs2015
rem .\premake\premake5.exe --os=windows --file=./master_CharAnim.lua vs2019
.\premake\premake5.exe --os=windows --file=./master_CharAnim.lua vs2022
pause
