default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/LookForBigErrorMatrixBug.so

source_format sel PDSSourceFormat
source create pdschain

file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121476-E5000-DTS15102002_101153-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121474-E5000-DTS15102002_101141-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121527-E5000-DTS15102002_101337-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121525-E5000-DTS15102002_101311-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121526-E5000-DTS15102002_101324-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121479-E5000-DTS15102002_101217-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121480-E5000-DTS15102002_101229-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121523-E5000-DTS15102002_101258-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121477-E5000-DTS15102002_101205-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data16/mumu/output/mumu-Jun27_02_MC-FL-1-R121519-E5000-DTS15102002_101244-pass2.pds event startrun beginrun endrun physics

go

exit
