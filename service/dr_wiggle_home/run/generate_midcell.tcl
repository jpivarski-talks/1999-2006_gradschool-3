default prompt off
exception continueEventLoop on

module sel CorbaModule
corba init
module sel ConstantsModule
constants in $env(C3_CONST) meta Offline MetaListRetrieval streams dralignment svalignment svcalibration drdaqcellidmap drwirequality drvarious drdriftdata drdriftdatav2 drgarfield_distancecorrection drgarfield_timecorrection drweightdrift drweightlayer drweighttheta drweightphi dradc2charge dradcpedestal drt0global drt0crate drt0card drt0hvcard drt0channel drt0channelv2 drtdc2time drelecmap dradcsaturation drcapedestal drcagain svresolution svradcorrection
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta SVHybridEfficiency MetaListRetrieval
constants in $env(C3_CONST) meta FieldMap MetaListRetrieval
constants in $env(C3_CONST) meta MagFudge MetaListRetrieval

source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

module sel RunStatisticsSourceModule
runstatistics in

source_format sel DRWeightDriftFileSourceFormat
file in midcell.drweightdrift

run_file $env(C3_SCRIPTS)/trackingDataFull.tcl
prod desel KalmanProd

prod sel ChisqFitProd
param ChisqFitProd ForceSVRFittingWeight true 
param ChisqFitProd ForceSVZFittingWeight true
param ChisqFitProd SVRFittingWeight 1000.
param ChisqFitProd SVZFittingWeight 1000.

prod sel MuonFromPionFitProd
prod sel DualTrackHelicesProd
prod sel DBEventHeaderProd

module sel DBModule
database in /nfs/cleo3/database/data21/db event startrun beginrun endrun
database read_collection rawdata bhabha
database run $env(USER_RUNSTART) $env(USER_RUNEND)

proc sel ../build_SunOS/shlib/PDSFilter

sink_format sel PDSSinkFormat
file sink ../rzn/midcell$env(USER_N).pds {event {FATable<TRTrack> FATable<TRHelixPionFit> PionFitDRHitLattice PionFitSVRHitLattice PionFitSVZHitLattice FATable<CalibratedSVRphiHit> FATable<CalibratedSVZHit> FATable<DualTrackHelices>}}

go
exit
