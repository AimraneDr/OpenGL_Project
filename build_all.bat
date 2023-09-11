@ECHO OFF
REM Engine
make -f "engine.mk" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit) ELSE ECHO engine built successfully.

REM Testbed
make -f "game.mk" all
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && exit) ELSE ECHO game built successfully.

ECHO All assemblies built successfully.