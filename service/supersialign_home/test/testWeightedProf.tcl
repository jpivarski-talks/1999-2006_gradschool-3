module sel ../build_OSF1/shlib/HbookHistogramModule
hbook file test.rzn
hbook init

#  module sel ../build_OSF1/shlib/RootHistogramModule
#  root file test.root
#  root init

source_format sel PDSSourceFormat
file in $env(USER_DAFE)/sialignment/loose_tracks/dual_chi_newalignment_loose1.pds event

prod sel NavigationProd
prod sel TrackDeliveryProd
prod sel SeedToTRTrackProd

proc sel ../build_OSF1/shlib/TestWeightedProf
# proc sel HistogramViewerProc

go 10

