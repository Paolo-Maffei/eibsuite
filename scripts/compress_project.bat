@echo off

set EIB_PROJECT_DIR=%1%
cd %EIB_PROJECT_DIR%

set FILES_EXTENSIONS = %EIB_PROJECT_DIR%\*.cpp %EIB_PROJECT_DIR%\*.h %EIB_PROJECT_DIR%\*.sln %EIB_PROJECT_DIR%\*.vcproj %EIB_PROJECT_DIR%\*.db %EIB_PROJECT_DIR%\*.template %EIB_PROJECT_DIR%\*.ico %EIB_PROJECT_DIR%\*.png %EIB_PROJECT_DIR%\*.bmp %EIB_PROJECT_DIR%\*.cs %EIB_PROJECT_DIR%\*.csproj %EIB_PROJECT_DIR%\*.resx %EIB_PROJECT_DIR%\*.settings %EIB_PROJECT_DIR%\*.vdproj %EIB_PROJECT_DIR%\*Debug %EIB_PROJECT_DIR%\*.mk %EIB_PROJECT_DIR%\makefile

"C:\Program Files\WinRar\rar" a -u -as %EIB_PROJECT_DIR%\scripts\backup.rar %FILES_EXTENSIONS%