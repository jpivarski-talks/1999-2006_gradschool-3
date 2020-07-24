// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CalibratedDRHit
//
// Description:    Calibrated DR hit constructors & member functions.
//
// Implementation: See CalibratedDRHit.h
//
// Author:      Jon Thaler
// Created:     Mon Dec 15 20:27:14 EST 1997
//              Thu Apr 2 1998 replace Calib* with Calibrated*
// 
// $Id: CalibratedDRHit.cc,v 1.12 2003/01/27 19:56:07 cdj Exp $
//
// $Log: CalibratedDRHit.cc,v $
// Revision 1.12  2003/01/27 19:56:07  cdj
// get iomanip.h from C++Std package
//
// Revision 1.11  2001/12/06 23:19:59  bkh
// Correct copy constructor
//
// Revision 1.10  2001/11/29 22:12:10  bkh
// Add raw charge and time to object
// Add raw charge and time to storage helper
// Fix operator== object to not just use identifier
//
// Revision 1.9  2000/03/03 15:16:19  lyon
// Changes for storage helpers
//
//
//-------------------+
// Environment flags |
//-------------------+
#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include "Experiment/report.h"

//----------------------+
// system include files |
//----------------------+
#include "C++Std/iomanip.h" 
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

//----------------------+
// system include files |
//----------------------+
#include <stdlib.h> // for exit

//--------------------+
// user include files |
//--------------------+
#include "CalibratedData/CalibratedDRHit.h"

//-------------+
// STL classes |
//-------------+
#include <vector>

//-------------------------------+
// constants, enums and typedefs |
//-------------------------------+
static const char* const kFacilityString = "CalibratedData.CalibratedDRHit" ;

//---------------------------------------------------------------------------+
// constructors and destructor                                               |
//  CalibratedDRHit(const UInt16& wire,..)                                   |
//                      Construct CalibratedDRHit from data (in proxy)       |
//  CalibratedDRHit(const CalibratedDRHit&)                                  |
//                     Copy a CalibratedDRHit from another CalibratedDRHit.  |
// ~CalibratedDRHit()   Delete memory obtained with "new".                   |
// DR is the device type, enumerated in CalibratedHit.h .                    |
//---------------------------------------------------------------------------+

CalibratedDRHit::CalibratedDRHit( UInt16   wire,
				  UInt16   layer, 
				  UInt16   wireInLyr,
				  PicoCoul charge,
				  PicoSec  time,
				  Tseries distanceSeries,
				  double   weight,
				  QualHit  qualHit,
				  DABoolean  singleTDC,
				  Identifier id)
     : CalibratedTimeHit(id),
	    m_Wire      (wire),
	    m_Layer     (layer),
	    m_WireInLyr (wireInLyr),
            m_Charge    (charge),
            m_Time      (time),
            m_DistanceSeries  (distanceSeries),
	    m_Weight(weight),
            m_nTDC(1),
            m_earlyGap(-1.),
            m_lateGap(-1.),
            m_whichTDC(0),
	    m_QualHit(qualHit),
       m_rawChargeTime(0,0)
{}
CalibratedDRHit::CalibratedDRHit( UInt16   wire,
				  UInt16   layer, 
				  UInt16   wireInLyr,
				  PicoCoul charge,
				  PicoSec  time,
				  Tseries distanceSeries,
				  double   weight,
				  QualHit  qualHit,
				  UInt16   nTDC,
				  PicoSec  earlyGap,
				  PicoSec  lateGap,
				  UInt16   whichTDC,
				  Identifier id)
     : CalibratedTimeHit(id),
	    m_Wire      (wire),
	    m_Layer     (layer),
	    m_WireInLyr (wireInLyr),
            m_Charge    (charge),
            m_Time      (time),
            m_DistanceSeries  (distanceSeries),
	    m_Weight(weight),
            m_nTDC(nTDC),
            m_earlyGap(earlyGap),
            m_lateGap(lateGap),
            m_whichTDC(whichTDC),
	    m_QualHit(qualHit),
       m_rawChargeTime(0,0)
{}
//                    
//
CalibratedDRHit::~CalibratedDRHit()
{}

