#if !defined(CALIBRATEDDATA_CALIBRATEDDRHIT_H) 
#define CALIBRATEDDATA_CALIBRATEDDRHIT_H 
// -*- C++ -*- 
// 
// Package: <CalibratedData> 
// Module:  CalibratedDRHit 
// 
/**\class CalibratedDRHit CalibratedDRHit.h CalibratedData/CalibratedDRHit.h

 Description: Calibrated DR Anode Wire data.
 
 Usage: The time and distance data access interface is defined in the base 
        classes, CalibratedHit and CalibratedTimeHit.
  
*/

// 
// Author: Jon Thaler 
// Created: Mon Dec 15 11:15:39 EST 1997 
//          Thu Apr 2 1998 replace Calib* with Calibrated
//  
// $Id: CalibratedDRHit.h,v 1.14 2001/11/29 22:11:54 bkh Exp $
//
// $Log: CalibratedDRHit.h,v $
// Revision 1.14  2001/11/29 22:11:54  bkh
// Add raw charge and time to object
// Add raw charge and time to storage helper
// Fix operator== object to not just use identifier
//
// Revision 1.13  2000/03/03 15:13:31  lyon
// Added which TDC was picked
//
//
//            
// Revision history
// 
// Revision 1998/03/11  Inga Karliner
//   include STLUtility not Utility; 
//   ADC, TDC were typedef C3UINT32 changed to UInt32 
// Revision 1998/03/13 Inga Karliner 
//   remove all RawHit information
//   move deviceType() function to CalibratedDRHit, return DR
//   move PicoCoul from CalibratedHit to CalibratedDRHit
//   move charge from base class
//   move to CalibratedDRHitProxy.cc data extraction from RawHit + conversions  
//   comment out PicoCoul adcToCharge   (ADC)     const;
//   comment out PicoSec  tdcToTime     (TDC)     const;
//   comment out Meters   timeToDistance(PicoSec) const;
//   eliminate multiple TDC/CalibratedHit
//   add leftDistance(), rightDistance(), resolution 
//   add DABoolean singleTDC = true for single TDC hit
//   add Experiment/types.h
// Revision 1998/03/27 Inga Karliner
//   Add hit quality word QualHit, with fields to be defined 
//   Replace resolution with weight
// Revision  4/15/98 Inga Karliner
//   replace typedef float-> double PicoCoul
//   replace int-> UInt32 wire, layer, wireInLyr 
//   redefine typedef long QualHit ( UInt64)
// Revision  4/30/98 Inga Karliner
//   code changes due to new RawData structure by Brian K. Heltsley
//   RawDRHit                   ===> CollatedRawDRHit
//   UInt32  wire,  layer, wireInLyr ==> Uint16 wire,  layer, wireInLyr ;
// Revision 10/09/98 Inga Karliner
//   documentation fixed: wireInLyr starts at 0, not 1
// Revision 12/2/98  Inga karliner
//   add set functions 
//--------------------+
// user include files |
//--------------------+
#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include "CalibratedData/Tseries.h"
#include "CalibratedData/CalibratedTimeHit.h"
#include "RawData/DRDatum.h"

//---------------------------------------------------------------+
// forward declarations                                          |
//  fwd_vector.h obviates the inclusion of the STL vector class. |
//---------------------------------------------------------------+
#include "STLUtility/fwd_vector.h"

class CalibratedDRHit : public CalibratedTimeHit
{
public:
  //------------------------------------------------------+
  // constants, enums, structures and typedefs            |
  //  PicoCoul    The deposited charge, in "happy units". |
  //  QualHit     quality word, typedef long = UInt64     |
  //------------------------------------------------------+

      typedef DRDatum::DigiType           DigiType      ;
      typedef pair< DigiType , DigiType > RawChargeTime ;
  
  typedef double PicoCoul;
  typedef UInt32   QualHit;
//struct QualHit { DABoolean oneTDC : 1 ; DABoolean bitTwo : 1 ; };

