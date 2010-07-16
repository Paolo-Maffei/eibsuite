set EIB_PROJECT_DIR="%1%"

del %EIB_PROJECT_DIR%\EIBVoice\bin\Debug\EIBStdLib.dll
del %EIB_PROJECT_DIR%\EIBVoice\bin\Debug\EIB.NetWrapper.dll

copy "%EIB_PROJECT_DIR%\Debug\EIBStdLib.dll" "%EIB_PROJECT_DIR%\EIBVoice\bin\Debug\"
copy "%EIB_PROJECT_DIR%\Debug\EIB.NetWrapper.dll" "%EIB_PROJECT_DIR%\EIBVoice\bin\Debug\"

del %EIB_PROJECT_DIR%\EIBVoice\bin\Release\EIBStdLib.dll
del %EIB_PROJECT_DIR%\EIBVoice\bin\Release\EIB.NetWrapper.dll

copy "%EIB_PROJECT_DIR%\Release\EIBStdLib.dll" "%EIB_PROJECT_DIR%\EIBVoice\bin\Release\"
copy "%EIB_PROJECT_DIR%\Release\EIB.NetWrapper.dll" "%EIB_PROJECT_DIR%\EIBVoice\bin\Release\"