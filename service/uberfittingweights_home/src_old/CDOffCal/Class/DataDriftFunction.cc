// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      DataDriftFunction
// 
// Author:      Martin Lohner, Ken McLean
// Created:     Wed Mar  4 14:16:52 EST 1998
// $Id: DataDriftFunction.cc,v 1.19 2002/11/21 21:04:34 cleo3 Exp $
//
// Revision history
//
// $Log: DataDriftFunction.cc,v $
// Revision 1.19  2002/11/21 21:04:34  cleo3
// changed fstream.h to C++Std/fstream.h
//
// Revision 1.18  2002/08/16 14:03:02  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.17  2001/10/16 19:47:01  lyon
// Fix typo "dangle=dangle-dangle -> dangle=dangle-angleBin"
//
// Revision 1.16  2000/12/02 02:20:59  urner
// changed return Value to type Time rather than Drift in charge2TimeCorr
//
// Revision 1.15  2000/12/01 14:40:29  urner
// included time2charge correction to take care of time slewing using T0global BDL at present as container for the correction
//
// Revision 1.14  2000/11/14 02:36:52  mclean
// Well, I was confused, the parity/symmetry/timelike/spacelike nature
// of the correction was fine, the theory now is the overall sign of
// the damned thing is reversed. Now I flee the country.
//
// Revision 1.13  2000/11/14 00:52:57  mclean
// Corrected entrance angle correction from timelike to spacelike (the
// Garfield data has a sign convention where the correction for ambiguity<0 is the
// opposite sign than the correcton for ambiguity >0, i.e. the correction
// is meant to be added to the unsigned drift, whereas DriftFunction.cc was
// adding it to the signed drift).
//
// Revision 1.12  2000/10/31 23:29:58  mclean
// Many changes:
//
// Added BDLs:
//     DBDRvarious.h     : list of scalar constants to replace parameters in code
//                         [1] is signal propagation beta
//                         [2] will be probably be chamber-average resolution
//                         etc. as needed
//     DBDRDriftDatav2.h : corrected Drift Function BDL to allow use
//                         of two-sided drift functions if available
//     (modified DriftFunctionProxy.cc, DFDataHolder.h)
//
// Added hierarchy of signal propagation correction value decision:
//     1) if the user parameter is set to a good value (0-1) this is taken
//     2) if the user parameter is not set (the parameter defaults to a
//        negative value) the value from DRvarious[1] is taken
//     3) if DRvarious is not available a true default value (0.5) is
//        taken. On CDJ's advice if this happens an exception is thrown
//        from DriftFunctionProxy
//     4) Added functions
//         SignalPropBeta_Set()   sets the value returned
//         SignalPropBeta_Val()   gets the value returned
//         SignalPropBeta_Par()   gets the current value of the user parameter
//         kdefault_SignalProp()  "default value" (unused)
//
// Added hierarchy of constants sources for Drift Functions:
//     1) DriftDatav2 and DriftData are both extracted, and exceptions
//        are caught in DriftFunctionProxy.
//     2) If both DriftDatav2 and DriftData throw exceptions,
//        DriftFunctionProxy throws an exception.
//     3) DriftDatav2's presence overrides DriftData's presence in
//        the DataDriftFunction constructor
//     4) At least one of these must be valid in the DataDriftFunction
//        constructor (redundant considering the proxy exception but...)
//     5) Note in DataDriftFunction.cc there are two almost identical blocks
//        of code that unpack DriftFunction and DriftFunctionv2.
//
// The old derived classes NaiveDriftFunction and HePrDriftFunction that
// were used in the development of the code have been deleted, all with
// all provision for quick and dirty input of constants from nonstandard
// ASCII files.
//
// DataDriftFunction should be used by everybody.
//
// For the purposes of development while maintaining code stability,
// an initally identical copy of DataDriftFunction, TestDriftFunction may be
// added.
//
// The constants expert can modify this code and select it via the
// DriftFunctionType parameter without disturbing anything else,
// and, when satisified, do a TestDriftFunction>DataDriftFunction update.
//
// The structure of the code is simpler now:
// DataDriftFunction.cc/h
//      > added three-sided driftfunctions, i.e. left,right,left+right
//        simple changes to code. The object that holds all the drift
//        function data is now an STL_VECTOR of STL_VECTORs, neat!
//      > select drift function unpack for DriftDatav2/DriftData as required
//      > added entrance angle correction unpack, with extensive cross-checks
//        due to the huge size of these constants. These are stored
//        simply in two large 4D arrays: corrDrift & corrTime.
//      > drift function, inverse drift function, simplified and added
//        some input data verification. If layer or "side" is incorrect
//        a WARNING is printed and 0 is returned. If there are no
//        left or right drift functions available, and a left or right
//        drift function is requested, then the average is returned
//        and a single WARNING is printed.
//      > entrance angle correction, inverse entrance angle correction
//        (these are bilinearly interpolated between the garfield data points
//        provided by llh14)
//      > extra member data for e-angle correction angle/time/drift bin finding
//
// DriftFunction.cc/h
//      > Several overloaded functions to access basic functionality
//        from DataDriftFunction.cc routines.
//      > These include time2SignedDrift and signeddrift2Time
//      > geometry now does not have to be provided by external routines
//        (i.e. CalibratedDRHitProd and TrackletBunchFinder) but
//        overloaded functions accepting these arguments are provided
//        for backwards compatibility.
//      > added weight unpack for Phi,Theta,Drift,Layer corrections to
//        resolution (previously only one scalar parameter). The average
//        values of each type of weight is printed out as a quick data
//        quality check.
//      > added ZCorr a function used by ZShift and HitCorrector.cc
//        to calculate the time shift due to Z offsets (this is the
//        only routine that should need access the signal propagation velocity).
//      > added "resolution" functions, these are overloaded and
//        recursively called i.e. resolution() returns the chamber
//        average resolution, resolution(layer) returns resolution()
//        multiplied by the layer correction, resolution (layer,drift)
//        returns resolution(layer) multiplied by the appropriate
//        drift correction. Each actual correction (except for layer :)
//        is linearly interpolated.
//      > extra member data for resolution correction bin finding
//      > sped up tdc2Time and inverse slightly by relying on constants
//        verification (i.e. slope non-zero) at unpack.
//      > Streamlined DebugLevel parameter use so that only member
//        data logicals are used after BOR, previously one or two
//        function calls to the Prod & logic test were necessary.
//        DebugLevel Hierarchy :
//            <0 selects that option exclusively
//            >0 selects that option and all lesser ones
//          -1/1 time2Drift,drift2Time internal debug
//          -2/2 Dumps information from makeCalibratedHit
//          -3/3 DF Constructor unpack debug
//          -4/4 DF Constructor Map data dump
//          -5/5 Dumps channel/t0/gain/slope/ped for all channels (DF constructor)
//          -6/6 time2DriftCorr,drift2TimeCorr internal debug
//      > Moved shift-of-time-to center-code to ZShift() and added this
//        to total t0 definition in order to allow consistent use automatically
//        in both tdc2Time and inverse code. Shifted R/C approximate arc length
//        correction to total t0 definition too.
//        So the total t0s are now redefined as:
//                 t0 = oldt0 - R/C + Z/(BetaC)
//        where ZShift calculates the Z to use and ZCorr accesses BetaC
//
// HePrDriftFunction.h
// NaiveDriftFunction.h
//      > there is no active code in these, they are kept just so
//        antiquated includes lurking about dont blow up. The .cc's are gone.
//
// DriftFunctionProxy.cc
//      > enabled unpack of DRWeight*
//      > added DRVarious,DRDriftDatav2 with exception catching as
//        described above
//
// Revision 1.11  2000/10/31 23:09:28  mclean
// *** empty log message ***
//
// Revision 1.10  2000/08/03 21:05:29  mclean
// Downgraded "inverse velocity has illegal value" message from ERROR to WARNING
// (this message arises when the drift function is flat or has a repeated point).
//
// This should only occur MC and is only info for calibration experts.
//
// The drift function may be flat in portions because of bad statistics or the
// effect of the magnetic field (though the latter case should be much rarer
// with DR III). Repeated points are created when the driftfunction from
// derived from a time-integral (inner cell) is combined with the driftfunction
// from average fitted distance (outer cell).
//
// Revision 1.9  2000/05/12 16:34:04  mclean
// Added t0global constant.
//
// Revision 1.8  2000/04/27 20:40:39  mclean
// The BDL of the drdriftdata object followed the CLEO II/II.V convention:
//    record#  timevalue driftvalue layer*10000000+type*1000+point
// however the last record was mistyped float instead of integer.
//
// One cannot conveniently change BDL.
//
// On writing data into the database rounding corrupts the "point" field
// since the value is never actually used we thought this was OK. However,
// on writing data from the database to ascii files and then reading it back
// in, different rounding occurs. I.E. if everything was float "type" would be
// 1+epsilon (normal drift function type), instead it is 1-epsilon which integer
// truncates to 0 (undefined drift function type, skipped in unpack).
//
// So this resulted in drift functions beyond layer 26 not being unpacked
// as they had type "0" which is not used.
//
// The fix is just not to test on the "type" field, we will probably
// not use the various subclasses of drift functions needed for DR II.
//
// Revision 1.7  2000/04/24 23:33:55  mclean
// In Singlelayer_df_data.cc
//   - Added a constructor and destructor, default values are set and a single
//     point is added to the drift function (0,0)
//   - DataDriftFunction.cc's constructor appends points, so this cannot be
//     initialized in one operation.
//   - Currently member data is accessed directly I will change this soon
// In DriftFunction.cc:
//    - default TDCQuantum -500ns/count
//    - default DriftFunctionType 2 (external constants)
//    - improved some error messages
//    - set tdc2time/time2tdc return value to zero (was undefined) if tdc slope
//      is zero and print error message.
// For DataDriftFunction.cc
//    - trivial change
//
// Revision 1.6  2000/03/13 20:33:32  mclean
// Commented out all m_statistics->counter() quality statistics monitoring
// calls. I will enable these again when I have time to implement CDJ's
// suggestions about speeding up the use of string variables.
//
// Revision 1.5  2000/02/14 23:49:02  mclean
// Added DPP Logic
// - take first hit from tight window, else
// - take first hit from wide window, else
// - take first hit from high window, else
// - take kast hit fron low window
// Where first,last are ordered as normal clock time...(increasing entropy).
//
// Added resolution(layer,drift) function for MC (just returns 123micron
// at present).
//
// Implemented ADCgain, TDCslope constants in code.
//
// Added parameters and access functions, default constants
// external name      internal name      default value (ps) access function
// MinimumTime        m_MinTightTime       -30000           getMinTightTime
// MaximumTime        m_MaxTightTime       350000           getMaxTightTime
// MinimumLooseTime   m_MinLooseTime       -50000           getMinLooseTime
// MaximumLooseTime   m_MaxLooseTime       600000           getMaxLooseTime
//
// Added new default resolution parameter (rather than weight=1/resolution**2)
// for convenience of MC.
//
// Added DriftFunction access functions for adcgain,
// adcgainRMS,tdcped,tdcpedRMS,tdcslope,tdcslopeRMS,resolution,badwiremap
//
// Added DriftFunction member data from constants:
// m_daqmap,m_quality,m_adcgain,m_adcgainRMS,m_tdcslope,m_tdcslopeRMS,m_tdcped,
// m_tdcpedRMS.
//
// HitQuality
// Added functions
//      set/test_windowNarrow,
//      set/test_windowWide,
//      set/test_windowHigh,
//      set/test_windowLow
// delete function
//      test_notinwindow
// and added/deleted corresponding static const "Flag*" bitmap variables.
//
// Revision 1.4  2000/02/02 15:37:59  cdj
// now dependent on DFDataHolder instead of DriftFunctionProxy
//
// Revision 1.3  2000/02/01 23:03:16  mclean
// This update introduces constants from the frame, i.e. from the official
// database or from CDJ FileSourceFormat ascii files as the user chooses.
//
// Guang Pei Chen also found a bug I wrote into HitQuality.cc, oops.
//
// I will add tcl scripts to the SuezScripts area as an example of each
// type of access, CDOffCal_dbase_v00.tcl CDOffCal_file_v00.tcl.
//
// I will not make loading constants from the frame default until there
// exists valid data-derived entities for all constants extracted from
// the database.
//
// The user can control where the constants come from (and other CDOffCal
// parameters) via the suez command, as reviewed below:
//   param CDOffCal DriftFunctionLevel 2
//      - This extracts constants from the frame
//      - The following BDL is expected to have existing constants,
//        either from the  data base or from ascii/FSF files :
//          DBDRadc2charge
// 	 DBDRtdc2time
// 	 DBDRDaqCellIDMap
// 	 DBDRGarfield_DistanceCorrection
// 	 DBDRGarfield_TimeCorrection
// 	 DBDRWireQuality
// 	 DBDRt0card
// 	 DBDRt0crate
//        **DBDRDriftData
// 	*DBDRadcpedestal
//        **DBDRt0channel
//        FileSourceFormat files exist for all the above in $C3_LIB/data/
//        and are loaded in /nfs/c3db/Offline/constants/Codi
//
//        Only those with (*,**) are used by this release. The others will
//        be implemented when nontrivial values are ready (this way people
//        will have to change their tcl scripts less often). Those with
//        ** are automatically used by "suez -cleog3", the adcpedestals require
//        an additional change to DRMCUtil.
//
//        Note that the crate/card t0s are summed into the channel value
//        currently.
//
//        The current default constants in FSF ascii format are in:
//                   $C3_LIB/data/test.drdriftdata
//                   $C3_LIB/data/test.drt0channel
//                   $C3_LIB/data/test.dradcpedestal
//        these correspond to that hard coded for DriftFunctionType 0 (see below)
//        and cleog3 old MC - this will change, old cleog3 MC will only
//        be supported for DriftFunctionLevel 0.
//      - Currently TDCQuantum, GlobalT0 parameters are still used.
//        TDCQuantum will become invalid when DBDRtdc2time constants are
//        available.
//
//   param CDOffCal DriftFunctionLevel 0
//      - use this for old cleog3 MC, otherwise obsolete
//      - default, this is the calibration corresponding to the old MonteCarlo
//                 TDC Quantum   +250ns/count
//                 Tzero         0ns
//                 ADC Quantum   1/count
//                 ADC pedestal  0
//                 DriftFunction hard coded (LKG)
//      - the following command overrides the TDC Quantum:
//           param CDOffCal TDCQuantum -500
//      - the following command -adds- a global T0 (picoseconds) to all channels:
//           param CDOffCal GlobalT0 5000.
//      - for real data with no available calibration one might
//        want to set TDCQuantum 0 and GlobalT0 to the middle
//        of the valid range for times (see other parameters below).
//      - if one wants to use a different t0 for individual channels
//        supply a file channelT0s.v01 in the same directory as you
//        run suez from, it is a list of ordered pairs (channel,tzero)
//        if this does not exist the t0s are set to 0+GlobalT0
//      - note that none of these options affect the old MC simulation
//        but they will affect suez -cleog.
//      - ADC pedestals for individual channels are read from channelPeds.v01
//        if the file does not exist they are set to zero
//   param CDOffCal DriftFunctionLevel 1
//      - this option should be used only for people making drift functions
//        and is essentially obsolete with this release
//      - same as level 0 except it uses the real HePr drift function
//        code from file hep_drift.v01 that must exist in the directory
//        you run suez from
//
// Details of changes:
//   DataDriftFunction.cc
//     - first nontrivial version, replaced file access by access
//       to constants from the frame as member functions of DriftFunctionProxy,
//       extracted before the DriftFunction is instantiated by the
//       faultHandler.
//     - Changed behaviour of DebugLevel parameter:
//          1   - nothing happens
//          >1  - information on every hit
//          =3  - dumps drift function information during constants unpack
//          =4  - dumps drift map information after constants unpack
//          =5  - dumps tzero,adc constants after constants unpack
//   DriftFunction.cc
//     - added adcpedestal(),adcpedestalRMS(),tdctzero() mainly
//       for use of constants external to CDOffCal (e.g. DRMCUtil)
//     - added unpack of t0s and adcpedestals for DriftFunctionType
//       kDFType_HePr_data1. Warnings are printed if constants
//       are not available for every channel.
//   DriftFunctionProxy.cc
//     All the BDL/constants objects described above are extracted
//     and stored as member data for access by the calibration,
//     but only for DriftFunctionType kDFType_HePr_data1 (behaviour
//     is not changed for other settings)
//   HePrDriftFunction.cc
//     Changes to DebugLevel parameter behaviour for DataDriftFunction.cc
//     are mirroed here.
//   HitQuality.cc
//     Guang Pei Chen corrected some of my miscoding, on evaluating expressions
//     of the type a & b == c I needed to add some brackets ( a & b ) == c
//
// Revision 1.2  1999/12/15 19:48:51  mclean
// Some bug fixes, made get-parameter functions in CDOffCalProducer
// inline functions, added channel t0s to DriftFunction.h,
// added DebugLevel parameter to CDOffCalProducer.
//
// DriftFunction Constructor reads in the channel t0s from a file
// channelT0s.v01 in the current (suez executing) directory (this is
// just a set of 9796 ordered pairs (wire#,T0).
// If this file is not found, then they are set t0 0. These add to the
// GlobalT0 that can be set by parameter (default 0). NOTE all
// times are in PICOSECONDS.
//
// There is now a DebugLevel Parameter.
//  0 means do nothing (currently this doesnt work, does same as 1)
//  1 means count various failure and success modes
//  2 means dump info from makeCalibratedDRHit once per hit
//  3 means dump raw DF info (only for HePrDriftFunction and DataDriftFunction)
//  4 means dump DF map info (only for HePrDriftFunction and DataDriftFunction)
// These decisions filter out most of the report(DEBUG messages, a few
// remain but should be executed at about the same rate that new calibration
// constants appear in the input streams.
//
// The parameters (even things needed per hit like GlobalT0 and TDCQuantum)
// are accessed by inline function calls to member data of CDOffCalProducer.
// In the future it might be better to force the DriftFunction (i.e. entire
// DR Calibration object) to be deleted and remade when a parameter changes,
// or to pass the data into the DriftFunction object from the
// init() function call in CDOffCalProducer.
//
// Revision 1.6  1999/12/15 08:07:09  mclean
// Additional debug trace statements in CDOffCalProducer/DriftFunctionProxy.
//
// DriftFunctionProxy also had one small bug fixed in the constructor,
// large swathes of code are also preprocessed out via the REALCONSTANTS
// flag on a temporary bases.
//
// In HePrDriftFunction.cc I massaged the report DEBUG statements to look
// a bit nicer, they trace the entire history of the DriftFunction constants
// objects construction, and then dump it again.. you will not enjoy
// "report level debug". I will AND this with an internal debug parameter
// flag soon, really, trust me.
//
// In Statistics.cc, instead of doing an "assert" when someone passes
// it an illegal index (i.e. books a counter out of the preallocated
// memory space ("just so Fortran-Passe' you say")) I just dont do
// anything. Seems civilized.
//
// Revision 1.5  1999/12/07 23:30:10  mclean
// Many changes:
//    (1) Parameter data is stored locally to the Producer and accessible
//        via member functions.
//    (2) Frame data is stored locally to the Proxy and accessible via
//        member functions (i.e. Geometry, Constants)
//    (3) Includes have been streamlined to speed up makedepend
//    (4) New parameters have been instituted for
//                 GlobalT0     (Global T0 shift)
//                 TDCQuantum   (TDC slope, 250. MC, -500. data)
//                 Mintime      (in normal time sense, earliest allowed hit)
//                 Maxtime      (in normal time sense, latest allowed hit)
//    (5) The following masks are overlayed in the "goodness"
//        parameter returned to makeCalibratedHit:
//                Flag_Bad         =  1;
//                Flag_SingleTDC   =  2;
//                Flag_TestChannel =  4;
//                Flag_NoCharge    =  8;
//                Flag_NoTime      = 16;
//                Flag_NotInWindow = 32;
//    (6) Class HitQuality has its own file and additional functionality
//        to set_ and test_ all the above qualities.
//    (7) Provision for access to all current DR constant entities
//        has been made for in the Proxy.
//    (8) There is an additional Class inheriting from DriftFunction:
//        DataDriftFunction. This will use real Frame-based constants
//        eventually.
//
// Revision 1.4  1999/12/06 20:34:41  mclean
// StopGap Measure: delete lots of report(debug... lines to limit cpu consumption
//
// Revision 1.3  1999/11/26 22:14:51  mclean
// Added statistical counters for messages that were annoying everyone along
// with lots of new information.
// These print out at INFO level when DriftFunction's destructor is called
// (i.e. once per calibration set).
//
// Any hits with cellid > 9796 are now flagged as bad.
// Any hits with more than one TDC hit were allready flagged as bad.
// Streamlined makeCalibratedHIt.
//
// Revision 1.2  1999/10/26 16:26:56  mclean
// Modified definition of time in CalibratedDRHit.
// It now includes the bunch shift appropriately (i.e. it is a true
// Drift time).
//
// Revision 1.1  1999/06/23 22:20:36  lyon
// Specific drift functions
//
// Revision 1.23  1999/03/05 20:57:40  pg
// Switched to using STL_XXX macros (was crashing on Solaris). 
// Got rid of delete calls in DriftFunction.cc (same reason)
//
// Revision 1.22  1998/12/15 21:03:46  mclean
// First working HePr DF code (hardwired, tcl parameters not enabled, not for
//                             general use)
//
// Revision 1.21  1998/12/10 21:43:49  mclean
// Updates for HePr drift functions (old analytic DFs are still default)
//
// Update for HePr driftfunctions 
// (old analytic drift functions are still default)
//
// Revision 1.20  1998/11/27 20:24:09  mclean
// Introduce STL definition conventions.
//
// Revision 1.19  1998/11/27 19:48:52  mclean
// Warnings introduced in DriftFunction::time2Drift in the case of
// negative times leading to unphysical negative unsigned drift distances.
// These are now set to zero drift.
//
// Warnings introduced in tdc2Time in the case of huge positive TDC's
// appearing (these are set to zero).
//
// Revision 1.16  1998/11/25 23:07:18  lyon
// Use STL macros
//
// Revision 1.15  1998/11/19 19:17:54  mclean
// Prelimintary changes for HePr drift functions...dummied out in this
// file with C-preprocessor statements.
//
// Modifications to LayerWire_To_Wire
//                  LayerWire_To_Address functions
// (layerwire2* arrays were defined starting at wire# -1!!!not 0)
//
// Motifications to test of rawaddress w.r.t. kMaxDRlayer (<= not <)
//
// Prelimintary changes for HePr drift functions...dummied out in this
// file with C-preprocessor statements.
//
// Revision 1.13  1998/10/13 21:41:39  mclean
// Change kTotalDRWires to correct value
// Change tests using kTotalDRWires to <= from < (value was too large
// which let this logic bug live)
//
// Revision 1.12  1998/10/07 21:29:55  mclean
// Modified wire number (kTotalDRWires) to remove non-sense wires :)
//
// Updated the fitting weights to be 1/0.000123**2 (i.e. to correspond
// to the average smearing in cg3/cdrawm/cdsmer.F and ccon/cd.zfx)
//
// Revision 1.11  1998/09/21 16:55:58  lyon
// Oops - Tseries in CalibratedData now
//
// Revision 1.10  1998/09/10 15:16:59  lyon
// Added getCalibratedDRSeries
//
// Revision 1.9  1998/09/02 20:29:08  mclean
// Installed several new const functions for address/layer/wire conversion:
//       Wire_To_Layer          ( wire    )
//       Address_To_Layer       ( address )
//       Address_To_Wire        ( address )
//       Address_To_WireInLayer ( address )
//       LayerWire_To_Wire      ( wire, layer )
//       LayerWire_To_Address   ( wire, layer )
//       ...All values are UInt16
//
// Revision 1.8  1998/08/12 21:22:21  mclean
// Added definitions for
// getCalibratedDR2
//   like getCalibratedDR but replaces
//                   distance,LeftDistance,RightDistance
//   by
//                   distance,velocity,acceleration
//   (velocity=ddistance/dtime,acceleration=dvelocity/dtime)
// getCalibratedDR3
//   like getCalibratedDR but replaces
//                   distance,LeftDistance,RightDistance
//   by
//                   Tseries dva
//   which is a "Taylor series" object one can use to expand
//   about a given time in the drift function (e.g. to determine
//   bunch times for a nearby bunch)
//
// Revision 1.7  1998/08/11 08:02:00  mclean
// Geometry file will be extracted from CDOffCal/data/cdcal.geom2 (thanks CDJ!)
// If not found, it will be looked for locally.
//
// Revision 1.6  1998/08/10 22:47:21  mclean
// This divorces CDOffCal from AJW's c3tr geometry routines to avoid conflict
// with updates of these geometry routines in the c3tr area.  CDOffCal
// now uses a much abbreviated local geometry file:
//          /home/mclean/cleo3/cvssrc/CDOffCal/cdcal.geom2
// If someone wants to tell me how to make this more publicly/transparently
// available I would appreciate it!
//
// Revision 1.5  1998/07/27 22:00:23  mclean
// After Adam Lyon notified on July 12th, 1998 at 21:50 of a small bug with
// big effect I have finally  (July 27th, 1998 at 17:22) fixed it, in
// DriftFunction.cc:
//          rawAddress2Layer[rawhit.daqId()-1]
// replaces:
//          rawAddress2Layer[rawhit.daqId()  ]
//
// Revision 1.4  1998/07/20 14:14:38  cleo3
// fixed up typo after changes required for solaris
//
// Revision 1.3  1998/05/06 21:40:26  mclean
// Updates to Match Modifications to RawData
//
// Revision 1.2  1998/04/24 20:30:08  mclean
// First Real Code Installed
//
// Revision 1.1.1.1  1998/03/04 20:41:49  mkl
// imported CDOffCal sources
//
#include "Experiment/Experiment.h"

