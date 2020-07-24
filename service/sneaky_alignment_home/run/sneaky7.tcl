default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/sneaky7.rzn
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
file out ../rzn/sneaky7.pds {event{DBTrackerValues FATable<TRTrack> FATable<TRHelixPionFit> FATable<TRPionQuality> FATable<TRSeedTrackQuality> FATable<TRSeedTrack> FATable<DoitSeedQuality> FATable<VXFitVeeKShort> FATable<CcBasicShowerAttsArg> TrackShowerLattice FATable<CcConRegAttributes> CcConRegShowerLattice EventProperties DBEventHeader EventType} startrun{BeamEnergy} beginrun{BeamSpot}}

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

