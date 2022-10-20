@echo off
title This is your first batch script!

rem For each file in your folder
for %%f in ("..\output\*") do (
echo ">>>>>>> frame %%~nf start <<<<<<<"
.\particles2obj.exe -i %%f -o ".\output\%%~nf.obj"
echo ">>>>>>> frame %%~nf complete <<<<<<<"
)

echo complete

pause