// system include files
#include "C++Std/fstream.h"
#include "C++Std/iostream.h"
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "CDOffCal/CDOffCalProducer.h"
#include "CDOffCal/DataDriftFunction.h"
#include "CDOffCal/DFDataHolder.h"
#include "CDOffCal/Statistics.h"
#include "CalibratedData/Tseries.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "ADRGeom/ADRSenseWireStore.h"

// STL include files
#include <vector>
#include <map>
#include <set>

static const char* const kFacilityString   = "CDOffCal.DataDriftFunction" ;
const double DataDriftFunction::kIllegalValue    =  19610912.0 ;
const double DataDriftFunction::kminimum_time    = -16384000.0 ;
const double DataDriftFunction::kmaximum_time    =  16384000.0 ;
const double DataDriftFunction::kmaximum_drift   =         0.1 ;
const float  DataDriftFunction::kCorr_DriftMax   =       0.007 ; 
const float  DataDriftFunction::kCorr_TimeMax    =    265000.0 ;
const float  DataDriftFunction::kCorr_EAngleMax  =        45.0 ; 
const float  DataDriftFunction::kCorr_QchargeMax =      8000.0 ; 
const float  DataDriftFunction::kCorr_Qquantum   =       100.0 ; 


//----------------------------------------------------------------------------
DataDriftFunction::DataDriftFunction(CDOffCalProducer* CDOffCal,
				     DFDataHolder* iDataHolder): 
          DriftFunction(CDOffCal, iDataHolder),
	  drIII_df_data(kmaxDFside,STL_VECTOR(Singlelayer_df_data)(kmaxDRlayer))
{
  //
  //---------------------------------------------------------------------------
  // OPEN CONSTANTS FROM FRAME
  //
  report( DEBUG, kFacilityString )  << "DataDriftFunction Constructor Starts" << endl;
  //
  //-------------------------------------------------------------------------------
  // DIGEST DRIFT DATA 
  //
  report ( INFO, kFacilityString )  << "Reading Drift Functions " << endl;  
  int    input_Record, input_Code, input_DFType ;
  int    input_DRlayer, input_DRlayer_Previous  ;
  double input_Time,    input_Time_Previous     ;
  double input_Dist,    input_Dist_Previous     ;
  double input_Velo    ;
  double input_InvVelo ;
  
  // 
  // layerread/layerused verifies that all input points are used properly
  // (local to constructor, constants verification)
  //
  int layerloop , layerread[kmaxDRlayer] , layerused[kmaxDRlayer] ; 
  for (layerloop = 0;layerloop<kmaxDRlayer;++layerloop) 
    { 
      layerread[layerloop]=0 ; 
      layerused[layerloop]=0 ;
    }
  //
  // drIII_df_data is an STL_VECTOR of drift function objects
  // indexed by side (L+R,R,L) and layer
  // we dont initialize every layer and side, but the internal
  // variable "side" is set to kNODF and should remain that
  // way if no data for this layer/side is found. 
  // 
  for (layerloop = 0;layerloop<kmaxDRlayer;++layerloop) 
    { 
      int sideloop ;
      for (sideloop = 0;sideloop<kmaxDFside;++sideloop) 
	{
	  drIII_df_data[sideloop][layerloop].side = kNODF ; 
	}
    }

  input_Record    = 0             ;
  input_DRlayer   = 0             ;
  input_Time      = 0             ;
  input_Dist      = 0             ;
  input_Velo      = 0             ;
  input_InvVelo   = 0             ;
  input_DRlayer   = 0             ;
  
  //-------------------------------------------------------------------------------------------------
  //
  // Unpack new (v2) constants object if valid
  //
  DABoolean threesided = m_dataHolder->DriftDatav2().valid() ;
  if ( threesided )
    {
      report( INFO, kFacilityString ) << "DriftDatav2 Object Used For DF Constants" << endl;
      int DF_filesize = m_dataHolder->DriftDatav2().size();
      report(DEBUG, kFacilityString )  << "Drift Function File Size: " << DF_filesize << endl;
 
      for(FAConstants<DBDRDriftDatav2>::const_iterator
	  loop1=m_dataHolder->DriftDatav2().begin()   ;
	  loop1!=m_dataHolder->DriftDatav2().end()    ;
	  ++loop1                                     ) 
	{
	  input_Time_Previous    = input_Time    ;
	  input_Dist_Previous    = input_Dist    ;
	  input_DRlayer_Previous = input_DRlayer ;
	  
	  input_Record++;
	  // Convert to PicoSeconds
	  input_Time = ( 1000 * (*loop1).get_Time() ) ;
	  input_Dist = (*loop1).get_Dist() ;
	  input_Code = (*loop1).get_Indx() ;
	  
	  //m_statistics->counter(51,"drift data records"); 
	  
	  // parse DF point code 
	  input_DFType    =    ( ( input_Code / 10000 ) % 1000 ) ;
	  input_DRlayer   =    ( input_Code / 10000000 )   ;
	  layerread[input_DRlayer-1]++;
	  
	  if  ( debugLevel[3] ) 
	    {
	      if ( input_DRlayer_Previous != input_DRlayer )
		{
		  report( INFO , kFacilityString )  << "New DF Layer Unpack" << endl;
		}
	      report( INFO , kFacilityString ) << "DRDMP Starting record,  # " << input_Record << endl
					       << "DRDMP                Time " << input_Time   << endl
					       << "DRDMP            Distance " << input_Dist   << endl
					       << "DRDMP                Type " << input_DFType << endl 
					       << "DRDMP               Layer " << input_DRlayer<< endl ;
	    }
	  if ( input_DFType == kLRaveraged || input_DFType == kleftside || input_DFType == krightside ) 
	    {
	      if ( input_DRlayer <= kmaxDRlayer ) 
		{     
		  Singlelayer_df_data& df = drIII_df_data[input_DFType-1][input_DRlayer-1];
		  if ( input_DRlayer_Previous != input_DRlayer )
		    {
		      //m_statistics->counter(52,"layers in drift records"); 
		      input_Time_Previous      = kminimum_time ;
		      input_Dist_Previous      = 0             ;
		      input_Velo               = 0             ;
		      input_InvVelo            = 0             ;
		      df.max_time              = kminimum_time ;
		      df.min_time              = kmaximum_time ;
		      df.max_drift             = 0             ;
		    }
		  if ( input_Time == input_Time_Previous ) 
		    {
		      report( WARNING, kFacilityString )  << "DRDMP repeated time, skipping point " << input_Time << endl;
		      //m_statistics->counter(53,"repeated times in drift records"); 
		      if ( input_Dist == input_Dist_Previous )
			{
			  //m_statistics->counter(54,"repeated drift points, skipped"); 
			}
		    }
		  else
		    {
		      input_Velo      = ( input_Dist - input_Dist_Previous  ) /
			( input_Time - input_Time_Previous  ) ;
		      if ( input_Dist == input_Dist_Previous ) 
			{
			  //report( WARNING, kFacilityString )  << "DRDMP repeated distance " << input_Dist << endl;
			  //m_statistics->counter(55,"repeated drift distance in drift records"); 
			  input_InvVelo      = kIllegalValue;
			}
		      else
			{
			  input_InvVelo      = ( input_Time    - input_Time_Previous     ) /
			    ( input_Dist    - input_Dist_Previous     ) ;
			}
		      //
		      // Should replace by an "addpoint" function in singledflayer object...
		      //
		      Tseries New_DF_Point(input_Dist,input_Velo,0.0)            ;
		      df.t2d_map[input_Time] = New_DF_Point                      ;
		      Tseries New_InverseDF_Point(input_Time,input_InvVelo)      ;
		      df.d2t_map[input_Dist] = New_InverseDF_Point               ;
		      if ( input_Time > df.max_time  ) df.max_time  = input_Time ;
		      if ( input_Time < df.min_time  ) df.min_time  = input_Time ;
		      if ( input_Dist > df.max_drift ) df.max_drift = input_Dist ;
		      df.side = input_DFType                                     ;
		      
		      layerused[input_DRlayer-1]++                               ;
		      //m_statistics->counter(56,"drift points used")            ;
		      
		      if  ( debugLevel[3] )
			{
			  report( INFO , kFacilityString ) << "DRDMP Velocity " 
							   << input_Velo  
							   << " Inverse " 
							   << input_InvVelo 
							   << " record# " 
							   << input_Record  
							   << endl ;
			}
		    }
		} 
	      else
		{
		  report( WARNING, kFacilityString )  << "Drift Function Layer has Illegal Value: " << input_DRlayer << endl;
		}
	    }
	  else
	    {
	      report( WARNING, kFacilityString )  << "Illegal Drift Function Type: " << input_DFType << endl;
	    }
	}
    }
  //-------------------------------------------------------------------------------------------------
  //
  // Unpack original constants object if new one is not valid
  //
  DABoolean onesided = m_dataHolder->DriftData().valid() ;
  if ( onesided && ( !threesided ) )
    {
      report( WARNING, kFacilityString ) << "Old DriftData Object Used For DF Constants" << endl;
      int DF_filesize = m_dataHolder->DriftData().size();
      report(DEBUG, kFacilityString )  << "Drift Function File Size: " << DF_filesize << endl;
 
      for(FAConstants<DBDRDriftData>::const_iterator
	  loop2=m_dataHolder->DriftData().begin();
	  loop2!=m_dataHolder->DriftData().end();
	  ++loop2 ) 
	{
	  input_Time_Previous    = input_Time    ;
	  input_Dist_Previous    = input_Dist    ;
	  input_DRlayer_Previous = input_DRlayer ;
	  
	  input_Record++;
	  // Convert to PicoSeconds
	  input_Time = ( 1000 * (*loop2).get_Time() ) ;
	  input_Dist = (*loop2).get_Dist() ;
	  input_Code = (*loop2).get_Indx() ;
	  
	  //m_statistics->counter(51,"drift data records"); 
	  
	  // parse DF point code 
	  input_DFType    =    ( ( (int) ( ( (float)input_Code ) / 10000. + 0.5 ) ) % 1000 ) ;
	  input_DRlayer   =   ( input_Code / 10000000 )   ;
	  layerread[input_DRlayer-1]++;
	  
	  if  ( debugLevel[3] )
	    {
	      if ( input_DRlayer_Previous != input_DRlayer )
		{
		  report( INFO , kFacilityString )  << "New DF Layer Unpack" << endl;
		}
	      report( INFO , kFacilityString ) << "DRDMP Starting record,  # " << input_Record << endl
					       << "DRDMP                Time " << input_Time   << endl
					       << "DRDMP            Distance " << input_Dist   << endl
					       << "DRDMP                Type " << input_DFType << endl 
					       << "DRDMP               Layer " << input_DRlayer<< endl ;
	    }
	  if ( input_DFType == kLRaveraged || input_DFType == kleftside || input_DFType == krightside ) 
	    {
	      if ( input_DRlayer <= kmaxDRlayer ) 
		{     
		  Singlelayer_df_data& df = drIII_df_data[input_DFType-1][input_DRlayer-1];
		  if ( input_DRlayer_Previous != input_DRlayer )
		    {
		      //m_statistics->counter(52,"layers in drift records"); 
		      input_Time_Previous      = kminimum_time ;
		      input_Dist_Previous      = 0             ;
		      input_Velo               = 0             ;
		      input_InvVelo            = 0             ;
		      df.max_time              = kminimum_time ;
		      df.min_time              = kmaximum_time ;
		      df.max_drift             = 0             ;
		    }
		  if ( input_Time == input_Time_Previous ) 
		    {
		      report( WARNING, kFacilityString )  << "DRDMP repeated time, skipping point " << input_Time << endl;
		      //m_statistics->counter(53,"repeated times in drift records"); 
		      if ( input_Dist == input_Dist_Previous )
			{
			  //m_statistics->counter(54,"repeated drift points, skipped"); 
			}
		    }
		  else
		    {
		      input_Velo      = ( input_Dist - input_Dist_Previous  ) /
			( input_Time - input_Time_Previous  ) ;
		      if ( input_Dist == input_Dist_Previous ) 
			{
			  //report( WARNING, kFacilityString )  << "DRDMP repeated distance " << input_Dist << endl;
			  //m_statistics->counter(55,"repeated drift distance in drift records"); 
			  input_InvVelo      = kIllegalValue;
			}
		      else
			{
			  input_InvVelo      = ( input_Time    - input_Time_Previous     ) /
			    ( input_Dist    - input_Dist_Previous     ) ;
			}
		      //
		      // Should replace by an "addpoint" function in singledflayer object...
		      //
		      Tseries New_DF_Point(input_Dist,input_Velo,0.0)            ;
		      df.t2d_map[input_Time] = New_DF_Point                      ;
		      Tseries New_InverseDF_Point(input_Time,input_InvVelo)      ;
		      df.d2t_map[input_Dist] = New_InverseDF_Point               ;
		      if ( input_Time > df.max_time  ) df.max_time  = input_Time ;
		      if ( input_Time < df.min_time  ) df.min_time  = input_Time ;
		      if ( input_Dist > df.max_drift ) df.max_drift = input_Dist ;
		      df.side = input_DFType                                     ;
		      
		      layerused[input_DRlayer-1]++                               ;
		      //m_statistics->counter(56,"drift points used")            ;
		      
		      if  ( debugLevel[3] )
			{
			  report( INFO , kFacilityString ) << "DRDMP Velocity " 
							   << input_Velo  
							   << " Inverse " 
							   << input_InvVelo 
							   << " record# " 
							   << input_Record  
							   << endl ;
			}
		    }
		} 
	      else
		{
		  report( WARNING, kFacilityString )  << "Drift Function Layer has Illegal Value: " << input_DRlayer << endl;
		}
	    }
	  else
	    {
	      report( WARNING, kFacilityString )  << "Illegal Drift Function Type: " << input_DFType << endl;
	    }
	}
    }
  if ( ( !onesided ) && ( !threesided ) )
    {
      report( WARNING, kFacilityString )  << "NO DriftData Constants Found! \n" 
					  << "The Proxy should have thrown an exception! "
					  << endl;
    }
  //-------------------------------------------------------------------------------------------------
  //
  // Check all layers are complete
  //
  for (layerloop = 0;layerloop<kmaxDRlayer;++layerloop) 
    { 
      if( layerread[layerloop] != layerused[layerloop] )
	{
	  report( WARNING, kFacilityString )  
	    << "Layer " << layerloop+1 << " inconsistent "
	    << layerread[layerloop]  << " "
	    << layerused[layerloop]  << endl;
	}
      if( layerread[layerloop] == 0 )
	{
	  report( WARNING, kFacilityString )  << "Layer "<<layerloop+1<<" has no points "<< endl;
	}
    }
  //-------------------------------------------------------------------------------------------------
  //
  // Debug Dump
  //
  int ilayer ;
  if  ( debugLevel[4] )
    { 
      if ( onesided || threesided ) 
	{
	  for ( ilayer = 1; ilayer<=kmaxDRlayer; ++ilayer )
	    {
	      const Singlelayer_df_data& ndf = drIII_df_data[kLRaveraged][ilayer-1] ;   
	      report( INFO , kFacilityString )  << "DFMAP LR Average " 
						<< " Layer "            
						<< ilayer
						<< " min_time "         
						<< ndf.min_time 
						<< " max_time "         
						<< ndf.max_time 
						<< endl;   

	      for (STL_MAP(double,Tseries)::const_iterator 
                   iter  = (ndf.t2d_map).begin(); 
                   iter != (ndf.t2d_map).end()  ; 
                 ++iter )
		{
		  report( INFO , kFacilityString ) << " Distance: " 
						   << (*iter).second.distance()
						   << " Time: "     
						   << (*iter).first 
						   << endl;
		}
	    }
	}
      if ( threesided )
	{
	  for ( ilayer = 1; ilayer<=kmaxDRlayer; ++ilayer )
	    {
	      const Singlelayer_df_data& ndf = drIII_df_data[kleftside][ilayer-1] ;   
	      report( INFO , kFacilityString )  << "DFMAP Left " 
						<< " Layer "            
						<< ilayer
						<< " min_time "         
						<< ndf.min_time 
						<< " max_time "         
						<< ndf.max_time 
						<< endl;   

	      for (STL_MAP(double,Tseries)::const_iterator 
                   iter  = (ndf.t2d_map).begin(); 
                   iter != (ndf.t2d_map).end()  ; 
                 ++iter )
		{
		  report( INFO , kFacilityString ) << " Distance: " 
						   << (*iter).second.distance()
						   << " Time: "     
						   << (*iter).first << endl;
		}
	    }
	  for ( ilayer = 1; ilayer<=kmaxDRlayer; ++ilayer )
	    {
	      const Singlelayer_df_data& ndf = drIII_df_data[krightside][ilayer-1] ;   
	      report( INFO , kFacilityString )  << "DFMAP Right " 
						<< " Layer "            
						<< ilayer
						<< " min_time "         
						<< ndf.min_time 
						<< " max_time "         
						<< ndf.max_time 
						<< endl;   

	      for (STL_MAP(double,Tseries)::const_iterator 
                   iter  = (ndf.t2d_map).begin(); 
                   iter != (ndf.t2d_map).end()  ; 
                 ++iter )
		{
		  report( INFO , kFacilityString ) << " Distance: " 
						   << (*iter).second.distance()
						   << " Time: "     
						   << (*iter).first << endl;
		}
	    }
	}
    }
  //-------------------------------------------------------------------------------------
  //
  // Unpack Garfield Drift Distance Corrections from Lauren Hsu (llh14)
  //
  //-------------------------------------------------------------------------------------
  //
  // the "Bins" numbers are actually points to be interpolated between...hence the - 1
  kCorr_EAngleBinSize  = ( 2.0 * kCorr_EAngleMax) / ( (float) ( kCorr_EAngleBins   - 1 ) ) ; 
  kCorr_DriftBinSize   = kCorr_DriftMax           / ( (float) ( kCorr_DriftBins    - 1 ) ) ;
  kCorr_TimeBinSize    = kCorr_TimeMax            / ( (float) ( kCorr_TimeBins     - 1 ) ) ;
  /*-------------------------------------------------------------------------------------
     Initialize corrections to zero 
     so any missing data does not yield a zero or corrupt weight
     we DO NOT verify that data is complete, indeed currently there
     no correction in the BDL object for layers 1 and 47

     Zero "set" array to check for repeated attempt to set a bin
  */  
  int setDrift[kmaxDRlayer][kCorr_EAngleBins][kAmbiguities][kCorr_TimeBins] ;
  for ( int loop_layr1=0 ; loop_layr1<kmaxDRlayer ; ++loop_layr1 )
    {
      for ( int loop_eangl=0 ; loop_eangl<kCorr_EAngleBins ; ++loop_eangl )
	{
	  for ( int loop_timeb=0 ; loop_timeb<kCorr_TimeBins ; ++loop_timeb )
	    {
	      int loop_ambig = 0 ;
	      corrDrift[ loop_layr1 ]
   		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_timeb ] = 0.0 ;
	       setDrift[ loop_layr1 ]
		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_timeb ] = 0   ;
	      loop_ambig = 1 ;
	      corrDrift[ loop_layr1 ]
   		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_timeb ] = 0.0 ;
	       setDrift[ loop_layr1 ]
		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_timeb ] = 0   ;
	    }
	}
    }
  /*-------------------------------------------------------------------------------------
     BDL definition of DBDRGarfield_DistanceCorrection
     UInt32 entry
     float  drifttime (ps, 5000ps increments)
     float  z_position (actually layer*10000+2+ambiguity)  (last minute kluge, sorry)
     float  entrance_angle (degrees, 5 degree increments from -45o thru 45o)
     float  driftdistance_correction (distance, meters) 
  */
  report(DEBUG, kFacilityString )  << "Garfield Drift Correction File Size: " 
		 		   << m_dataHolder->DriftCorr().size()
			 	   << endl;
  for(FAConstants<DBDRGarfield_DistanceCorrection>::const_iterator 
	loopD =m_dataHolder->DriftCorr().begin() ;
        loopD!=m_dataHolder->DriftCorr().end()   ; ++loopD ) 
    {
        int error = 0 ;
        float angle = (*loopD).get_entrance_angle() ;
        if ( angle < -kCorr_EAngleMax )         { error +=  8 ; }
        if ( angle >  kCorr_EAngleMax )         { error += 16 ; }

	int anglebin  = ( (int) ( ( (angle + kCorr_EAngleMax) / kCorr_EAngleBinSize ) + 0.5 ) ) ;
        int timebin   = ( (int) ( ( (*loopD).get_drifttime()  / kCorr_TimeBinSize   ) + 0.5 ) ) ;
        int layer     = ( (int) ( ( (*loopD).get_z_position() / 10000.              ) + 0.5 ) ) ;

        int   ambiguity = 0 ; 
        if ( ( (*loopD).get_z_position() - ( ( (float) layer ) * 10000. ) - 2.0 ) > 0 )
	  { ambiguity = 1 ; } 
        //
        // Range and Error Checking
        // Dont store data from out of range points!
        //
        if ( ambiguity != 0 && ambiguity != 1 )  { error +=  1 ; }
        if ( layer    <             1  )         { error +=  2 ; }
        if ( layer    >   kmaxDRlayer  )         { error +=  4 ; }
        if ( timebin  <             0  )         { error += 32 ; }
        if ( timebin  > kCorr_TimeBins  )        { error += 64 ; }
	if ( error != 0 )
	  {
	    report( WARNING, kFacilityString )  << " DistanceCorrection, corrupt record "
						<< " Entry " << (*loopD).get_entry() 
						<< " Time  " << (*loopD).get_drifttime()
						<< " Code  " << (*loopD).get_z_position()
						<< " Angle " << (*loopD).get_entrance_angle()
						<< " CorrV " << (*loopD).get_driftdistance_correction()
						<< " Ambig " << ambiguity
						<< " Layer " << layer
						<< " Error " << error
						<< endl;	    
	  }
	else
	  {
	    if ( setDrift[layer-1  ]
	                 [anglebin ]
	                 [ambiguity]
	                 [timebin  ] == 0 )
	      {
		corrDrift[layer-1  ]
		         [anglebin ]
		         [ambiguity]
		         [timebin  ] = (*loopD).get_driftdistance_correction();
	      }
	    else
	      {
		report( WARNING, kFacilityString )  << " Repeated Record "
						    << " Entry " << (*loopD).get_entry() 
						    << " Time  " << (*loopD).get_drifttime()
						    << " Code  " << (*loopD).get_z_position()
						    << " Angle " << (*loopD).get_entrance_angle()
						    << " CorrV " << (*loopD).get_driftdistance_correction()
						    << " Ambig " << ambiguity
						    << " Tbin  " << timebin
						    << " Abin  " << anglebin
						    << " Layer " << layer
						    << " Error " << error
						    << endl;	    
		corrDrift[layer-1  ]
		         [anglebin ]
		         [ambiguity]
		         [timebin  ] = 0.0;
	      }
	    setDrift[layer-1  ]
	            [anglebin ]
	            [ambiguity]
	            [timebin  ]++;
	  }
    }
  /*-------------------------------------------------------------------------------------
     Initialize corrections to zero 
     so any missing data does not yield an undefined correction
     we DO NOT verify that data is complete, indeed currently there
     no correction in the BDL object for layers 1 and 47

     Zero "set" array to check for repeated attempt to set a bin
  */
  int setTime[kmaxDRlayer][kCorr_EAngleBins][kAmbiguities][kCorr_DriftBins];
  for ( int loop_layr2=0 ; loop_layr2<kmaxDRlayer ; ++loop_layr2 )
    {
      for ( int loop_eangl=0 ; loop_eangl<kCorr_EAngleBins ; ++loop_eangl )
	{
	  for ( int loop_drift=0 ; loop_drift<kCorr_DriftBins ; ++loop_drift)
	    {
	       int loop_ambig = 0 ;
	       corrTime[ loop_layr2 ]
   		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_drift ] = 0.0 ;
	        setTime[ loop_layr2 ]
		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_drift ] = 0   ;
	       loop_ambig = 1 ;
	       corrTime[ loop_layr2 ]
   		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_drift ] = 0.0 ;
	        setTime[ loop_layr2 ]
		       [ loop_eangl ]
		       [ loop_ambig ]
		       [ loop_drift ] = 0   ;
	    }
	}
    }
  /*----------------------------------------------------------------------------------- 
     BDL definition of DBDRGarfield_TimeCorrection
     UInt32 entry
     float  driftdistance (meters in 200 micron increments)
     float  z_position (actually layer*10000+1+ambiguity)
     float  entrance_angle (degrees, 5 degree increments from -45o thru 45o)
     float  drifttime_correction (time, ps)
  */
  report(DEBUG, kFacilityString )  << "Garfield Time  Correction File Size: " 
				   <<  m_dataHolder->TimeCorr().size()
				   << endl;

  for(FAConstants<DBDRGarfield_TimeCorrection>::const_iterator 
	loopT =m_dataHolder->TimeCorr().begin()  ;
        loopT!=m_dataHolder->TimeCorr().end()    ; ++loopT ) 
    {
        int error = 0 ;
        float angle = (*loopT).get_entrance_angle() ;
        if ( angle < -kCorr_EAngleMax )         { error +=  8 ; }
        if ( angle >  kCorr_EAngleMax )         { error += 16 ; }

	int anglebin  = ( (int) ( ( (angle + kCorr_EAngleMax)    / kCorr_EAngleBinSize ) + 0.5 ) ) ;
        int layer     = ( (int) ( ( (*loopT).get_z_position()    / 10000.              ) + 0.5 ) ) ;
        int driftbin  = ( (int) ( ( (*loopT).get_driftdistance() /  kCorr_DriftBinSize ) + 0.5 ) ) ;

        int   ambiguity = 0 ; 
        if ( ( (*loopT).get_z_position() - ( ( (float) layer ) * 10000. ) - 2.0 ) > 0 )
	  { ambiguity = 1 ; } 
        //
        // Range and Error Checking
        // Dont store data from out of range points!
        //
        if ( ambiguity != 0 && ambiguity != 1 )  { error +=  1 ; }
        if ( layer    <              1 )         { error +=  2 ; }
        if ( layer    >    kmaxDRlayer )         { error +=  4 ; }
        if ( driftbin <              0 )         { error += 32 ; }
        if ( driftbin > kCorr_DriftBins )        { error += 64 ; }
	if ( error != 0 )
	  {
	    report( WARNING, kFacilityString )  << " TimeCorrection, corrupt record "
						<< " Entry " << (*loopT).get_entry() 
						<< " Dist  " << (*loopT).get_driftdistance()
						<< " Code  " << (*loopT).get_z_position()
						<< " Angle " << (*loopT).get_entrance_angle()
						<< " CorrV " << (*loopT).get_drifttime_correction()
						<< " Ambig " << ambiguity
						<< " Dbin  " << driftbin
						<< " Abin  " << anglebin
						<< " Layer " << layer
						<< " Error " << error
						<< endl;	    
	  }
	else
	  {
            if ( setTime[layer-1  ]
                        [anglebin ]
                        [ambiguity]
                        [driftbin ] == 0 )
	      {
		corrTime[layer-1  ]
		        [anglebin ]
                        [ambiguity]
                        [driftbin ] = (*loopT).get_drifttime_correction();
	      }
	    else
	      {
		report( WARNING, kFacilityString )  << " TimeCorrection, repeated record "
						    << " Entry " << (*loopT).get_entry() 
						    << " Dist  " << (*loopT).get_driftdistance()
						    << " Code  " << (*loopT).get_z_position()
						    << " Angle " << (*loopT).get_entrance_angle()
						    << " CorrV " << (*loopT).get_drifttime_correction()
						    << " Ambig " << ambiguity
						    << " Layer " << layer
						    << " Error " << error
						    << endl;	    
		corrTime[layer-1  ]
		        [anglebin ]
                        [ambiguity]
                        [driftbin ] = 0 ;
	      }
	    setTime[layer-1  ]
	           [anglebin ]
	           [ambiguity]
	           [driftbin ]++;
	  }
    }
  //------------------------------------------------------------------------
  //
  // Distance Correction vs. Time
  //
  // A little bit of data quality verification 
  //
  float rms_dcorr         = 0. ;
  float count_dcorr       = 0. ;
  float count_all_dcorr   = 0. ;
  float count_dcorr_zeros = 0. ;
  for ( int loop_layr3=0 ; loop_layr3<kmaxDRlayer ; ++loop_layr3 )
    {
      for ( int loop_eangl=0 ; loop_eangl<kCorr_EAngleBins ; ++loop_eangl )
	{
	  for ( int loop_timeb=0 ; loop_timeb<kCorr_TimeBins ; ++loop_timeb )
	    {
              count_all_dcorr += 2. ;
              float X = 
	      corrDrift[ loop_layr3 ]
   		       [ loop_eangl ]
		       [          0 ]
		       [ loop_timeb ] ;
              if ( X != 0.0 ) 
		{
		  rms_dcorr += X*X ; 
		  count_dcorr++    ;
		}
              else
		{
		  count_dcorr_zeros++ ;
		}
              X = 
	      corrDrift[ loop_layr3 ]
   		       [ loop_eangl ]
		       [          1 ]
		       [ loop_timeb ] ;
              if ( X != 0.0 ) 
		{
		  rms_dcorr += X*X ; 
		  count_dcorr++    ;
		}
              else
		{
		  count_dcorr_zeros++ ;
		}
	    }
	}
    }
  if ( count_dcorr > 0 )
    {
      rms_dcorr = sqrt ( rms_dcorr / count_dcorr ) ;
      int nonzero_corrs = (int) 
	( ( ( count_dcorr                      / count_all_dcorr ) * 100. ) + 0.5 ) ;
      int    zero_corrs = (int) 
	( ( ( count_dcorr_zeros                / count_all_dcorr ) * 100. ) + 0.5 ) ;
      int  filled_corrs = (int) 
	( ( ( m_dataHolder->DriftCorr().size() / count_all_dcorr ) * 100. ) + 0.5 ) ;
      report( INFO, kFacilityString )  << "Entrance Angle Statistics \n"
                                       << "                                     Distance Correction vs. Time \n" 
                                       << "                                     Nonzero Correction Bins: " << nonzero_corrs << "%, \n" 
				       << "                                     RMS correction value   : " << rms_dcorr     << "(m), \n"
				       << "                                     Zero Correction Bins   : " << zero_corrs    << "%, \n"
				       << "                                     Filled Correction Bins : " << filled_corrs  << "%  \n"
				       << endl ;
   }  
  else 
    {
      int    zero_corrs = (int)
	( ( ( count_dcorr_zeros / m_dataHolder->DriftCorr().size() ) * 100. ) + 0.5 ) ;
      report( ERROR, kFacilityString ) << " Entrance Angle Statistics: Distance Correction vs. Time \n" 
				       << " Zero Correction Bins: "    << zero_corrs
				       << endl ;
    }  
  //------------------------------------------------------------------------
  //
  // Time Correction vs. Distance
  //
  // A little bit of data quality verification 
  //
  float rms_tcorr         = 0. ;
  float count_tcorr       = 0. ;
  float count_all_tcorr   = 0. ;
  float count_tcorr_zeros = 0. ;
  for ( int loop_layr4=0 ; loop_layr4<kmaxDRlayer ; ++loop_layr4 )
    {
      for ( int loop_eangl=0 ; loop_eangl<kCorr_EAngleBins ; ++loop_eangl )
	{
	  for ( int loop_drift=0 ; loop_drift<kCorr_DriftBins ; ++loop_drift)
	    {
               count_all_tcorr += 2. ;
               float X = 
	       corrTime[ loop_layr4 ]
   		       [ loop_eangl ]
		       [          0 ]
		       [ loop_drift ] ;
              if ( X != 0.0 ) 
		{
		  rms_tcorr += X*X ; 
		  count_tcorr++    ;
		}
              else
		{
		  count_tcorr_zeros++ ;
		}
               X = 
	       corrTime[ loop_layr4 ]
   		       [ loop_eangl ]
		       [          1 ]
		       [ loop_drift ] ;
              if ( X != 0.0 ) 
		{
		  rms_tcorr += X*X ; 
		  count_tcorr++    ;
		}
              else
		{
		  count_tcorr_zeros++ ;
		}
	    }
	}
    }
  if ( count_tcorr > 0 )
    {
      rms_tcorr = sqrt ( rms_tcorr / count_tcorr ) ;
      int nonzero_corrs = (int) 
	( ( ( count_tcorr                      / count_all_tcorr ) * 100. ) + 0.5 ) ;
      int    zero_corrs = (int) 
	( ( ( count_tcorr_zeros                / count_all_tcorr ) * 100. ) + 0.5 ) ;
      int  filled_corrs = (int) 
	( ( ( m_dataHolder->TimeCorr().size()  / count_all_tcorr ) * 100. ) + 0.5 ) ;
      report( INFO, kFacilityString )  << "Entrance Angle Statistics \n"
                                       << "                                     Time Correction vs. Distance \n" 
                                       << "                                     Nonzero Correction Bins: " << nonzero_corrs << "%, \n" 
				       << "                                     RMS correction value   : " << rms_tcorr     << "(ps),  \n"
				       << "                                     Zero Correction Bins   : " << zero_corrs    << "%, \n"
				       << "                                     Filled Correction Bins : " << filled_corrs  << "%  \n"
				       << endl ;
    }  
  else 
    {
      int    zero_corrs = (int) 
	( ( ( count_tcorr_zeros / m_dataHolder->TimeCorr().size() ) * 100. ) + 0.5 ) ;
      report( ERROR, kFacilityString )  << " Entrance Angle Statistics: Time Correction vs. Distance \n" 
					<< " Zero Correction Bins: "    << zero_corrs
					<< endl ;
    }  
  //----------------------------------------------------------------------------------- 
  report( DEBUG, kFacilityString )  << "DataDriftFunction Constructor Ends" << endl;
}
//---------------------------------------------------------------------------
DataDriftFunction::~DataDriftFunction()
{
  report( DEBUG, kFacilityString )  << "DataDriftFunction Destructor" << endl;
}
// const member functions
//---------------------------------------------------------------------------
DriftFunction::Drift DataDriftFunction::time2Drift( Time time, 
						    int layer, 
						    int outside) const
{
  // CHECK LAYER IN VALID RANGE
  if ( layer < 1 || layer > kmaxDRlayer )
    {
      report( ERROR, kFacilityString )  << "Illegal Layer # "
				        << layer 
				        << endl;
      return 0.0 ;
    }
  
  // CHECK AMBIGUITY IN VALID RANGE
  if ( outside != kLRaveraged && outside != kleftside && outside != krightside )
    {
      report( ERROR, kFacilityString )  << "Illegal Side Key "
			         	<< outside 
				        << endl;
      return 0.0 ;
    }
  double Distance(0.0);
  DABoolean once ( true ) ;
  //
  // If two-sided drift functions are not available 
  // use LR averaged or panic
  //
  int inside ;
  if ( drIII_df_data[outside-1][layer-1].side == kNODF ) 
    { 
      if ( once ) 
	{
	  report( WARNING, kFacilityString )  << "DF type unavailable: "
					      << drIII_df_data[outside-1][layer-1].side
					      << " trying to use " 
					      << kLRaveraged 
					      << endl ;
          once = false ; 
	}
      inside = kLRaveraged ;
      if ( drIII_df_data[inside-1][layer-1].side == kNODF ) 
	{
	  report( ERROR  , kFacilityString )  << "No DF types available "
					      << drIII_df_data[inside-1][layer-1].side 
					      << " "
					      << drIII_df_data[outside-1][layer-1].side
					      << endl ;
	  return 0.0 ;
	}
    }        
  else
    {
      inside = outside ;
    }
  const Singlelayer_df_data& df  = drIII_df_data[inside-1][layer-1] ;   
  //
  // Evaluate
  // 
  //
  // Find first entry in map with time component greater than current time
  // (note that velocity stored in map is difference with previous point in map...)
  //   Note:
  //            (lowpoint,highpoint) upper_bound(lowpoint) returns highpoint
  //            (lowpoint,highpoint) lower_bound(lowpoint) returns lowpoint
  //            out of range points returns zero values for functions! but
  //            can test on begin and end to see out of range behaviour
  //            this should be caught by outer two levels of ifs...but...
  //
  STL_MAP(double,Tseries )::const_iterator it = (df.t2d_map).upper_bound(time);
  if ( ( time <= df.min_time ) ||  ( it == (df.t2d_map).begin() ) )
    {
      if ( time <= df.min_time )
	{
	  //m_statistics->counter(57,"time out of range, low");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << "undertime " << time << " " << df.min_time << endl;
	    }
	}
      else 
	{
	  //m_statistics->counter(58,"time under range, iterator not found");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << "time under range, iterator not found"<< endl;
	    }
	}
      return 0.0;
    }
  //
  // Check if time is out of range of drift function, high
  //
  if ( ( time >= df.max_time ) || ( it == df.t2d_map.end() ) ) 
    {
      if ( time >= df.max_time ) 
	{
	  //m_statistics->counter(59,"time out of range, high");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << "overtime " << time << " " << df.max_time << endl;
	    }
	}
      else 
	{
	  //m_statistics->counter(60,"time over range, iterator not found");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << "time over range, iterator not found"<< endl;
	    }
	}
      //
      // Extrapolate past measured time range using average drift velocity and 
      // last measured point on drift function 
      //
      Distance = ( ( ( time - df.max_time ) * ( df.max_drift / df.max_time ) ) + df.max_drift ) ;
      return Distance;
    }
  // 
  // Since we are extrapolating from the upper edge of the bin 
  // this DeltaT should be negative!
  //
  Distance = (*it).second.distance();
  Time DeltaT = time - (*it).first;  
  if ( DeltaT > 0 ) 
    {
      //m_statistics->counter(61,"time out of drift bin, problem");		  
      if ( debugLevel[1] )
	{
	  report( INFO , kFacilityString )  << "time out of drift bin, problem" << endl;
	}
    }
  else
    {
      Distance = Distance + (*it).second.velocity()*DeltaT;
      //m_statistics->counter(62,"time to drift OK");
    }
  if ( Distance < 0 )
    {
      //m_statistics->counter(63,"negative unsigned drift distance (zeroed)");
      if ( debugLevel[1] )
	{
	  report( INFO , kFacilityString )  << "negative unsigned drift distance (set 0)" << Distance << endl;
	}
      Distance = 0 ;
    }
  if ( debugLevel[1] )
    {
      report( INFO , kFacilityString )  << time << " " << Distance << endl;
    }
  return Distance;
}
//--------------------------------------------------------------------------
DriftFunction::Time DataDriftFunction::drift2Time( Drift drift, 
						   int layer, 
						   int outside ) const
{ 
  // CHECK LAYER IN VALID RANGE
  if ( layer < 1 || layer > kmaxDRlayer )
    {
      report( WARNING, kFacilityString )  << "Illegal Layer # "
					  << layer 
					  << endl;
      return 0.0 ;
    }
  
  // CHECK AMBIGUITY IN VALID RANGE
  if ( outside != kLRaveraged && outside != kleftside && outside != krightside )
    {
      report( WARNING, kFacilityString )  << "Illegal Side Key "
					  << outside 
					  << endl;
      return 0.0 ;
    }
  
  Time drifttime;
  //
  // Use absolute value internally, side defines "L,R,L+R"
  //
  double cdrift = fabs ( drift );
  DABoolean once ( true ) ;
  //
  // If two-sided drift functions are not available 
  // use LR averaged or panic
  //
  int inside ;
  if ( drIII_df_data[outside-1][layer-1].side == kNODF ) 
    { 
      if ( once ) 
	{
	  report( WARNING, kFacilityString )  << "DF type unavailable: "
					      << drIII_df_data[outside-1][layer-1].side
					      << " trying to use " 
					      << kLRaveraged 
					      << endl ;
          once = false ; 
	}
      inside = kLRaveraged ;
      if ( drIII_df_data[inside-1][layer-1].side == kNODF ) 
	{
	  report( ERROR  , kFacilityString )  << "No DF types available "
					      << drIII_df_data[inside-1][layer-1].side 
					      << " "
					      << drIII_df_data[outside-1][layer-1].side
					      << endl ;
	  return 0.0 ;
	}
    }        
  else
    {
      inside = outside ;
    }
  const Singlelayer_df_data& df  = drIII_df_data[inside-1][layer-1] ;   
  //
  // Evaluate
  // 
  STL_MAP(double, Tseries)::const_iterator dfbin = (df.d2t_map).upper_bound(cdrift);
  
  if ( ( cdrift > df.max_drift ) || ( dfbin == df.d2t_map.end() ) ) 
    { 
      if ( cdrift > df.max_drift )
	{
	  //m_statistics->counter(64,"drift distance out of range");
	  if ( debugLevel[1] )
	    {
	      report( INFO , kFacilityString )  << " drift distance out of range " << cdrift << " " << df.max_time << endl;
	    }
	}
      else 
	{
	  //m_statistics->counter(65,"drift distance over range, iterator not found");
	  if ( debugLevel[1] )
	    {	 
	      report( INFO , kFacilityString )  << " drift distance over range, iterator not found  " 
						<< cdrift << " " << df.max_drift << endl;
	    }
	}
      drifttime = ( cdrift - df.max_drift ) / ( df.max_drift / df.max_time ) + df.max_time;
      return drifttime ;
    } 
  if ( dfbin == df.d2t_map.end() ) 
    { 
      //m_statistics->counter(66,"drift distance over range, iterator not found");
      if ( debugLevel[1] )
	{
	  report( INFO , kFacilityString )  << " drift distance under range, iterator not found  " 
					    << cdrift << " " << endl;
	}
      drifttime = 0.;
      return drifttime ;
    } 
  // the member data functions for Tseries are distance,velocity should really be
  // called "firstderivative" "secondderivative" :) in this context, confusing...
  drifttime =  (*dfbin).second.distance() ;
  if ( (*dfbin).second.velocity() != kIllegalValue )
    {
      //m_statistics->counter(67,"drift to time OK");
      Drift DeltaD = drift-(*dfbin).first;
      drifttime = drifttime + (*dfbin).second.velocity()*DeltaD;
    }
  else
    {
      //m_statistics->counter(68,"inverse velocity has illegal value");
      report( WARNING, kFacilityString )  << "inverse velocity has illegal value"<< endl;
    }
  return drifttime;
}
//--------------------------------------------------------------------------
DriftFunction::Time DataDriftFunction::drift2TimeCorr( Drift drift, 
				   		       int   layer,
						       float eAngle,
						       float zHit ) const
{
  // CHECK LAYER IN VALID RANGE
  if ( layer < 1 || layer > kmaxDRlayer )
    {
      report( WARNING, kFacilityString )  << "Illegal Layer # "
					  << layer 
					  << endl;
      return 0.0 ;
    }
  
  // CHECK AMBIGUITY IN VALID RANGE
  int ambiguity ;
  float ldrift ;
  if ( drift > 0 )
    {
      ambiguity = 1 ;
      ldrift = drift ;
    }
  else
    {
      ambiguity = 0 ;
      ldrift = -drift ;
    }
  
  // CHECK ENTRANCE ANGLE LIMITS 
  int anglebin  ; 
  float dangle  ;
  if ( eAngle < -kCorr_EAngleMax ) 
    { 
      anglebin = 0   ;  
      dangle   = 0.0 ;
    }
  else
    {
      if ( eAngle >= kCorr_EAngleMax )
	{ 
	  anglebin = kCorr_EAngleBins - 2 ;  // count from 0!
	  dangle   = 1.0                  ;  // interpolating between [-2] and [-1] (last two points)
	}
      else
	{ // assumes truncation runs 0.9999>0 
	  dangle   = (  ( eAngle + kCorr_EAngleMax ) / kCorr_EAngleBinSize ) ;
	  anglebin = (  (int) dangle                                       ) ;
	  dangle   = (  dangle - ( (float) anglebin )                      ) ;
	}
    }

  // CHECK DRIFT LIMITS
  int driftbin ; 
  float ddrift ;
  if ( ldrift >= kCorr_DriftMax )
    { 
      driftbin = kCorr_DriftBins - 2 ; // count from 0!
      ddrift   = 1.0                 ; // interpolating between [-2] and [-1] (last two points)
    }
  else
    { // assumes truncation runs 0.9999>0 
      ddrift   = (  ldrift / kCorr_DriftBinSize    ) ;
      driftbin = (  (int) ddrift                   ) ;
      ddrift   = (  ddrift - ( (float) driftbin )  ) ;
    }
  //
  // BiLinear Interpolation 
  //
  float result = (corrTime[layer-1][anglebin  ][ambiguity][driftbin  ]*( (1.0-ddrift)  * (1.0-dangle) )) +
                 (corrTime[layer-1][anglebin+1][ambiguity][driftbin  ]*( (1.0-ddrift)  *    dangle    )) +
                 (corrTime[layer-1][anglebin  ][ambiguity][driftbin+1]*(   ddrift      * (1.0-dangle) )) +
                 (corrTime[layer-1][anglebin+1][ambiguity][driftbin+1]*(   ddrift      *    dangle    )) ;

  if  ( debugLevel[6] )
    {
      report( INFO , kFacilityString )  << " result   " << result   << endl
					<< " driftbin " << driftbin << endl
					<< " layer    " << layer    << endl
					<< " anglebin " << anglebin << endl
					<< " ddrift   " << ddrift   << endl
					<< " dangle   " << dangle   << endl
					<< " drift    " << drift    << endl
					<< " angle    " << eAngle   << endl ;
    }
  //
  // sign convention of correctiondata appears to be flipped...
  // 
  return -result; 
}
//---------------------------------------------------------------------------
DriftFunction::Drift DataDriftFunction::time2DriftCorr( Time  time, 
  						        int   layer, 
						        int   ambiguity,
						        float eAngle,
						        float zHit ) const
{
  //
  // CHECK LAYER IN VALID RANGE
  // 
  if ( layer < 1 || layer > kmaxDRlayer )
    {
      report( WARNING, kFacilityString )  << "Illegal Layer # "
					  << layer 
					  << endl;
      return 0.0 ;
    }
  
  // CHECK AMBIGUITY IN VALID RANGE
  if ( ambiguity != 0 && ambiguity != 1 )
    {
      report( WARNING, kFacilityString )  << "Illegal Ambiguity "
					  << ambiguity
					  << endl;
      return 0.0 ;
    }
  
  // CHECK eAngle (ENTRANCE ANGLE) LIMITS 
  int anglebin ; 
  float dangle ;
  if ( eAngle < -kCorr_EAngleMax ) 
    { 
      anglebin = 0.0 ;  
      dangle   = 0.0 ;
    }
  else
    {
      if ( eAngle > kCorr_EAngleMax )
	{ 
	  anglebin = kCorr_EAngleBins - 2 ; // counts from 0!
	  dangle   = 1.0                  ;
	}
      else
	{ // assumes truncation runs 0.9999>0 
          dangle   = ( ( eAngle + kCorr_EAngleMax ) / kCorr_EAngleBinSize ) ;
	  anglebin = ( (int) dangle )                                       ;
	  dangle   = ( dangle - ( (float) anglebin ) )                        ; 
	}
    }

  // CHECK TIME LIMITS
  int timebin ; 
  float dtime ;
  if ( time < 0 ) 
    { 
      return 0.0 ;  
    }
  else
    {
      if ( time > kCorr_TimeMax )
	{ 
	  timebin = kCorr_TimeBins - 2 ; // counts from 0!
          dtime   = 1.0                ; 
	}
      else
	{ // assumes truncation runs 0.9999>0 
          dtime   = ( time / kCorr_TimeBinSize );
          timebin = ( (int) dtime )             ;
	  dtime   = dtime - ( (int) timebin )   ; 
	}
    }
  //
  // BiLinear Interpolation 
  //
  float result = (corrDrift[layer-1][anglebin  ][ambiguity][timebin  ]*( (1.0-dtime)  * (1.0-dangle) )) +
                 (corrDrift[layer-1][anglebin+1][ambiguity][timebin  ]*( (1.0-dtime)  *    dangle    )) +
                 (corrDrift[layer-1][anglebin  ][ambiguity][timebin+1]*(   dtime      * (1.0-dangle) )) +
                 (corrDrift[layer-1][anglebin+1][ambiguity][timebin+1]*(   dtime      *    dangle    )) ;

  if  ( debugLevel[6] )
    {
      report( INFO , kFacilityString )  << " result   " << result   << endl
					<< " timebin  " << timebin  << endl
					<< " layer    " << layer    << endl
					<< " anglebin " << anglebin << endl
					<< " dtime    " << dtime    << endl
					<< " dangle   " << dangle   << endl
					<< " time     " << time     << endl
					<< " angle    " << eAngle   << endl ;
    }
  //
  // sign convention of correctiondata appears to be flipped...
  // 
  return -result ;
}
//---------------------------------------------------------------------------
DriftFunction::Time DataDriftFunction::charge2TimeCorr( Time   time, 
							Charge Qcharge ) const
{
  int Qbin ; 
  if ( Qcharge < 0 ) 
    {
      return 0 ;
    }
  else
    {
      if ( Qcharge > kCorr_QchargeMax ) 
	{
          Qbin = kCorr_QBins - 1 ; 
	}
      else
	{
	  Qbin = int( Qcharge / kCorr_Qquantum ) ;
	}
    }
  return m_Qcorr[Qbin];
}




