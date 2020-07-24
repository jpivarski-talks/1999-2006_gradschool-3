default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel DoitProd
prod desel DoitProd
proc sel ../build_SunOS/shlib/CompareInOutChi2.so

module sel DBModule
database in /nfs/cleo3/database/data9/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 114141

# source_format sel PDSSourceFormat
# file source /home/mccann/kalman5/pds/r114141_backaged.pds

go 10000

