# This is the "standard" script for tracking on MC. It will be updated 
# with the latest and greatest tracking improvements as they become 
# available. Please see the "Cleo III Tracking Status" web page,
# available from the Cleo III Software web page for the lastest
# tracking news. Updates to this script will be detailed there.
#
# NOTE: Do not use the word "E_rror" in this script. The test scripts
#       will catch it and fail
#
# This script now only loads tracking packages. No constants 
# (except for special ones necessary), no event 
#  selection, no data files. See sampleTrackingOnData.tcl for 
#  a full script that calls this one (but it runs on data).
#
# Load this script with...
#
#    Suez> run_file $env(C3_SCRIPTS)/trackingMCFull.tcl
#
# This script will *not* work on real data. See trackingDataFull.tcl
#    for a script that will work on real data.
#
#   This script does full tracking (Doit pattern recognition and Kalman
#   fitting) and makes NavTracks.
#
#  Questions should go to Adam Lyon (lyon@mail.lns.cornell.edu)
#
# $Id: trackingMCFull.tcl,v 1.16 2002/04/04 15:40:46 bkh Exp $
# $Log: trackingMCFull.tcl,v $
# Revision 1.16  2002/04/04 15:40:46  bkh
# Alter for accomodation of trigger phase producers
#
# Revision 1.15  2002/02/11 22:22:51  bkh
# Various adjustments including controlling writeout of cleog,
# restoration of control writing out mcpass2, removal of
# TrackletBunchFinder from MC tracking, and other housekeeping changes.
#
# Revision 1.14  2002/02/06 23:19:53  bkh
# Fix ups
#
# Revision 1.13  2002/01/24 23:45:17  bkh
# set cathodes fake merge flag false
# deselect PreliminaryDRHitProd just to be safe in trackingMC
#
# Revision 1.12  2002/01/24 16:04:40  mahlke
# deselect DRCACorrectHitProd, CorrectedSVStripProd, SiHitsProd also
#
# Revision 1.11  2002/01/24 14:58:05  lyon
# Deselects CalibratedDRHitProd (hits are already added by geant
#
# Revision 1.10  2001/11/27 14:26:07  bkh
# Add PreliminaryDRHitProd and CorrectedSVStripProd to trackingDataFull.tcl
# Make MC tracking tcl file driven off of data tracking tcl file
#
# Revision 1.9  2001/10/12 17:56:10  mahlke
# add in KalmanProd HitListFromOneHypo true parameter, Scale Cathode Resolution by 0.7 in KalmanProd as per hennessy, turn off entrance angle corrections in DRHitCorrectorProd
#
# Revision 1.8  2001/09/17 15:09:55  cdj
# Added Prod to give Electron and Muon from the Pion fits
#
# Revision 1.7  2001/05/08 22:35:42  lyon
# removed CDOffCal parameters
#
# Revision 1.6  2001/03/29 17:46:25  ajm36
# clean out old scripts, get rid of drcalibration emptysources
#
# Revision 1.5  2000/11/16 03:13:01  lkg
# Changed MagField to MagFieldProd
#
# Revision 1.4  2000/11/01 15:27:58  lyon
# Removed prod sel CESR -- no longer needed
#
# Revision 1.3  2000/10/26 14:47:49  lyon
# Try to remove the word E_rror from the scripts so test scripts will not catch it
#
# Revision 1.2  2000/06/30 10:39:29  lkg
# - commented out special DR Cathode pedestal file, pedestals now used in MC
# - told KalmanProd to UseSVZ hits (UseRPH) was already specified
#
# Revision 1.1  2000/06/22 15:06:28  lyon
# Restructured tracking scripts
#
#
#
#

if { ! [info exists trackingMCFull_tcl_loaded] } {
# keep script from multiply loading
    set trackingMCFull_tcl_loaded 1

# Handle "unintentional features" gracefully (don't say E_rror)
    if [ catch { 
 
	run_file trackingDataFull_GuangShun.tcl

#       Deselect the following producers as their hits are 
#       produced by CLEOG:
	prod desel PreliminaryDRHitProd
	prod desel CalibratedDRHitProd
        prod desel DRCACorrectHitProd
#        prod desel CorrectedSVStripProd
#        prod desel SiHitsProd
	prod desel TrackletBunchFinder

	prod desel TriggerPhaseForDataProd

# MC sometimes (when from rp file) has no bunch structure

#	param TrackletBunchFinder ForceTimeOffset true
#	param TrackletBunchFinder TheForcedTimeOffset 0.0


    } resultString ] { # ------------------- tcl try-catch block
	puts stdout "loading failed for a tracking Producer:"
	puts stdout "resultString:"
	puts stderr "$resultString"
    } else {
	puts stdout "Tracking producers loaded successfully"
    }

}