//------------------------------------------------------------------------+
// Member functions                                                       |
// Data access: (inline)                                    Declared in:  |
  //  wire           DR wire  (1-9796)                     CalibratedDRHit|
  //  layer          DR layer (1-47)                             "        |
  //  wireInLyr      wire # in DR layer(0-63 Lyr1; 0-347 Lyr47   "        |
  //  charge         the charge deposited, in pC.          CalibratedDRHit|
  //  time           the time                            CalibratedTimeHit|
  //  distance       the avg L-R distance ?                    "          |
  //  leftDistance   the left distance                         "          | 
  //  rightDistance  the right distance                        "          | 
  //  weight         weight                                    "          | 
  //  qualHit        returns m_QualHit                     CalibratedDRHit|
  //                   bits to be defined by CDOFFCAL          "          |
  //  singleTDC      = true if single TDC                      "          |
  //                   false for multiple TDC                             | 
  //  print                                                               |
//------------------------------------------------------------------------+

UInt16 CalibratedDRHit::wire     ()   const { return m_Wire;}
UInt16 CalibratedDRHit::layer    ()   const { return m_Layer;}
UInt16 CalibratedDRHit::wireInLyr()   const { return m_WireInLyr;} 
//
CalibratedDRHit::PicoCoul CalibratedDRHit::charge()        const 
          { return m_Charge; }
//
CalibratedDRHit::PicoSec  CalibratedDRHit::time()          const 
          { return m_Time;} 

//////////////////////////////////////////////////////////////////////
//  distance(): Return the nominal distance for this hit
//
CalibratedDRHit::Meters   CalibratedDRHit::distance()      const 
          { return m_DistanceSeries.distance(); } 
//
double       CalibratedDRHit::weight()      const { return m_Weight ;} 
//
CalibratedDRHit::QualHit  CalibratedDRHit::qualHit()     const 
          { return m_QualHit ;} 
//
DABoolean           CalibratedDRHit::singleTDC()     const 
          { return m_nTDC==1 ; }
//
UInt16              CalibratedDRHit::nTDC()     const 
          { return m_nTDC ; }
//
CalibratedTimeHit::PicoSec             CalibratedDRHit::earlyGap()     const 
          { return  m_earlyGap ; }
//
CalibratedTimeHit::PicoSec             CalibratedDRHit::lateGap()     const 
          { return m_lateGap ; }

UInt16 CalibratedDRHit::whichTDC() const
{ return m_whichTDC; }



//////////////////////////////////////////////////////////////////////
//  extrapolatedDistance(PicoSec deltaT): Return the distance for 
//     a shifted time deltaT. If deltaT is zero, this routine returns
//     the same value as distance().
//
CalibratedDRHit::Meters CalibratedDRHit::extrapolatedDistance(
   PicoSec deltaT) const
{
   return m_DistanceSeries.extrapolated_value(deltaT);
}

// Comparison operator
DABoolean CalibratedDRHit::operator==(const CalibratedDRHit& rhs) const
{
   return (       identifier()     == rhs.identifier()     &&
		  m_Wire           == rhs.m_Wire           &&
		  m_Layer          == rhs.m_Layer          &&
		  m_WireInLyr      == rhs.m_WireInLyr      &&
		  m_Charge         == rhs.m_Charge         &&
		  m_Time           == rhs.m_Time           &&
		  m_DistanceSeries == rhs.m_DistanceSeries &&
		  m_Weight         == rhs.m_Weight         &&
		  m_QualHit        == rhs.m_QualHit        &&
		  m_nTDC           == rhs.m_nTDC           &&
		  m_earlyGap       == rhs.m_earlyGap       &&
		  m_lateGap        == rhs.m_lateGap        &&
  		  m_whichTDC       == rhs.m_whichTDC       &&
		  m_rawChargeTime  == rhs.m_rawChargeTime     ) ;
}

DABoolean CalibratedDRHit::operator!=(const CalibratedDRHit& rhs) const
{
  return !( (*this) == rhs );
}


//-------+
// print |
//-------+
void CalibratedDRHit::printCDRH(ostream& os) const
{ CalibratedTimeHit::printCTH(os);
  os << "CalibratedDRHit      " << endl;
  os << "Calibrated wire      " << setw(15) << wire() << endl; 
  os << "Calibrated layer     " << setw(15) << layer() << endl;
  os << "Calibrated wireInLyr " << setw(15) << wireInLyr() << endl;
  os << "Calibrated charge    " << setw(15) << charge() << endl;   
  os << "Calibrated time      " << setw(15) << time() << endl;  
  os << "Calibrated distance  " << setw(15) << distance() << endl;   
  os << "Calibrated weight    " << setw(15) << weight() << endl;
  os << "Calibrated nTDC      " << setw(15) << nTDC() << endl;
  os << "Calibrated qualHit   " << setw(15) << qualHit() << endl ;
  os << "Calibrated earlyGap  " << setw(15) << earlyGap() << endl ;
  os << "Calibrated lateGap   " << setw(15) << lateGap() << endl ;
}











