default prompt off

emptysource def Test2 physics

# Load tracking packages for running on data with full Doit/Kalman
run_file $env(C3_SCRIPTS)/trackingMCFull.tcl
prod desel ElectronFromPionFitProd
prod desel MuonFromPionFitProd

#source_format sel QQDriverSourceFormat
source_format sel BinarySourceFormat
#source_format sel PDSSourceFormat

#file in /cdat/tem/wsun/bhabha.bin
#file in /home/wsun/c3lib/run/qq/bhabha.rp
#file in /cdat/tem/wsun/pions.bin
#file in /cdat/lnsct3/disk3/wsun/pions.rp
#file in /cdat/solaris2/tem/wsun/cleog_bb_1000.bin
#file in /cdat/lnsct3/disk3/wsun/bb_1000.rp
#file in /cdat/lnsro4/disk1/hennessy/cleo3/scripts/genrp/bha7116044.asc
#file in /home/wsun/c3lib/run/qq/bhabha.rp
#file in /cdat/dafa/mahlke/gensim/bhabha/bhabha_116045_5k.pds

file in /cdat/dafc/pds/b+.rho0pi+.anything_5000_final.p2.pds event
file in /cdat/dafc/signal_cleog/B.RHOPI/qq_b+.rho0pi+_5000_final.cg.rp

#file in $env(C3_DATA)/MC/cleog_bb_50.pds event
#file in $env(C3_DATA)/MC/qq_cleog3_bb_1000.rp

#param KalmanProd UseSVRPhi false
#param KalmanProd UseSVZ false
#param KalmanProd ApplyDedx false
#param KalmanProd ApplyStraggling false
#param KalmanProd UseDRAxial false
param KalmanProd OptimizeForAllHypos false
#param KalmanProd HitListFromOneHypo false
#param KalmanProd HyposToFit kaon electron
#param KalmanProd HitListHypo electron

#prod sel ChisqFitProd
#param ChisqFitProd UseSVRPhi false
#param ChisqFitProd UseSVZ false
#param ChisqFitProd UseDRAxial false
#param ChisqFitProd UseDRStereoU false
#param ChisqFitProd UseDRStereoV false
#param ChisqFitProd UseDRCathode false

#prod sel C3trProd

prod sel MCInfoDelivery
#param MCInfoDelivery userDecayDec /home/wsun/mc/dec/pions.dec
param MCInfoDelivery userDecayDec /cdat/dafc/signal_cleog/B.RHOPI/b+.rho0pi+.dec

prod sel MCtoTrack
prod inter MCtoTrack
production_tag MC
exit
proc sel FastMCTaggerProc
#param FastMCTaggerProc MaxFigureOfMerit 30.

proc sel SeedDiagProc
#proc sel FitDiagProc
#param FitDiagProc FitHypo electron
#proc sel KalmanDedxProc

# Load constants
run_file $env(C3_SCRIPTS)/getConstants.tcl

#proc sel SpExtractMCDecayTreeProc.so
#run_file $env(C3_SCRIPTS)/viewTrackFinder.tcl

#param KalmanProd PrintDiagnostics true
#report level debug
go 1
#goto 116042 177
#go
exit
