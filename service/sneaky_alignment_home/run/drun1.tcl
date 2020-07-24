default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/dsneak1.rzn
hbook init

run_file $env(C3_SCRIPTS)/runOnPass2.tcl
run_file $env(C3_SCRIPTS)/BeamSpot.tcl
run_file $env(C3_SCRIPTS)/CcHotList.tcl

prod sel DBEventHeaderProd
prod sel CesrBeamEnergyProd

prod sel TrkmanProd
prod sel TrkmanSelectionProd

proc sel ../build_SunOS/shlib/DSneak.so

sink_format sel PDSSinkFormat
file out ../rzn/dsneak1.pds {event{DBTrackerValues FATable<TRTrack> FATable<TRHelixPionFit> FATable<TRPionQuality> FATable<TRSeedTrackQuality> FATable<TRSeedTrack> FATable<DoitSeedQuality> FATable<VXFitVeeKShort> FATable<CcBasicShowerAttsArg> TrackShowerLattice FATable<CcConRegAttributes> CcConRegShowerLattice EventProperties DBEventHeader EventType} startrun{BeamEnergy} beginrun{BeamSpot}}

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
