default prompt off
report level INFO

# database and metafiles -------
module sel CorbaModule
corba init
module sel ConstantsModule
# constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams drdriftdatav2 dralignment
constants in $env(C3_CONST) meta Offline MetaListRetrieval exclude_streams dralignment

constants in $env(C3_CONST) meta Online MetaListRetrieval
constants in $env(C3_CONST) meta CesrOffset MetaListRetrieval
constants in $env(C3_CONST) meta BaseGeometry MetaListRetrieval
constants in $env(C3_CONST) meta RunByRun MetaListRetrieval

emptysource def dralignment dralignment

# Get material properties-------
source_format sel AsciiSourceFormat
file in $env(C3_DATA)/material.asc basegeometry

# set up runStatistics access
module sel RunStatisticsSourceModule
runstatistics in

# load mu constants producer
prod select MuConsProd

#  # this calibration was done on the stereo layers only. The axial data
#  # in this file comes from the 119033 iter3 calibration, integrated
#  # bins only (0-140)
#  source_format sel DRDriftDatav2FileSourceFormat
#  file in /cdat/sol406/disk1/drcal/main/cake/asciifiles/patch_axial.drdriftdatav2

# exclude_streams: alignedgeometry drcalibration
#  source_format sel ADRGeomLayerFileSourceFormat
#  file in $env(C3_DATA)/dr3.adrgeomlayer
#  source_format sel ADRGeomWireSurveyFileSourceFormat
#  file in $env(C3_DATA)/dr3.adrgeomwiresurvey
#  #  source_format sel DRGeomAlignmentFileSourceFormat
#  #  file in $env(C3_DATA)/test.drgeomalignment
#  source_format sel DRAlignedConstantsFileSourceFormat
#  file in $env(C3_DATA)/test.dralignedconstants

prod sel DBEventHeaderProd
prod sel MagFieldProd
param MagFieldProd alwaysUseDefault true
prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

prod sel ADRGeomProd
prod sel DRGeom
prod sel DRWireLayerProd
prod sel DRHitCorrectorProd
prod sel CDOffCal

module sel $env(USER_BUILD)/shlib/DRWeddingCakeAlignment
param DRWeddingCakeAlignment minhits 10000
# param DRWeddingCakeAlignment minhits 100
param DRWeddingCakeAlignment inputConst unaligned.drgeomalignment

source_format sel PDSSourceFormat
file in /cdat/sol404/disk1/cakeAlign/normaldrift_119590.pds
source activate normaldrift_119590 beginrun startrun event
