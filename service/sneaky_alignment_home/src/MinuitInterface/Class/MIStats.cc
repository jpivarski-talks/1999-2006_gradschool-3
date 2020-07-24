// -*- C++ -*-
//
// Package:     <MinuitInterface>
// Module:      MIStats
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Tue Dec  7 14:20:28 EST 1999
// $Id: MIStats.cc,v 1.1.1.1 1999/12/09 15:34:51 lyon Exp $
//
// Revision history
//
// $Log: MIStats.cc,v $
// Revision 1.1.1.1  1999/12/09 15:34:51  lyon
// Imported MinuitInterface sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "MinuitInterface/MIStats.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "MinuitInterface.MIStats" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: MIStats.cc,v 1.1.1.1 1999/12/09 15:34:51 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
MIStats::MIStats( double fmin, double fedm, double errdef, 
		  int npari, int nparx, int istat ) :
  m_fmin(fmin),
  m_fedm(fedm),
  m_errdef(errdef),
  m_npari(npari),
  m_nparx(nparx),
  m_istat(istat)
{}

// MIStats::MIStats( const MIStats& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MIStats::~MIStats()
{}

//
// assignment operators
//
// const MIStats& MIStats::operator=( const MIStats& rhs )
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

//
// const member functions
//
void MIStats::dump(ostream& out) const
{
  out << "Minuit Status and Statistics: \n"
      << "   Function Minimum = " << functionMinimum() << "\n"
      << "   Vertical Distance to Minimum = " << verticalDistanceToMinimum() 
      << "\n"
      << "   Number of variable parameters = " << numberVariableParameters()
      << "\n"
      << "   Number of parameters defined = " << numberDefinedParameters()
      << "\n"
      << "   Error Matrix Status: ";

  switch (errorMatrixStatus())
  {
    case kNotCalculated:
      out << "Not Calculated";
      break;
    case kDiagonalApproximation:
      out << "Diagonal Approximation";
      break;
    case kForcedPosDef:
      out << "Forced to be Positive Definite";
      break;
    case kAccurate:
      out << "Accurate";
      break;
    default:
      out << " --- UNKNOWN STATUS ---";
  }
}

//
// static member functions
//

//
// friend member functions
//
ostream& operator<<( ostream& s, const MIStats& stats ) {

   stats.dump( s );
   return s;
}
