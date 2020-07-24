default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/runOnPass2.tcl

prod sel DBEventHeaderProd

proc sel ../build_SunOS/shlib/LookForBigErrorMatrixBug.so

module sel DBModule
database in /nfs/cleo3/database/data$env(USER_DB)/db event startrun beginrun endrun
database read_collection pass2 hadron
go

exit
