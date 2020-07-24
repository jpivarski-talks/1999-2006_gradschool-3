default prompt off
exception continueEventLoop on

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingMCFull.tcl
prod desel KalmanProd

prod sel ChisqFitProd
param ChisqFitProd ForceSVRFittingWeight true 
param ChisqFitProd ForceSVZFittingWeight true
param ChisqFitProd SVRFittingWeight 1000.
param ChisqFitProd SVZFittingWeight 1000.

prod sel MuonFromPionFitProd
prod sel DualTrackHelicesProd
prod sel DBEventHeaderProd

source_format sel ADRGeomLayerFileSourceFormat
file in random100.adrgeomlayer

source_format sel PDSSourceFormat
source create pdschain
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122274-E5000-DTS15102002_091613-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122275-E5000-DTS15102002_091624-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122276-E5000-DTS15102002_091637-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122277-E5000-DTS15102002_091648-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122278-E5000-DTS15102002_091700-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122279-E5000-DTS15102002_091712-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122280-E5000-DTS15102002_091725-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122281-E5000-DTS15102002_091739-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122283-E5000-DTS15102002_091752-cg3.pds event beginrun startrun endrun physics
file add pdschain /cdat/sol221/disk4/c3mc/data17/mumu/diagnostic/cleogoutput/mumu-Jun27_02_MC-FL-1-R122285-E5000-DTS15102002_091804-cg3.pds event beginrun startrun endrun physics
source act pdschain event beginrun startrun endrun physics

proc sel ../build_SunOS/shlib/PDSFilter

sink_format sel PDSSinkFormat
file sink ../rzn/mc100.pds {event {FATable<TRTrack> FATable<TRSeedTrack> FATable<TRSeedTrackQuality> FATable<DoitSeedQuality> FATable<TRHelixPionFit> FATable<TRPionQuality> SeedTrackDRHitLattice SeedTrackSVRHitLattice SeedTrackSVZHitLattice PionFitDRHitLattice PionFitSVRHitLattice PionFitSVZHitLattice FATable<CalibratedSVRphiHit> FATable<CalibratedSVZHit> FATable<DualTrackHelices>}}

go
exit
