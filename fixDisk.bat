@echo off
set drive=D:

chkdsk %drive%
IF %ERRORLEVEL% NEQ 0 (
    echo Errors found. Fixing volum...
    echo Y | chkdsk %drive% /F
) else (
    echo No errors found.
)
pause
