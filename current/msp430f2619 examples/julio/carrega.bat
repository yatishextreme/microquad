@echo off
REM carrega.bat v1/2008 -RCV-
cls

echo                   USE COM1 OU 2 PARA BSL E COM5 PARA SBW

IF NOT EXIST %1.elf GOTO ERRO0
IF "%2"=="COM1" GOTO CARREGACOM1
IF "%2"=="COM2" GOTO CARREGACOM2
IF "%2"=="COM5" GOTO SPYBWIRE
GOTO ERRO2

:CARREGACOM1
echo                 CARREGANDO ARQUIVO PELA %2 - AGUARDE
echo.

msp430-bsl --invert-reset --invert-test -c0 -epv -r %1.elf
IF ERRORLEVEL 1 GOTO ERRO1
echo.
echo                          CARREGAMENTO COMPLETO
GOTO FIM


:CARREGACOM2
echo                 CARREGANDO ARQUIVO PELA %2 - AGUARDE
echo.

msp430-bsl --invert-reset --invert-test -c1 -epv -r %1.elf
IF ERRORLEVEL 1 GOTO ERRO1
echo.
echo                          CARREGAMENTO COMPLETO
GOTO FIM

:SPYBWIRE
echo                 CARREGANDO ARQUIVO PELA %2 (SPY-BI-WIRE) - AGUARDE
echo.

msp430-JTAG --spy-bi-wire --lpt=COM5 -eEpv %1.elf
IF ERRORLEVEL 1 GOTO ERRO1
echo.
echo                          CARREGAMENTO COMPLETO
GOTO FIM


:ERRO0
echo.
echo                          ERRO! ARQUIVO %1.elf INEXISTENTE
echo                                 COMPILE O ARQUIVO
GOTO FIM


:ERRO1
echo.
echo                          FALHA NO CARREGAMENTO
GOTO FIM

:ERRO2
echo.
echo           PORTA INVALIDA: USE APENAS COM1 OU COM2 PARA BOOTSTRAP LOADER
echo                 OU COM5 CASO QUEIRA USAR COMUNICA��O SPY-BI-WIRE
GOTO FIM



rem echo.

:FIM