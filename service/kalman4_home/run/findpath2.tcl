default prompt off

module sel HbookHistogramModule
hbook file ../rzn/findpath2_broken2.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl

proc sel ../build_SunOS/shlib/FindPathologies.so

source_format sel PDSSourceFormat
file in /home/erdene/RichProblem/pds/fj_pipi_cg1.pds beginrun event startrun endrun

go 70

