@echo off
cd %1
FOR /L %%G IN (0,1,7) DO (
copy %%G.bmp %2%%G.bmp
bmp2bin.exe %2%%G.bmp
del  %2%%G.bmp
move %2%%G.h ../
)


