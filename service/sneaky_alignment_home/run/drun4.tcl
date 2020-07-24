default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/dsneak4.rzn
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
file out ../rzn/dsneak4.pds {event{DBTrackerValues FATable<TRTrack> FATable<TRHelixPionFit> FATable<TRPionQuality> FATable<TRSeedTrackQuality> FATable<TRSeedTrack> FATable<DoitSeedQuality> FATable<VXFitVeeKShort> FATable<CcBasicShowerAttsArg> TrackShowerLattice FATable<CcConRegAttributes> CcConRegShowerLattice EventProperties DBEventHeader EventType} startrun{BeamEnergy} beginrun{BeamSpot}}

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
