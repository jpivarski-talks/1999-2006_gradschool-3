// -*- C++ -*-
//
// Package:     <Doit>
// Module:      DoitSeedQuality
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Wed Jan  3 19:08:28 EST 2001
// $Id: DoitSeedQuality.cc,v 1.2 2002/08/16 14:44:40 cleo3 Exp $
//
// Revision history
//
// $Log: DoitSeedQuality.cc,v $
// Revision 1.2  2002/08/16 14:44:40  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.1  2001/01/05 05:37:13  lyon
// Added Doit status words
//

#include "Experiment/Experiment.h"

#include "C++Std/iostream.h"

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
#include "Doit/DoitSeedQuality.h"

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

static const char* const kFacilityString = "Doit.DoitSeedQuality" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitSeedQuality.cc,v 1.2 2002/08/16 14:44:40 cleo3 Exp $";
static const char* const kTagString = "$Name:  $";

// Other methods are inlined

//
//
// const member functions
//
void DoitSeedQuality::printout(ostream& s) const
{
  // Call print out for the base class
  TRSeedTrackQuality::printout(s);

  // Now print our stuff
  s << "DoitSeedQuality: \n";

  s << " ChiSquareZ = " << chiSquareZ() 
    << " ChiSquare3D = " << chiSquare3() 
    << " RESICD = " << resicd() << "\n";

  reportFlag(s, "good_z_information", goodZ());
  reportFlag(s, "doit_inward_curler", inwardDoitCurler());
  s << "\n";

  // Now print the DoitStatus information
  s << doitStatus();
}
