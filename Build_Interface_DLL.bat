
SET /P MATLAB_TEST=Enter the path of MATLAB ROOT DIRECTORY: 
SETX MATLAB_PATH %MATLAB_TEST%
SETX MATLAB_INCLUDE %MATLAB_TEST%\extern\include
SETX MATLAB_LIB %MATLAB_TEST%\extern\lib\win32\microsoft

devenv "NetSim.sln" /clean
devenv "NetSim.sln" /build Debug /project "NetSim_MATLAB_Interface\NetSim_MATLAB_Interface.vcxproj" /projectconfig Debug  

SET /P PAUSE=:

