TITLE DataFile Backup
@ECHO ON

REM - $Id$

set WINRAR_PATH=C:\Program Files\WinRAR\
set SOURCE_PATH=D:\Games\polserver\data\
set SAVE_PATH=D:\Games\polserver\backups

set FDATE=%date:~4,25%
set FDATE=%FDATE:/=-%

set FHOUR=%time:~0,2%
set FHOUR=%FHOUR: =%

"%WINRAR_PATH%winrar.exe" a -m5 -s %SAVE_PATH%\data_hr%FHOUR%_on%FDATE%.rar %SOURCE_PATH%