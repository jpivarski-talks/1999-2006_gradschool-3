// -*- C++ -*-
//
// Package:     CalibratedData
// Module:      CalibratedCathodeHit
// 
// Description: Calibrated Cathode data (see CalibratedCathodeHit.h)
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Dawn Isabel
// Created:     Wed Jul  1 14:20:23 EDT 1998
// $Id: CalibratedCathodeHit.cc,v 1.5 2000/03/03 15:16:18 lyon Exp $
//
// Revision history
//
// $Log: CalibratedCathodeHit.cc,v $
// Revision 1.5  2000/03/03 15:16:18  lyon
// Changes for storage helpers
//
// Revision 1.4  1999/03/19 17:41:17  jjt
// Implement operator<< for all devices.
//
// Revision 1.3  1998/12/15 18:54:00  jjt
// Modify print() to eliminate compiler warnings
//
// Revision 1.2  1998/08/18 19:20:04  wsun
// added phi position to CalibratedCathodeHit
//
// Revision 1.1  1998/08/07 17:54:46  isabel
// class for calibrated cathode hits
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include "Experiment/report.h"

// system include files
#include <iomanip.h>
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

#include <stdlib.h> // for exit

// user include files
#include "CalibratedData/CalibratedCathodeHit.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"CalibratedData.CalibratedCathodeHit" ;

//
// static data member definitions
//

//
// constructors and destructor
//
CalibratedCathodeHit::
CalibratedCathodeHit(UInt16 peakPad,
		     UInt16 numberOfPads,
		     CalibratedCathodeHit::PicoCoul totalCharge,
		     CalibratedCathodeHit::PicoCoul peakCharge,
		     CalibratedHit::Identifier id,
		     Meters z, 
		     double phi,
		     double weight,
		     double width,
		     CalibratedCathodeHit::QualHit qualHit) :
   CalibratedHit(id),
   m_peakPad(peakPad),
   m_numberOfPads(numberOfPads),
   m_totalCharge(totalCharge),
   m_peakCharge(peakCharge),
   m_z(z),
   m_phi(phi),
   m_weight(weight),
   m_width(width),
   m_qualHit(qualHit)
{
}

CalibratedCathodeHit::
CalibratedCathodeHit( const CalibratedCathodeHit& calibratedCathodeHit) :
   CalibratedHit(calibratedCathodeHit),
   m_peakPad            (calibratedCathodeHit.peakPad()),
   m_numberOfPads       (calibratedCathodeHit.numberOfPads()),
   m_totalCharge        (calibratedCathodeHit.totalCharge()),
   m_peakCharge         (calibratedCathodeHit.peakCharge()),
   m_z                  (calibratedCathodeHit.z()),
   m_phi                (calibratedCathodeHit.phi()),
   m_weight             (calibratedCathodeHit.weight()),
   m_width              (calibratedCathodeHit.width()),
   m_qualHit            (calibratedCathodeHit.qualHit())
{
   //    // do actual copying here; if you implemented
   //    // operator= correctly, you may be able to use just say      
   //    *this = rhs;
}

CalibratedCathodeHit::~CalibratedCathodeHit()
{
}

// Comparisons
DABoolean CalibratedCathodeHit::operator==(const CalibratedCathodeHit& rhs) const
{
  return ( identifier() == rhs.identifier() );
}

DABoolean CalibratedCathodeHit::operator!=(const CalibratedCathodeHit& rhs) const
{
  return !( (*this) == rhs );
}


//
// assignment operators
//
// const CalibratedCathodeHit& CalibratedCathodeHit::operator=( const CalibratedCathodeHit& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//


//print
void CalibratedCathodeHit::printCCH(ostream& os) const
{ CalibratedHit::printCH(os);
  os << "CalibratedCathodeHit       " << endl;
  os << "Calibrated peak pad        " << setw(15) << peakPad() << endl;
  os << "Calibrated number of pads  " << setw(15) << numberOfPads() << endl;
  os << "Calibrated total charge    " << setw(15) << totalCharge() << endl;
  os << "Calibrated peak charge     " << setw(15) << peakCharge() << endl;
  os << "Calibrated Z position      " << setw(15) << z() << endl;
  os << "Calibrated phi position    " << setw(15) << phi() << endl;
  os << "Calibrated weight          " << setw(15) << weight() << endl;
  os << "Calibrated width           " << setw(15) << width() << endl;
  os << "Calibrated qualHit         " << setw(15) << qualHit() << endl;
  os << "\n";
}
