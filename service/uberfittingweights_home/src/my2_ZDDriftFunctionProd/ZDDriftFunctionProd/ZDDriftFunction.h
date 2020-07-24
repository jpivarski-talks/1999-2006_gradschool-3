#if !defined(ZDDRIFTFUNCTION_DRIFTFUNCTION_H)
#define ZDDRIFTFUNCTION_DRIFTFUNCTION_H
// -*- C++ -*-
//
// Package:     <ZDDriftFunctionProd>
// Module:      ZDDriftFunction
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Guangshun Huang
// Created:     Fri Jan 3, 2003
//
// Revision history
// $Log: ZDDriftFunction.h,v $
// Revision 1.2  2003/10/10 17:36:43  mccann
// Totally vamped the fittingweights representation and delivery to use the new ZDWeight2* constants
//
// Revision 1.1.1.1  2003/06/26 19:02:49  huanggs
// imported ZDDriftFunctionProd source
//
// Revision 1.1  2003/04/24 18:08:17  huanggs
// add new files
//

// system include files
// user include files
#include "CommandPattern/Parameter.h"
#include "Experiment/cleo_math.h"
#include "FrameAccess/FATable.h"
#include "RawData/CollatedRawZDHit.h"
#include "CalibratedData/Tseries.h"

// forward declarations
class ZDDriftFunctionProd   ;
class ZDDFDataHolder       ;
class ZDStatistics         ;
class CalibratedZDHit    ;
class AZDSenseWireStore  ;

class ZDDriftFunction
{
public:
  // ---------- constants, enums and typedefs --------------
  typedef double  Velocity;    // Drift velocity in meters/picosecond
  typedef double  Drift;       // Drift distance in meters
  typedef double  Time;        // Drift Time in picoseconds
  typedef double  Charge;      // Charge in picocoulombs 
  typedef double  Resolution;  // Drift resolution in scaled length (NOT scaled length^2!)

      typedef float*   floatPtr;
      typedef float**  floatPtrPtr;
      typedef float*** floatPtrPtrPtr;
  
  enum ArrayLengths {      kmaxZDlayer              =   6,
			   kTotalZDWires            = 300, 
			   kmaxWireInLayer          =  66,     
                           kmaxDFside               =   3,
                           kAmbiguities             =   2,
                           kmaxQcorrBins            =  80};

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
  ZDDriftFunction(ZDDriftFunctionProd* Producer,ZDDFDataHolder* iDataHolder);
  virtual ~ZDDriftFunction();
  
  // ---------- const member functions ---------------------
  
  virtual Drift  time2Drift( Time time, 
			     int layer, 
			     int side) const = 0;
  
  virtual Time   drift2Time( Drift drift, 
			     int layer, 
			     int side) const = 0;
  
  ZDDriftFunction::Drift  time2Drift( ZDDriftFunction::Time time, 
				    int layer) const;

  ZDDriftFunction::Time   drift2Time( ZDDriftFunction::Drift drift, 
				    int layer) const;

  ZDDriftFunction::Drift  time2SignedDrift( ZDDriftFunction::Time time, 
					  int layer,
					  int ambiguity) const;
  
  ZDDriftFunction::Time   signeddrift2Time( ZDDriftFunction::Drift drift, 
					  int layer ) const;
  
  ZDDriftFunction::Drift  time2SignedDrift( ZDDriftFunction::Time time, 
					  int layer,
					  int ambiguity,
					  float eAngle, 
					  float Zhit ) const;
  
  ZDDriftFunction::Time   signeddrift2Time( ZDDriftFunction::Drift drift, 
					  int layer, 
					  float eAngle, 
					  float Zhit ) const;

  virtual Drift time2DriftCorr( ZDDriftFunction::Time time, 
				int layer,
				int ambiguity,
				float eAngle, 
				float Zhit ) const = 0;
 
  virtual Time drift2TimeCorr( ZDDriftFunction::Drift drift, 
			       int layer, 
			       float eAngle, 
			       float Zhit ) const = 0;
  
  virtual Time charge2TimeCorr ( ZDDriftFunction::Time time, 
				 ZDDriftFunction::Charge Qcharge ) 
                                 const = 0;

  ZDDriftFunction::Time Zcorr( int layer, float dZ) const;

  Tseries time2DriftSeries( ZDDriftFunction::Time time, 
			    int layer ) const;

  Tseries time2DriftSeries( ZDDriftFunction::Time time, 
			    int layer,
			    int ambiguity) const;

