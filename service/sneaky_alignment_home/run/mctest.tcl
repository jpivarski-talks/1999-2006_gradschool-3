default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/mctest.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/runOnPass2.tcl

proc sel ../build_SunOS/shlib/MCTest.so
param MCTest explicitBeamSpot true

source_format sel PDSSourceFormat
source create pdschain

file add pdschain /cdat/sol221/disk3/c3mc/data8/ups4s_MN/output/BBbar_0130M_113243_1.pds event beginrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data8/ups4s_MN/output/BBbar_0130M_113244_1.pds event beginrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data8/ups4s_MN/output/BBbar_0130M_113246_1.pds event beginrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data8/ups4s_MN/output/BBbar_0130M_113247_1.pds event beginrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data8/ups4s_MN/output/BBbar_0130M_113248_1.pds event beginrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data8/ups4s_MN/output/BBbar_0130M_113250_1.pds event beginrun physics
file add pdschain /cdat/sol221/disk3/c3mc/data8/ups4s_MN/output/BBbar_0130M_113251_1.pds event beginrun physics

source activate pdschain beginrun event physics

go

hbook terminate
exit

