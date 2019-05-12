set FXCOPTS=/Zi

if not exist "Compiled"  mkdir "Compiled"

call :CompileShaderHLSL%1 ps_cube ps main
call :CompileShaderHLSL%1 vs_cube vs main
call :CompileShaderHLSL%1 ps_color ps main
call :CompileShaderHLSL%1 vs_color vs main
call :CompileShaderHLSL%1 vs_debug vs main
call :CompileShaderHLSL%1 ps_debug ps main
:CompileShaderHLSL
set fxc=fxc.exe %1.hlsl %FXCOPTS% /T%2_5_0 /E%3 /FoCompiled\%1.cso
echo.
echo %fxc%
%fxc% || set error=1
exit /b