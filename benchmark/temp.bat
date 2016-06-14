@echo off

g++ -o ..\source\soe_seq\a -O3 ..\source\soe_seq\main.cpp
g++ -o ..\source\cols_seq\a -O3 ..\source\cols_seq\main.cpp

set "soeSeqStartTime=%time%"
..\source\soe_seq\a.exe
set "soeSeqStopTime=%time%"
call :timeToMS time1 "%soeSeqStartTime"
call :timeToMS time2 "%soeSeqStopTime"
set /a soeSeqTotalTime=time2-time1

set "colsSeqStartTime=%time%"
..\source\cols_seq\a.exe
set "colsSeqStopTime=%time%"
call :timeToMS time1 "%colsSeqStartTime"
call :timeToMS time2 "%colsSeqStopTime"
set /a colsSeqTotalTime=time2-time1

echo.
echo Soe seq:  %soeSeqTotalTime% milli seconds
echo Cols seq:  %colsSeqTotalTime% milli seconds

goto :eof

:timeToMS
SETLOCAL EnableDelayedExpansion
FOR /F "tokens=1,2,3,4 delims=:,.^ " %%a IN ("!%~2!") DO (
  set /a "ms=(((30%%a%%100)*60+7%%b)*60+3%%c-42300)*1000+(1%%d0 %% 1000)"
)
(
  ENDLOCAL
  set %~1=%ms%
  goto :eof
)