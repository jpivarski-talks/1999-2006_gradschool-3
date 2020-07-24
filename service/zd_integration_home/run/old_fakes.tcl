run_file $env(C3_SCRIPTS)/getNewestConstants.tcl

# module sel CorbaModule
# corba init
# module sel ConstantsModule
# constants in $env(C3_CONST) meta Offline MetaListRetrieval streams dralignment svradcorrection
# constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
# constants in $env(C3_CONST) meta MagFudge MetaListRetrieval
# constants in $env(C3_CONST) meta FieldMap MetaListRetrieval
# source_format sel AsciiSourceFormat
# file in $env(C3_DATA)/material.asc basegeometry
# source_format sel ../build_OSF1/shlib/AZDGeomLayerFileSourceFormat
# file in $env(C3_CVSSRC)/AZDGeomLayerFileSourceFormat/zd.azdgeomlayer
# source_format sel ../build_OSF1/shlib/AZDGeomLayersEndsFileSourceFormat
# file in $env(C3_CVSSRC)/AZDGeomLayersEndsFileSourceFormat/zd.azdgeomlayersends

# prod sel ../build_OSF1/shlib/DetectorConfigurationProd
# # param DetectorConfigurationProd Override true;
# # param DetectorConfigurationProd UseZDnotSI true;
# prod sel MagFieldProd
# param MagFieldProd alwaysUseDefault true
# prod sel ADRGeomProd
# prod sel DRGeom
# # prod sel DRWireLayerProd
# prod sel DRHitCorrectorProd
# prod sel ASiStorePro
# prod sel SiGeom
prod sel ../build_OSF1/shlib/AZDGeomProd
prod sel ../build_OSF1/shlib/ZDGeomProd
# # prod sel ZDWireLayerProd
prod sel ../build_OSF1/shlib/FakeATrackProd

run_file $env(C3_SCRIPTS)/trackingDataFull.tcl

prod sel DBEventHeaderProd

prod sel ../build_OSF1/shlib/KalmanProd
param KalmanProd CorrectDRHits false
param KalmanProd UseSVRPhi false
param KalmanProd UseSVZ false
param KalmanProd UseDRCathode false
# param KalmanProd PrintDiagnostics true

proc sel ../build_OSF1/shlib/ExtractFakes

# emptysource def emptyevent event
source_format sel BinarySourceFormat
file in /cdat/sol191/disk2/cleo3/data/r132153.bin event startrun beginrun endrun

# report level DEBUG
go 1
