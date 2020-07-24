#if !defined(CALIBRATEDDATA_CALIBRATEDTIMEHIT_H)
#define      CALIBRATEDDATA_CALIBRATEDTIMEHIT_H
// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CalibratedTimeHit
// 
// Description: Abstract base class for calibrated "time" data (ie, DR & VD)
//
// Usage:
//   Declares the data access interface functions for times and distances:
//     time     ()  returns first time.
//     distance ()  returns the first distance (avg, left, right)
//     weight   ()  returns distance resolution
//     singleTDC()  = true if single TDC, false for multiple TDCs
//   Data is kept in the detector specific derived classes.   
//
//
// Author:      Jon Thaler
// Created:     Mon Dec 15 12:01:43 EST 1997
//              Thu Apr 2 1998 replace Calib* with Calibrated*
//
// Revision history
// Revision 1.1 1998/03/11 16:00:27 inga
//    include STLUtility not Utility;
// Revision  3/13/98 Inga Karliner
//   remove all RawHit information 
//   take out device from Constructor
//   add leftDistance(), rightDistance(), resolution
//   eliminate multiple TDC/CalibratedHit
//   add DABoolean singleTDC = true for single TDC hit
// Revision  3/27/98 Inga Karliner
//   replace resolution with weight
// Revision  4/15/98 Inga Karliner
//   replace typedef float-> double PicoSec,Meters 
//--------------------+
// user include files |
//--------------------+
//#include "Experiment/Experiment.h"
#include "CalibratedData/CalibratedHit.h"

//---------------------------------------------------------------+
// forward declarations                                          |
//  fwd_vector.h obviates the inclusion of the STL vector class. |
//---------------------------------------------------------------+
#include "STLUtility/fwd_vector.h"

class CalibratedTimeHit : public CalibratedHit
{public:

  //------------------------------------+
  // Constants, enums and typedefs:     |
  //  PicoSec    the time data type     |
  //  Meters     the distance data type |
  //------------------------------------+
  typedef double  PicoSec;
  typedef double  Meters;

  //-----------------------------+
  // Constructors and destructor |
  //-----------------------------+
  //  CalibratedTimeHit(const Identifier id);
  CalibratedTimeHit(Identifier id);
  //CalibratedTimeHit(const CalibratedTimeHit&);
  virtual ~CalibratedTimeHit();

  //--------------------------------------------+
  // member functions                           |
  //  time           returns the time           |
  //  distance       returns the <distance>     |
  //  weight         returns weight             |
  //  singleTDC      returns true if single TDC |
  //                 false for multiple TDC     | 
  //  printCTH                                  | 
  //--------------------------------------------+
  virtual       PicoSec          time          () const = 0;
  virtual       Meters           distance      () const = 0;
  virtual       double           weight        () const = 0;
  virtual       DABoolean        singleTDC     () const = 0; 

  void printCTH(ostream&) const;

protected:
  // protected member functions

 private:
  //-----------------------------+
  // Constructors and destructor |
  //  Disable some constructors. |
  //-----------------------------+
  CalibratedTimeHit();
};

// inline function definitions
inline ostream& operator<< (ostream& os, const CalibratedTimeHit& hit)
{ hit.printCTH(os);
  return os;
}

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "CalibratedData/Template/CalibratedTimeHit.cc"
//#endif

#endif /* CALIBRATEDDATA_CALIBRATEDTIMEHIT_H */







