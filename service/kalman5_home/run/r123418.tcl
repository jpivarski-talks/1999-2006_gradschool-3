default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl

prod desel KalmanProd

prod sel DBEventHeaderProd

source_format sel BinarySourceFormat
file source /cdat/sol191/disk1/cleo3/data/r123418.bin event beginrun startrun endrun

sink_format sel PDSSinkFormat
file sink ../pds/r123418.pds {event {SeedTrackCathodeHitLattice SeedTrackDRHitLattice SeedTrackSVRHitLattice SeedTrackSVZHitLattice FATable<CalibratedCathodeHit> FATable<CalibratedDRHit> FATable<CalibratedSVRphiHit> FATable<CalibratedSVZHit> FATable<TRSeedTrack> FATable<TRSeedTrackQuality>}}

go 100000
exit
