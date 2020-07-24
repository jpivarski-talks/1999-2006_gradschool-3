default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/sneaky3.rzn
hbook init

run_file $env(C3_SCRIPTS)/runOnPass2.tcl
run_file $env(C3_SCRIPTS)/BeamSpot.tcl
run_file $env(C3_SCRIPTS)/CcHotList.tcl

prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd

prod sel TrkmanProd
prod sel TrkmanSelectionProd

proc sel ../build_SunOS/shlib/Sneaky.so

sink_format sel PDSSinkFormat
file out ../rzn/sneaky3.pds {event{DBTrackerValues FATable<TRTrack> FATable<TRHelixPionFit> FATable<TRPionQuality> FATable<TRSeedTrackQuality> FATable<TRSeedTrack> FATable<DoitSeedQuality> FATable<VXFitVeeKShort> FATable<CcBasicShowerAttsArg> TrackShowerLattice FATable<CcConRegAttributes> CcConRegShowerLattice EventProperties DBEventHeader EventType} startrun{BeamEnergy} beginrun{BeamSpot}}

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
