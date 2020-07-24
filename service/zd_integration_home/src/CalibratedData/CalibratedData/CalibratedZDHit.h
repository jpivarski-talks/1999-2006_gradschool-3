#if !defined(CALIBRATEDDATA_CALIBRATEDZDHIT_H) 
#define CALIBRATEDDATA_CALIBRATEDZDHIT_H 
// -*- C++ -*- 
// 
// Package: <CalibratedData> 
// Module:  CalibratedZDHit 
// 
/**\class CalibratedZDHit CalibratedZDHit.h CalibratedData/CalibratedZDHit.h

 Description: Calibrated ZD Anode Wire data.
 
 Usage: The time and distance data access interface is defined in the base 
        classes, CalibratedHit and CalibratedTimeHit.
  
*/

// 
// Author: Brian Heltsley
//
//
//            
// Revision history
// //--------------------+
// user include files |
//--------------------+
#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include "CalibratedData/Tseries.h"
#include "CalibratedData/CalibratedDRHit.h"
#include "RawData/ZDDatum.h"

//---------------------------------------------------------------+
// forward declarations                                          |
//  fwd_vector.h obviates the inclusion of the STL vector class. |
//---------------------------------------------------------------+
#include "STLUtility/fwd_vector.h"

class CalibratedZDHit : public CalibratedDRHit
{
public:

  //-----------------------------+
  // Constructors and destructor |
  //-----------------------------+
      CalibratedZDHit(UInt16 wire,
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
		      Identifier id) :
	 CalibratedDRHit( wire,
			  layer,
			  wireInLyr,
			  charge,
			  time,
			  distanceSeries,
			  weight,
			  qualHit,
			  nTDC,
			  earlyGap,
			  lateGap,
			  whichTDC,
			  id ) {};
      CalibratedZDHit(UInt16 wire,
		      UInt16 layer, 
		      UInt16 wireInLyr,
		      PicoCoul charge,
		      PicoSec time,
		      Tseries distanceSeries,
		      double  weight,
		      QualHit qualHit,
		      DABoolean singleTDC, 
		      Identifier id) :
	 CalibratedDRHit( wire,
			  layer,
			  wireInLyr,
			  charge,
			  time,
			  distanceSeries,
			  weight,
			  qualHit,
			  singleTDC,
			  id                ) {};
      //
//  CalibratedZDHit(const CalibratedZDHit&);

  // Comparison operator
      DABoolean operator==(const CalibratedZDHit& rhs) const {
	 return ( (CalibratedDRHit&)(*this) == (CalibratedDRHit&)rhs ) ; } ;
      DABoolean operator!=(const CalibratedZDHit& rhs) const {
	 return !operator==(rhs) ; } ;
  
      virtual ~CalibratedZDHit() {};

      //! Return the device type for this hit (should be ZD)
      inline DeviceType deviceType() const; 

protected:
  // protected member functions

  //------------------------+
  // assignment operator(s) |
  //------------------------+
//  CalibratedZDHit& operator = (const CalibratedZDHit&);

 private:
  //-----------------------------+
  // Constructors and destructor |
  //-----------------------------+
  CalibratedZDHit();


};

// inline function definitions
inline CalibratedZDHit::DeviceType CalibratedZDHit::deviceType() const
{ return ZD; }

#endif /* CALIBRATEDDATA_CALIBRATEDZDHIT_H */















