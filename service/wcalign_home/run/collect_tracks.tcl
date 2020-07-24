default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../pds/run$env(USER_RUN).rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/exclude_constants_streams.tcl
exclude_constants_streams {drdriftdatav2 drt0card drt0channelv2 drt0crate drt0global drt0hvcard drgarfield_distancecorrection drweight2layerdriftentang}

source_format sel DRGarfield_DistanceCorrectionFSF
file in /cdat/lnssb2/disk2/dubrovin/cleo3/test_tuning/build/EA_COR/d130016.drgarfield_distancecorrection

source_format sel DRDriftDatav2FileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRDriftDatav2.drdriftdatav2
source_format sel DRt0cardFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0card.drt0card
source_format sel DRt0channelv2FileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0channelv2.drt0channelv2
source_format sel DRt0crateFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0crate.drt0crate
source_format sel DRt0globalFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0global.drt0global
source_format sel DRt0hvcardFileSourceFormat
file in /cdat/sol406/disk1/drcal/Constants/DRt0hvcard.drt0hvcard

source_format sel DRWeight2LayerDriftEntAngFileSourceFormat
file in deweight_axial.drweight2layerdriftentang

prod sel ChisqFitProd
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel KalmanProd

prod sel AZDGeomProd
prod sel ZDGeomProd
prod sel CalibratedZDHitProd
prod sel ZDDriftFunctionProd
prod sel ZDHitCorrectorProd
prod sel CesrBeamEnergyProd
prod sel DBEventHeaderProd

param DetectorConfigurationProd Override true
param DetectorConfigurationProd UseZDnotSI true
param DRHitCorrectorProd ApplyEntranceAngleCorr true
param DRHitCorrectorProd ApplyStandardCorrections true
param DoitProd UseZD false
param ChisqFitProd HyposToFit pion
param ChisqFitProd UseZDStereoU false
param ChisqFitProd UseZDStereoV false

source_format sel BinarySourceFormat
file in /cdat/linux/tem/drcal/cleoc/run$env(USER_RUN).bin event beginrun startrun endrun

sink_format sel PDSSinkFormat
file out ../pds/run$env(USER_RUN).pds {event{DBEventHeader DBTrackerValues FATable<TRTrack> FATable<TRHelixPionFit> FATable<TRPionQuality> FATable<TRSeedTrack> FATable<TRSeedTrackQuality> SeedTrackDRHitLattice PionFitDRHitLattice FATable<CalibratedDRHit> BunchDecision FATable<CollatedRawDRHit>}}

proc sel ../build_SunOS/shlib/DRAlignmentBhabhaFilterProc
go
hbook terminate
exit
