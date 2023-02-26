@echo off
if exist ..\source\%1 (
    scp ..\source\%1 MI_gr_I31@hpc.usm.md:~/Cataln_Pv/%1
    goto end
)

echo ..\source\%1 missing

:end
