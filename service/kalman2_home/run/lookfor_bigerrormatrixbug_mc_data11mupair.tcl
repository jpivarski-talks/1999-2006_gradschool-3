default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/LookForBigErrorMatrixBug.so

source_format sel PDSSourceFormat
source create pdschain

file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116428-E5000-DTS27012003_104410-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116432-E5000-DTS27012003_104456-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116421-E5000-DTS27012003_104255-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116422-E5000-DTS27012003_104308-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116424-E5000-DTS27012003_104338-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116420-E5000-DTS27012003_104240-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116423-E5000-DTS27012003_104324-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116429-E5000-DTS27012003_104425-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116430-E5000-DTS27012003_104441-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol531/disk3/c3mc/data11/mumu/output/mumu-Jun27_02_MC-FL-1-R116425-E5000-DTS27012003_104354-pass2.pds event startrun beginrun endrun physics

go

exit
