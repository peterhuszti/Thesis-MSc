REM author: Peter Huszti

@echo off

cd ..\source\soe_seq
g++ -o soe_seq -O3 main.cpp

set "soeSeqStartTime=%time%"
soe_seq.exe
set "soeSeqStopTime=%time%"
call :timeToMS time1 "%soeSeqStartTime"
call :timeToMS time2 "%soeSeqStopTime"
set /a soeSeqTotalTime=time2-time1

cd ..\cols_seq
g++ -o cols_seq -O3 main.cpp

set "colsSeqStartTime=%time%"
cols_seq.exe
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