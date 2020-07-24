#if !defined(CALIBRATEDDATA_CALIBRATEDHIT_H)
#define CALIBRATEDDATA_CALIBRATEDHIT_H
// -*- C++ -*-
//
// Package:     <CalibratedData>
// Module:      CalibratedHit
// 
// Description: Abstract base class for all calibrated hits
//
// Usage:
//    Calibrated detector data means, eg, DR hits after TDC to time conversion.
//    This class declares the interface functions:
//      identifier() returns the id of this hit, a number between 1 and N.
//      deviceType() returns the device type. (pure virtual)
//    Member data:
//      m_id          A sequence number
//
//
// Author:      Jon Thaler
// Created:     Mon Dec 15 11:28:51 EST 1997
//              Thu Apr 2 1998 replace Calib* with Calibrated
//
// Revision history
// Revision  3/13/98 Inga Karliner
//   remove all RawHit information
//   move charge to CalibratedDRHit
//   remove m_DeviceType, take out device from Constructor
//   make deviceType() pure virtual
//   move deviceType() function to CalibratedDRHit
//   add Experiment/types.h
// Revision  6/16/98 Inga Karliner
//   enum DeviceType: add Cathode and NumberOfDeviceTypes 
//--------------------+
// user include files |
//--------------------+
#include "Experiment/types.h"
#include "Experiment/Experiment.h"
#include "Experiment/report.h"

class CalibratedHit 
{ 
  // friend classes and functions
public:
  //------------------------------------------------------+
  // constants, enums and typedefs                        |
  //  Identifier  A sequence number between 1 and N.      |
  //  DeviceType  An enumeration of the allowed devices.  |
  //------------------------------------------------------+ 
  typedef Count Identifier;
  enum    DeviceType {BeamSpot, SVR, SVZ, DR, RICH, CC, Muon, Cathode, ZD,
		      NumberofDeviceTypes}; 
  static string DeviceName[NumberofDeviceTypes];
  
  //-----------------------------+
  // Constructors and destructor |
  //-----------------------------+
  //  CalibratedHit(const Identifier id);
  CalibratedHit(Identifier id);
  virtual ~CalibratedHit();

// Operator == (needed for storage)
      DABoolean operator ==(const CalibratedHit& rhs) const;
      DABoolean operator !=(const CalibratedHit& rhs) const;

  //---------------------------------------------------+
  // member functions                                  |
  //  deviceType returns the device type (enumerated). |
  //  printCH                                          |
  //  identifier returns the id of this hit, a number  |
  //             between 1 and N.                      |
  //  setIdentifier (Identifier id  )                  |
  //---------------------------------------------------+
  virtual DeviceType deviceType()      const = 0;
  void               printCH(ostream&) const;

  // const member functions
  Identifier identifier() const;

  inline void setIdentifier(Identifier Id);

 protected:
  // protected member functions

 private:
  //-----------------------------------+
  // Constructor                       |
  //  Disable the default constructor. |
  //-----------------------------------+
  CalibratedHit();

  //-------------------------------------------+
  // member data                               |
  //  m_id          A sequence number.         |
  //-------------------------------------------+
  Identifier  m_id;
};

//-----------------------------+
// inline function definitions |
//-----------------------------+
inline ostream& operator<< (ostream& os, const CalibratedHit& hit)
{ hit.printCH(os);
  return os;
}

inline void
CalibratedHit::setIdentifier(CalibratedHit::Identifier id)
{ m_id=id; }

//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
// templated member function definitions
//# include "CalibratedData/Template/CalibratedHit.cc"
//#endif

#endif /* CALIBRATEDDATA_CALIBRATEDHIT_H */






