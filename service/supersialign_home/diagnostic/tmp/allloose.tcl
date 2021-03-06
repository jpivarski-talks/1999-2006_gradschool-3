
default prompt off
exception continueEventLoop on

module sel LoadHbook4MModule
module sel HbookHistogramModule
hbook file ../rzn/allloose.rzn
hbook init

run_file $env(C3_SCRIPTS)/getConstants.tcl

source_format sel SVResolutionGlobalFileSourceFormat
file in data16.svresolutionglobal
source_format sel ../build_$env(OS_NAME)/shlib/SVResolutionLayerFileSourceFormat
file in data16_allloose.svresolutionlayer

set pdsfile ../pds/data16/allloose.pds
run_file loadraw.tcl

proc sel ../build_$env(OS_NAME)/shlib/AlignmentPlots

param AlignmentPlots useKalmanResids false
param AlignmentPlots useMuPairs false
param AlignmentPlots useDualTracks true
param AlignmentPlots beamEnergy 5.1794
param AlignmentPlots weightHits false

param AlignmentPlots rphiCut 0.000200
param AlignmentPlots zCut 0.0015

param AlignmentPlots requiredRphiLayers 1
param AlignmentPlots requiredZLayers 1

param AlignmentPlots fillSuperNtuple true
param AlignmentPlots fillRphiHitNtuple false
param AlignmentPlots fillZHitNtuple false
param AlignmentPlots fillMatchedNtuple false
param AlignmentPlots fillRphiOverlapNtuple false
param AlignmentPlots fillZOverlapNtuple false

report level NOTICE
go 50000

hbook terminate
exit

