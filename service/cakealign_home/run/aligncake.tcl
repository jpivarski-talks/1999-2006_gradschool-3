module sel HbookHistogramModule
hbook file aligncake.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/exclude_constants_streams.tcl
exclude_constants_streams {dralignment drweight2layerdriftentang}

emptysource def drweight2layerdriftentang drweight2layerdriftentang
emptysource def dralignment dralignment

run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod sel AZDGeomProd
prod sel ZDGeomProd
prod sel CalibratedZDHitProd
prod sel ZDDriftFunctionProd
prod sel ZDHitCorrectorProd
prod sel CesrBeamEnergyProd

run_file $env(C3_SCRIPTS)/CcP2.tcl

param DetectorConfigurationProd Override true;
param DetectorConfigurationProd UseZDnotSI true;
param KalmanProd HyposToFit pion
param KalmanProd UseZDStereoU false
param KalmanProd UseZDStereoV false

source_format sel BinarySourceFormat
file in /cdat/sol191/disk1/cleo-c/r200558.bin event beginrun startrun endrun

report level NOTICE
module sel ../build_SunOS/shlib/CakeAlignMod
param CakeAlignMod drfw cleo3.drweight2layerdriftentang
param CakeAlignMod geom cleo3.drgeomalignment
param CakeAlignMod events 50000
param CakeAlignMod showPlots false
iterator go

exit
