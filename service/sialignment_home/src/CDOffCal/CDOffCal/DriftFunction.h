#if !defined(CDOFFCAL_DRIFTFUNCTION_H)
#define CDOFFCAL_DRIFTFUNCTION_H
// -*- C++ -*-
//
// Package:     <CDOffCal>
// Module:      DriftFunction
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Martin Lohner, Ken McLean
// Created:     Wed Mar  4 14:16:47 EST 1998
// $Id: DriftFunction.h,v 1.39 2001/05/30 17:20:34 urner Exp $
//
// Revision history
//
// $Log: DriftFunction.h,v $
// Revision 1.39  2001/05/30 17:20:34  urner
// changed kmaxWeightBins from 20 to 100
//
// Revision 1.38  2000/12/03 02:50:36  cdj
// put back DriftFunction::time2tdc used by MCRawDataProd
//
// Revision 1.37  2000/12/01 19:55:47  mclean
// Changed Zcorr to be dependent on layer and drift ( to account for
// readout end )
//
// Revision 1.36  2000/12/01 14:40:28  urner
// included time2charge correction to take care of time slewing using T0global BDL at present as container for the correction
//
// Revision 1.35  2000/10/31 23:09:19  mclean
// *** empty log message ***
//
// Revision 1.34  2000/03/07 22:27:10  mclean
// Added function adcwithped, just subtracts ped from adc, no
// gain correction (which is included properly in adc2Charge).
//
// Revision 1.33  2000/02/15 21:32:10  mclean
// Added a parameter to set the resolution (used for the fitting weight)
// i.e.
// param CDOffCal Resolution 10.0
// sets the resolution to 10 meters.
//
// Revision 1.32  2000/02/15 17:29:23  mclean
// Added CellID2DaqID function and corresponding member data.
//
// Revision 1.31  2000/02/14 23:37:14  mclean
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
// Revision 1.30  2000/02/02 15:36:17  cdj
// now dependent on DFDataHolder instead of DriftFunctionProxy
//
// Revision 1.29  2000/02/01 22:51:03  mclean
// This update introduces constants from the frame, i.e. from the official
// database or from CDJ FileSourceFormat ascii files as the user chooses.
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
//       **DBDRDriftData
// 	 *DBDRadcpedestal
//       **DBDRt0channel
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
//   DriftFunction.h - added const functions
//        adcpedetal(CellID), adcpedestalRMS(CellID),tdctzero(CellID)
//   DriftFunctionProxy.h - added member data and data-access functions
//        for all the BDL/constants objects.
//
// Revision 1.28  2000/01/25 18:49:42  mclean
// I added provisions for easy access to pedestal files meant for the
// use of constants-production experts/specialist. This is not intended for
// general use.
//
// If one creates a file:
//                    channelPeds.v01
// in the directory you are running suez from this will OVERRIDE any other source
// (database or ascii source format neither of which is implemented) of these
// constants. channelPeds.v01 is meant to represent the constants defined in
// DRadcpedestal.bdl and should be in the format of ordered triples
//             (channel#,ped,pedestal_rms)
// i.e. the same as the normal ascii format without headers (this can be
// changed if requested).
//
// The record ordering (i.e. channel#) is not checked and is assumed to be
// 1-1 increasing CELL#, i.e. that appropriate after the DRDaqCellIDMap
// mis-cabling map has been applied to the raw DAQID.
//
// Revision 1.27  1999/12/15 22:50:12  mclean
// The HitQuality entries here were supposed to have been moved to HitQuality.h.
// But thrice-cursed CVS clobbered them back into DriftFunction.h file because it
// thought a newer version existed in the repository, didnt warn, it thought
// things were internally consistent ( a nice big block of code ).
// This is why I really hate committing a slightly tweaked version for
// others convenience when I am working on a major update in parallel.
//
// It took about an hour to force CVS to take the update, based an older "cvs co".
// I have to "cvs co" the old version and copy files in, and then it lost lots of
// "cvs add" and other changes... It also died refusing to commit a file that
// was >unchanged< from the antique release...and now 4 days later still things
// are not right.
//
// Revision 1.26  1999/12/15 19:41:46  mclean
// Some bug fixes, made get-parameter functions in CDOffCalProducer
// inline functions, added channel t0s to DriftFunction.h,
// added DebugLevel parameter to CDOffCalProducer.
//
// Revision 1.25  1999/12/07 23:29:36  mclean
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
//
// Revision 1.1.1.1  1998/03/04 20:41:49  mkl
// imported CDOffCal sources
//

