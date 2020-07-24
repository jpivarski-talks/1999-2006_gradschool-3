module sel LoadHbook4MModule
module sel HbookHistogramModule
hbook file ../dafedir/standalone_kalman_124543.rzn
hbook init

# Load constants
run_file $env(C3_SCRIPTS)/getConstants.tcl

default prompt off
exception continueEventLoop on

# Load tracking packages for running on data with full Doit/Kalman
run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel ElectronFromPionFitProd
prod desel MuonFromPionFitProd
param KalmanProd OptimizeForAllHypos false

prod sel CesrBeamEnergyProd

# override producers in trackingDataFull.tcl
# prod sel ChisqFitProd
#param ChisqFitProd ContinueIfAnyException true

prod sel DBEventHeaderProd

# Load CC stuff for event selection
prod sel CcfcReconProd
prod sel C3ccReconProd
proc sel CcEventSelectProc
prod sel C3ccProd
prod sel CcGeomProd
prod sel C3ccConProd

# What kind of events do you want?
param CcEventSelectProc BhabhaKeep true
#param CcEventSelectProc MuPairKeep true
#param CcEventSelectProc HadronKeep true

# Load processors
proc sel SeedDiagProc
proc sel FitDiagProc
param FitDiagProc FitHypo electron
#param FitDiagProc BeamSpotX -0.000741
#param FitDiagProc BeamSpotY  0.000097

# !!!!!!! Load in the file here !!!!!!!
source_format sel BinarySourceFormat
# file in /home/wsun/c3lib/run/data/124154/124154.bin beginrun startrun event endrun
# file in cont_123592.bin beginrun startrun event endrun
file in run_124543.bin beginrun startrun event endrun

#  module sel DBModule
#  database in /nfs/cleo3/database/data16/db event beginrun startrun endrun
#  database read_collection rawdata bhabha
#  source activate db event beginrun startrun endrun
#  database run 121339
# database run 113315

#proc sel SpExtractDBEventHeaderProc.so
#run_file $env(C3_SCRIPTS)/viewTrackFinder.tcl

#goto 109977 44597
#go 2
#go 1
#report level debug
#param KalmanProd PrintDiagnostics true
report level INFO
go 10000
hbook terminate
exit
