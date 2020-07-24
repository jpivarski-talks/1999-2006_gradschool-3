default prompt off

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl

# param KalmanProd PrintDiagnostics true

proc sel ../build_SunOS/shlib/FindPathologies.so

source_format sel PDSSourceFormat
file in /home/erdene/RichProblem/pds/fj_pipi_cg1.pds beginrun event startrun endrun

proc sel ../build_SunOS/shlib/SpExtractKineExitPionFitProc.so
run_file $env(C3_SCRIPTS)/viewTrackFitter.tcl

# report level DEBUG

goto 112259 17
go 1

# look for 1.18621e-10
