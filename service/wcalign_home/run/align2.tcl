run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/exclude_constants_streams.tcl
exclude_constants_streams {drweight2layerdriftentang}
emptysource def dralignment dralignment

prod sel ChisqFitProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel KalmanProd
param DoitProd UseZD false
prod sel AZDGeomProd
prod sel ZDGeomProd
prod sel CalibratedZDHitProd
prod sel ZDDriftFunctionProd
prod sel ZDHitCorrectorProd
prod sel CesrBeamEnergyProd
prod sel DBEventHeaderProd

source_format sel DRWeight2LayerDriftEntAngFileSourceFormat
file in deweight_axial.drweight2layerdriftentang

param DetectorConfigurationProd Override true;
param DetectorConfigurationProd UseZDnotSI true;
param DRHitCorrectorProd DisableAllCorrections false
param DRHitCorrectorProd ApplyStandardCorrections true
param DRHitCorrectorProd ApplyEntranceAngleCorr true
param DRHitCorrectorProd ApplyPulseHeightCorr true
param DRHitCorrectorProd ApplySignalPropagationCorr true
param DRHitCorrectorProd ApplyTimeOfFlightCorr true

proc sel ../build_SunOS/shlib/DRAlignmentBhabhaFilterProc

source_format sel BinarySourceFormat
file in /cdat/linux/tem/drcal/cleoc/run200700.bin event beginrun startrun endrun

sink_format sel PDSSinkFormat
file out ../pds/better.pds {event{DBEventHeader DBTrackerValues FATable<TRTrack> FATable<TRHelixPionFit> SeedTrackDRHitLattice FATable<CalibratedDRHit>}}

go 200
