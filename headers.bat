@echo off

::this batch will traverse headers and create new homonymic headers reference former.
::it will avoid developer adding vast header search paths tediously.

pushd %~dp0

set dst=GHEADERS

if exist %dst% (rmdir /q /s %dst%)
mkdir %dst%

set err=0

set src="mainsrc"
call :gen
if not %err% == 0 (goto end)

goto end

:gen
for /r %src% %%f in (*.h) do (
    if not exist %~dp0\%dst%\%%~nxf (
        echo #include "%%f" >> %~dp0\%dst%\%%~nxf
    ) else (
        echo "error: homonymic '%%~nxf'."
        rmdir /q /s %dst%
        set err=1
        goto end
    )
)

:end

popd
