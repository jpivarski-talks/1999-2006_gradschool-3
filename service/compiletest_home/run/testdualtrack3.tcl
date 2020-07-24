module sel HbookHistogramModule
hbook file stuff3.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl

prod sel ChisqFitProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel KalmanProd

prod sel AZDGeomProd
prod sel ZDGeomProd
prod sel CalibratedZDHitProd
prod sel ZDDriftFunctionProd
prod sel ZDHitCorrectorProd
prod sel CesrBeamEnergyProd
prod sel TrackShowerMatchingProd
prod sel DBEventHeaderProd

param DetectorConfigurationProd Override true
param DetectorConfigurationProd UseZDnotSI true
param DRHitCorrectorProd ApplyEntranceAngleCorr true
param DRHitCorrectorProd ApplyStandardCorrections true

source_format sel BinarySourceFormat
file in /cdat/sol191/disk1/cleo-c/r202031.bin event beginrun startrun endrun

prod sel ../build_SunOS/shlib/DualTrackProd
prod sel ../build_SunOS/shlib/DualTrackToUsageTagProd

proc sel ../build_SunOS/shlib/TestDualTrackProc
go 1