  //-----------------------------+
  // Constructors and destructor |
  //-----------------------------+
  CalibratedDRHit(UInt16 wire,
		  UInt16 layer, 
		  UInt16 wireInLyr,
		  PicoCoul charge,
		  PicoSec time,
		  Tseries distanceSeries,
		  double  weight,
		  QualHit qualHit,
		  UInt16  nTDC, 
                  PicoSec earlyGap,
                  PicoSec lateGap,
		  UInt16  whichTDC,
		  Identifier id);
  CalibratedDRHit(UInt16 wire,
		  UInt16 layer, 
		  UInt16 wireInLyr,
		  PicoCoul charge,
		  PicoSec time,
		  Tseries distanceSeries,
		  double  weight,
		  QualHit qualHit,
		  DABoolean singleTDC, 
		  Identifier id);
  //
  CalibratedDRHit(const CalibratedDRHit&);
  virtual ~CalibratedDRHit();

  //! Get overall DR wire number (1-9796) 
  UInt16 wire() const;

  //! Get DR layer (1-47)
  UInt16 layer() const;

  //! Get wire # in DR layer (e.g. 0 to 63 in layer 1, 0 to 347 in layer 47)
  UInt16 wireInLyr() const;

  //! Get charged deposited (in pC)
  virtual PicoCoul charge() const;

  //! Get drift time (in ps)
  virtual PicoSec time() const;

  //! Get drift time series (use time() instead)
  const Tseries& tSeries() const;

  //! Get drift distance (in m)
  virtual Meters distance() const;

  //! Get fitting weight
  virtual double weight() const;

  //! Get fit quiality word
  QualHit qualHit() const;

  //! Get number of time TDCs for this hit
  virtual UInt16 nTDC() const;  

  //! The which TDC was the best
  //!   whichTDC tells you the element in the TDC list of the CollatedRawDRHit
  //!   (not the TDC itself)
  virtual UInt16 whichTDC() const;

  //! Return true if there was only one time TDC for this hit
  virtual DABoolean singleTDC() const;  

  //! Return the difference between the chosen time and the
  //! next closest earlier time
  virtual PicoSec earlyGap() const;  

  //! Return the difference between the chosen time and the
  //! next closest later time
  virtual PicoSec lateGap() const;  

  //! Return the device type for this hit (should be DR)
  inline DeviceType deviceType() const; 

  RawChargeTime rawChargeTime() const ;
  DigiType      rawCharge()     const ;
  DigiType      rawTime()       const ;

  //! Extrapolate the drift distance for a time offset 
  //!  (is inaccurate for large offsets)
  Meters extrapolatedDistance(PicoSec deltaT) const;

  inline void setWire       ( UInt16 wire);
  inline void setLayer      ( UInt16 layer);
  inline void setWireInLayer( UInt16 wireInLayer);
  inline void setCharge     (PicoCoul charge  );
  inline void setTime       (PicoSec time  );
  inline void setTseries    (Tseries distanceSeries  );
  inline void setWeight     (double weight  );
  inline void setQualHit    (QualHit qualHit  );
  inline void setSingleTDC  (DABoolean singleTDC );
  inline void setnTDC       ( UInt16 nTDC );
  inline void setearlyGap   ( PicoSec earlyGap );
  inline void setlateGap    ( PicoSec lateGap );
  //  inline void setIdentifier (Identifier id  );

  void setRawChargeTime( const RawChargeTime& aRawChargeTime ) ;

  void printCDRH(ostream&) const;

  // Comparison operator (checks Identifier)
  DABoolean operator==(const CalibratedDRHit& rhs) const;
  DABoolean operator!=(const CalibratedDRHit& rhs) const;
  
protected:
  // protected member functions

  //------------------------+
  // assignment operator(s) |
  //------------------------+
  CalibratedDRHit& operator = (const CalibratedDRHit&);

 private:
  //-----------------------------+
  // Constructors and destructor |
  //-----------------------------+
  CalibratedDRHit();

