set EIB_PROJECT_DIR=C:\Users\Yosi\Desktop\EIBProject

del %WINDIR%\System32\EIBStdLib.dll
del %WINDIR%\System32\EIB.NetWrapper.dll

copy %EIB_PROJECT_DIR%\Debug\EIBStdLib.dll %WINDIR%\System32
copy %EIB_PROJECT_DIR%\Debug\EIB.NetWrapper.dll %WINDIR%\System32