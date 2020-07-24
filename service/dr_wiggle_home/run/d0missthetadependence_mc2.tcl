default prompt off
exception continueEventLoop on

module sel HbookHistogramModule
hbook file ../rzn/d0missthetadependence_mc2.rzn
hbook init

run_file $env(C3_SCRIPTS)/getNewestConstants.tcl
run_file $env(C3_SCRIPTS)/trackingMCFull.tcl
prod desel KalmanProd

prod sel ChisqFitProd
param ChisqFitProd ForceSVRFittingWeight true 
param ChisqFitProd ForceSVZFittingWeight true
param ChisqFitProd SVRFittingWeight 1000.
param ChisqFitProd SVZFittingWeight 1000.
param ChisqFitProd UseDRAxial false

prod sel CustomBeamEnergyProd
param CustomBeamEnergyProd BeamEnergy 5.29

source_format sel BeamSpotFileSourceFormat
file in $env(C3_INFO)/mc/zeromc.beamspot beginrun

proc sel ../build_SunOS/shlib/D0MissThetaDependence

source_format sel PDSSourceFormat
source create pdschain

file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/diagnostics/cleogoutput/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112802-E3387-cleog.pds event startrun
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/diagnostics/cleogoutput/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112803-E5539-cleog.pds event startrun
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/diagnostics/cleogoutput/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112804-E4832-cleog.pds event startrun
file add pdschain /cdat/sol221/disk3/c3mc/data7/bhabha/diagnostics/cleogoutput/bhabha-cleo3_MCPS2_Jan30_2002-MN-1-R112806-E4977-cleog.pds event startrun

go

hbook terminate
exit