  //----------------------------------------------------------------------+
  // Data members                                                         |
  //  m_Wire       Wire in DR (1-9796)                                    |
  //  m_Layer      Layer in DR (1-47)                                     |
  //  m_WireInLyr  Wire in DR Lyr (0 to 63 in lyr 1; 0 to 347 in Lyr 47)  |
  //  m_Charge     Deposited charge in picoCoulombs                       |
  //  m_Weight     Weight                                                 |
  //  m_Time       Drift time in PicoSeconds.                             |
  //  m_DistanceSeries  A Taylor series for the drift distance            |
  //  m_QualHit    long word (UInt64) defining hit quality                | 
  //  m_nTDC       number of TDCs in hit                                  |
  //  m_earlyGap   time gap between TDC used and next earlier TDC         |
  //  m_lateGap    time gap between TDC used and next latest TDC          |
  //               the gap variables are positive if there are earlier    |
  //               or later hit as appropriate, if there is no appropriate|
  //               hit they are set to a large negative number            |
  //----------------------------------------------------------------------+

  UInt16           m_Wire;
  UInt16           m_Layer;
  UInt16           m_WireInLyr;
  PicoCoul         m_Charge;
  PicoSec          m_Time;
  Tseries          m_DistanceSeries;
  double           m_Weight;
  QualHit          m_QualHit;
  UInt16           m_nTDC;  
  PicoSec          m_earlyGap;  
  PicoSec          m_lateGap;  
  UInt32           m_whichTDC;

  RawChargeTime    m_rawChargeTime ;

};

// inline function definitions
inline ostream& operator<< (ostream& os, const CalibratedDRHit& hit)
{ hit.printCDRH(os);
  return os;
}

inline CalibratedDRHit::DeviceType CalibratedDRHit::deviceType() const
{ return DR; }

inline
const Tseries&
CalibratedDRHit::tSeries() const { return m_DistanceSeries; }

inline void CalibratedDRHit::setWire( UInt16 wire)
{ m_Wire = wire; }

inline void CalibratedDRHit::setLayer( UInt16 layer)
{ m_Layer = layer; }

inline void CalibratedDRHit::setWireInLayer( UInt16 wireInLayer)
{ m_WireInLyr = wireInLayer; }

inline void CalibratedDRHit::setCharge(CalibratedDRHit::PicoCoul charge)
{ m_Charge = charge; }

inline void CalibratedDRHit::setTime(CalibratedTimeHit::PicoSec time  )
{ m_Time = time; }

inline void CalibratedDRHit::setTseries(Tseries distanceSeries  )
{ m_DistanceSeries = distanceSeries; }

inline void CalibratedDRHit::setWeight(double weight)
{ m_Weight = weight; }

inline void CalibratedDRHit::setQualHit(CalibratedDRHit::QualHit qualHit )
{ m_QualHit = qualHit; }

inline void CalibratedDRHit::setSingleTDC(DABoolean singleTDC ) 
{ m_nTDC = 1; }

inline void CalibratedDRHit::setnTDC(UInt16 nTDC) 
{ m_nTDC = nTDC; }

inline void CalibratedDRHit::setearlyGap(PicoSec earlyGap) 
{ m_earlyGap = earlyGap; }

inline void CalibratedDRHit::setlateGap(PicoSec lateGap) 
{ m_lateGap = lateGap; }

inline void 
CalibratedDRHit::setRawChargeTime(
   const CalibratedDRHit::RawChargeTime& aRawChargeTime )
{
   m_rawChargeTime = aRawChargeTime ;
}

inline CalibratedDRHit::RawChargeTime
CalibratedDRHit::rawChargeTime() const
{
   return m_rawChargeTime ;
}

inline CalibratedDRHit::DigiType
CalibratedDRHit::rawCharge() const
{
   return m_rawChargeTime.first ;
}

inline CalibratedDRHit::DigiType
CalibratedDRHit::rawTime() const
{
   return m_rawChargeTime.second ;
}

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "CalibratedData/Template/CalibratedDRHit.cc"
//#endif

#endif /* CALIBRATEDDATA_CALIBRATEDDRHIT_H */