  virtual Charge badwiremap(UInt16 wire) const;
  virtual Charge adcwithped(UInt16 wire, ZDDatum::DigiType adc ) const;
  virtual Charge adc2Charge(UInt16 wire, ZDDatum::DigiType adc ) const;
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
  virtual double soft_bin(double value, double min, double max, int nbins) const;
  virtual double sbDrift(double absDrift) const;
  virtual double sbEntAng(double sinalpha) const;
  virtual double sbTheta(double cosTheta) const;
  virtual double sbPhi(double phi) const;
  virtual int close_low(double sb, int nbins) const;
  virtual int clDrift(double sb) const;
  virtual int clEntAng(double sb) const;
  virtual int clTheta(double sb) const;
  virtual double interp(double sb, int cl, double fl, double fh) const;
  virtual double intEntAng(int l, int d, double sinalpha) const;
  virtual double intDriftEntAng(int l, double absDrift, double sinalpha) const;
  virtual double intDriftOnly(int l, double absDrift) const;
  virtual double standardTheta(double cosTheta) const;
  virtual double intTheta(int l, double cosTheta) const;
  virtual double intPhi(int l, double phi) const;
  virtual Resolution resolution() const;
  virtual Resolution resolution(int layer) const;
  virtual Resolution resolution(int layer,Drift drift) const;
  virtual Resolution resolution(int layer,Drift drift,double costheta,double phi,double sinalpha) const;

  virtual Time     tdc2Time(UInt16 wire, ZDDatum::DigiType tdc ) const;
  virtual Time     ZShift(int layer) const;
  virtual Time     tdctzero(UInt16 wire) const;
  
  virtual ZDDatum::DigiType charge2adc( UInt16 wire, Charge PulseHeight ) const;
  
  virtual ZDDatum::DigiType time2tdc( UInt16 wire, Time time ) const;
  virtual ZDDatum::DigiType time2tdc( UInt16 wire, 
				      Time time, 
				      double radiusOfLayer ) const;
  UInt16 daqID2CellID(UInt16 daqID) const;
  UInt16 CellID2daqID(UInt16 CellID) const;


//////////////////////////////////////////////////////////////////////
// makeCalibratedHit( raw hit, id, time offset, senseWireStore)
//
//   Construct a CalibratedZDHit from the raw hit using the tOffset.
//         (tOffset is subtracted from the time before processing)
//         the default tOffset is 0.0 ps.
//
//        id is the ID number for this hit
//
//       NOTE: THIS RETURNS A *NEW'D* CalibratedZDHit
//
  CalibratedZDHit* makeCalibratedHit( 
				     const CollatedRawZDHit& rawhit, 
				     int             id, 
				     const AZDSenseWireStore* senseWireStore,
				     double          tOffset = 0.0
				     ) const;

protected: 
     
      ZDStatistics *m_statistics;
      ZDDFDataHolder* m_dataHolder   ;
      ZDDriftFunctionProd*   m_ZDDriftFunctionProd;

      Time   m_Qcorr[kmaxQcorrBins];
      Time   m_channelT0[kTotalZDWires];
      Charge m_channelPed[kTotalZDWires];
      Charge m_channelPedRMS[kTotalZDWires];
      UInt16 m_daqmap[kTotalZDWires];
      UInt16 m_invdaqmap[kTotalZDWires];
      UInt32 m_quality[kTotalZDWires];
      Charge m_adcgain[kTotalZDWires];
      Charge m_adcgainRMS[kTotalZDWires];
      Time   m_tdcslope[kTotalZDWires];
      Time   m_tdcslopeRMS[kTotalZDWires];
      Time   m_tdcped[kTotalZDWires];
      Time   m_tdcpedRMS[kTotalZDWires];
      floatPtrPtrPtr m_LayerDriftEntAngWeight;
      floatPtrPtr    m_LayerThetaWeight;
      floatPtrPtr    m_LayerPhiWeight;
      floatPtr       m_LayerSummary;
      double m_WeightDriftMin;
      double m_WeightDriftMax;
      int    m_WeightDriftNBins;
      double m_WeightEntAngMin;
      double m_WeightEntAngMax;
      int    m_WeightEntAngNBins;
      double m_WeightThetaMin;
      double m_WeightThetaMax;
      int    m_WeightThetaNBins;
      int    m_WeightPhiNBins;
      int    m_WeightMiddleEntAngBin;

      //double m_Mintime;
      //double m_Maxtime;
      //double m_TDCQuantum;
      //double m_GlobalT0;

      DABoolean debugLevel[20];

private:


  // ---------- Constructors and destructor ----------------
  ZDDriftFunction( const ZDDriftFunction& );
  const ZDDriftFunction& operator=( const ZDDriftFunction& );
};

#endif /* ZDDRIFTFUNCTION_DRIFTFUNCTION_H */
