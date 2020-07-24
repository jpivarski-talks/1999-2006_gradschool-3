rem #
set name=%1%
set bin=..\%2%
dumpbin /out:tmp /symbols %bin%\%name%.arc
%OMAKEHOME%\%OMAKECONFIG%\owindef.exe %name% < tmp > %name%.def
del tmp
