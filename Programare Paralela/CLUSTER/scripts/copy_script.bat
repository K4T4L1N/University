@echo off
if exist %1 (
    scp %1 MI_gr_I31@hpc.usm.md:~/Catalin_Pv/%1
    goto end
)

echo %1 missing

:end