// system include files
// user include files
#include "CommandPattern/Parameter.h"
#include "Experiment/cleo_math.h"
#include "FrameAccess/FATable.h"
#include "RawData/CollatedRawDRHit.h"
#include "CalibratedData/Tseries.h"

// forward declarations
class CDOffCalProducer   ;
class DFDataHolder       ;
class Statistics         ;
class CalibratedDRHit    ;
class ADRSenseWireStore  ;

class DriftFunction
{
public:
  // ---------- constants, enums and typedefs --------------
  typedef double  Velocity;    // Drift velocity in meters/picosecond
  typedef double  Drift;       // Drift distance in meters
  typedef double  Time;        // Drift Time in picoseconds
  typedef double  Charge;      // Charge in picocoulombs 
  
  enum ArrayLengths {      kmaxDRlayer              =   47, 
			   kTotalDRWires            = 9796, 
			   kmaxWireInLayer          =  348,     
                           kmaxDFside               =    3,
                           kmaxWeightBins           =  100,
                           kAmbiguities             =    2,
                           kmaxQcorrBins            =   80};

  enum DriftFunctionTypes {kDFType_NAIVE,
			   kDFType_HePr,
                           kDFType_HePr_data1};

  //
  // Drift Function Side Codes
  // 
  enum DriftFunctionSide  {kNODF        = 0        ,
                           kLRaveraged  = 1        ,
                           kleftside    = 2        , 
                           krightside   = 3        }; 

  // Constants for calibration
  static const double kdefault_MinTightTime,kdefault_MaxTightTime;
  static const double kdefault_MinLooseTime,kdefault_MaxLooseTime;
  static const double kdefault_GlobalT0,kdefault_TDCQuantum;
  static const double kclight,kdefault_Resolution,kdefault_SignalProp;
  const static UInt16 kHighTDC;
  
  // ---------- Constructors and destructor ----------------
  DriftFunction(CDOffCalProducer* Producer,DFDataHolder* iDataHolder);
  virtual ~DriftFunction();
  
  // ---------- const member functions ---------------------
  
  virtual Drift  time2Drift( Time time, 
			     int layer, 
			     int side) const = 0;
  
  virtual Time   drift2Time( Drift drift, 
			     int layer, 
			     int side) const = 0;
  
  DriftFunction::Drift  time2Drift( DriftFunction::Time time, 
				    int layer) const;

  DriftFunction::Time   drift2Time( DriftFunction::Drift drift, 
				    int layer) const;

  DriftFunction::Drift  time2SignedDrift( DriftFunction::Time time, 
					  int layer,
					  int ambiguity) const;
  
  DriftFunction::Time   signeddrift2Time( DriftFunction::Drift drift, 
					  int layer ) const;
  
  DriftFunction::Drift  time2SignedDrift( DriftFunction::Time time, 
					  int layer,
					  int ambiguity,
					  float eAngle, 
					  float Zhit ) const;
  
  DriftFunction::Time   signeddrift2Time( DriftFunction::Drift drift, 
					  int layer, 
					  float eAngle, 
					  float Zhit ) const;

  virtual Drift time2DriftCorr( DriftFunction::Time time, 
				int layer,
				int ambiguity,
				float eAngle, 
				float Zhit ) const = 0;
 
  virtual Time drift2TimeCorr( DriftFunction::Drift drift, 
			       int layer, 
			       float eAngle, 
			       float Zhit ) const = 0;
  
  virtual Time charge2TimeCorr ( DriftFunction::Time time, 
				 DriftFunction::Charge Qcharge ) 
                                 const = 0;

  DriftFunction::Time Zcorr( int layer, float dZ) const;

  Tseries time2DriftSeries( DriftFunction::Time time, 
			    int layer ) const;

  Tseries time2DriftSeries( DriftFunction::Time time, 
			    int layer,
			    int ambiguity) const;

