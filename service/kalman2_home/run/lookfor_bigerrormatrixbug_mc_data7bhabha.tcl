default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/LookForBigErrorMatrixBug.so

source_format sel PDSSourceFormat
source create pdschain

file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/output/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112805-E5269-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/output/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112802-E3387-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/output/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112806-E4977-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/output/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112803-E5539-pass2.pds event startrun beginrun endrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/output/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112804-E4832-pass2.pds event startrun beginrun endrun physics

go

exit