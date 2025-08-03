::@echo off
:: Cambia la ruta de putty.exe si no está en el PATH del sistema
set PUTTY_PATH="C:\Program Files\PuTTY\putty.exe"

%PUTTY_PATH% -serial COM4 -sercfg 115200,8,n,1,N
