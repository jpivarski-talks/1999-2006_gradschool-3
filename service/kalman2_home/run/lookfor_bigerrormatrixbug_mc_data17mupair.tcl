default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/LookForBigErrorMatrixBug.so

source_format sel PDSSourceFormat
source create pdschain

file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122283-E5000-DTS15102002_091752-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122281-E5000-DTS15102002_091739-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122285-E5000-DTS15102002_091804-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122274-E5000-DTS15102002_091613-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122280-E5000-DTS15102002_091725-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122277-E5000-DTS15102002_091648-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122276-E5000-DTS15102002_091637-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122278-E5000-DTS15102002_091700-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122279-E5000-DTS15102002_091712-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/output/mumu-Jun27_02_MC-FL-1-R122275-E5000-DTS15102002_091624-pass2.pds event startrun beginrun endrun physics

go

exit
