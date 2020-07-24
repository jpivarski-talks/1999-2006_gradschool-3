default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/dsneak.rzn
hbook init

run_file $env(C3_SCRIPTS)/runOnPass2.tcl
run_file $env(C3_SCRIPTS)/BeamSpot.tcl
run_file $env(C3_SCRIPTS)/CcHotList.tcl

prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd

prod sel TrkmanProd
prod sel TrkmanSelectionProd

proc sel ../build_SunOS/shlib/DSneak.so

module sel DBModule
database in /nfs/cleo3/database/data16/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 121899 121915
go

module sel DBModule
database in /nfs/cleo3/database/data16/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 122080 122091
go

module sel DBModule
database in /nfs/cleo3/database/data17/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 122264 122273
go

module sel DBModule
database in /nfs/cleo3/database/data17/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 122418 122434
go

module sel DBModule
database in /nfs/cleo3/database/data17/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 122585 122606
go

module sel DBModule
database in /nfs/cleo3/database/data17/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 122783 122810
go

module sel DBModule
database in /nfs/cleo3/database/data17/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 123011 123019
go

module sel DBModule
database in /nfs/cleo3/database/data18/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 123369 123592
go

module sel DBModule
database in /nfs/cleo3/database/data18/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 123767 123775
go

module sel DBModule
database in /nfs/cleo3/database/data18/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 123793 123820
go

module sel DBModule
database in /nfs/cleo3/database/data18/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 123938 123962
go

module sel DBModule
database in /nfs/cleo3/database/data18/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 124113 124141
go

module sel DBModule
database in /nfs/cleo3/database/data18/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 124299 124326
go

module sel DBModule
database in /nfs/cleo3/database/data18/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 124454 124477
go

module sel DBModule
database in /nfs/cleo3/database/data19/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 124644 124668
go

module sel DBModule
database in /nfs/cleo3/database/data19/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 124807 124820
go

module sel DBModule
database in /nfs/cleo3/database/data19/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 125158 125178
go

module sel DBModule
database in /nfs/cleo3/database/data19/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 125297
go

module sel DBModule
database in /nfs/cleo3/database/data19/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 125315
go

module sel DBModule
database in /nfs/cleo3/database/data20/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 125422 125435
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 126252 126274
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 126618 126625
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 126641 126647
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 126992 127006
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 127017 127034
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 127127 127141
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 127180 127199
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 126648 126673
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 126832 126856
go

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 127417 127444
go

module sel DBModule
database in /nfs/cleo3/database/data22/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 127619 127723
go

module sel DBModule
database in /nfs/cleo3/database/data22/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 127950 127955
go

module sel DBModule
database in /nfs/cleo3/database/data22/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 128086 128111
go

module sel DBModule
database in /nfs/cleo3/database/data22/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 128257 128269
go

module sel DBModule
database in /nfs/cleo3/database/data22/db event startrun beginrun endrun
database read_collection pass2 hadron
database run 128285 128298
go