  virtual Charge badwiremap(UInt16 wire) const;
  virtual Charge adcwithped(UInt16 wire, DRDatum::DigiType adc ) const;
  virtual Charge adc2Charge(UInt16 wire, DRDatum::DigiType adc ) const;
  virtual Charge adcpedestal(UInt16 wire) const;
  virtual Charge adcpedestalRMS(UInt16 wire) const;
  virtual Charge adcgain(UInt16 wire) const;
  virtual Charge adcgainRMS(UInt16 wire) const;
  virtual Charge tdcped(UInt16 wire) const;
  virtual Charge tdcpedRMS(UInt16 wire) const;
  virtual Charge tdcslope(UInt16 wire) const;
  virtual Charge tdcslopeRMS(UInt16 wire) const;
  //
  // resolution functions with layers of fitting weights added 
  //
  virtual Drift  resolution() const;
  virtual Drift  resolution(int layer) const;
  virtual Drift  resolution(int layer,Drift drift) const;
  virtual Drift  resolution(int layer,Drift drift,float costheta) const;
  virtual Drift  resolution(int layer,Drift drift,float costheta,float phi) const;

  virtual Time     tdc2Time(UInt16 wire, DRDatum::DigiType tdc ) const;
  virtual Time     ZShift(int layer) const;
  virtual Time     tdctzero(UInt16 wire) const;
  
  virtual DRDatum::DigiType charge2adc( UInt16 wire, Charge PulseHeight ) const;
  
  virtual DRDatum::DigiType time2tdc( UInt16 wire, Time time ) const;
  virtual DRDatum::DigiType time2tdc( UInt16 wire, 
				      Time time, 
				      double radiusOfLayer ) const;
  UInt16 daqID2CellID(UInt16 daqID) const;
  UInt16 CellID2daqID(UInt16 CellID) const;


//////////////////////////////////////////////////////////////////////
// makeCalibratedHit( raw hit, id, time offset, senseWireStore)
//
//   Construct a CalibratedDRHit from the raw hit using the tOffset.
//         (tOffset is subtracted from the time before processing)
//         the default tOffset is 0.0 ps.
//
//        id is the ID number for this hit
//
//       NOTE: THIS RETURNS A *NEW'D* CalibratedDRHit
//
  CalibratedDRHit* makeCalibratedHit( 
				     const CollatedRawDRHit& rawhit, 
				     int             id, 
				     const ADRSenseWireStore* senseWireStore,
				     double          tOffset = 0.0
				     ) const;

protected: 
     
      Statistics *m_statistics;
      DFDataHolder* m_dataHolder   ;
      CDOffCalProducer*   m_CDOffCal;

      Time   m_Qcorr[kmaxQcorrBins];
      Time   m_channelT0[kTotalDRWires];
      Charge m_channelPed[kTotalDRWires];
      Charge m_channelPedRMS[kTotalDRWires];
      UInt16 m_daqmap[kTotalDRWires];
      UInt16 m_invdaqmap[kTotalDRWires];
      UInt32 m_quality[kTotalDRWires];
      Charge m_adcgain[kTotalDRWires];
      Charge m_adcgainRMS[kTotalDRWires];
      Time   m_tdcslope[kTotalDRWires];
      Time   m_tdcslopeRMS[kTotalDRWires];
      Time   m_tdcped[kTotalDRWires];
      Time   m_tdcpedRMS[kTotalDRWires];
      float  m_LayerWeight[kmaxDRlayer];
      float  m_DriftWeight[kmaxDRlayer][kmaxWeightBins] ;
      float  m_ThetaWeight[kmaxDRlayer][kmaxWeightBins] ;
      float  m_PhiWeight  [kmaxDRlayer][kmaxWeightBins] ;
      float  m_DriftWeight_Min ;
      float  m_DriftWeight_Max ;
      float  m_DriftWeight_BinSize ;
      float  m_ThetaWeight_Min ;
      float  m_ThetaWeight_Max ;
      float  m_ThetaWeight_BinSize ;
      float  m_PhiWeight_Min   ;
      float  m_PhiWeight_Max   ;
      float  m_PhiWeight_BinSize   ;
      int    m_DriftWeight_Nbins ;
      int    m_ThetaWeight_Nbins ;
      int    m_PhiWeight_Nbins   ;
 
      //double m_Mintime;
      //double m_Maxtime;
      //double m_TDCQuantum;
      //double m_GlobalT0;

      DABoolean debugLevel[20];

private:


  // ---------- Constructors and destructor ----------------
  DriftFunction( const DriftFunction& );
  const DriftFunction& operator=( const DriftFunction& );
};

#endif /* CDOFFCAL_DRIFTFUNCTION_H */






