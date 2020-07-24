// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTrackStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: NavTrackStorageHelper.cc,v 1.2 2003/09/02 01:50:18 cdj Exp $
//
// Revision history
//
// $Log: NavTrackStorageHelper.cc,v $
// Revision 1.2  2003/09/02 01:50:18  cdj
// now used templated version
//
// Revision 1.1  2003/08/31 23:25:32  cdj
// added the ability to store NavTracks
//
// Revision 1.3  2001/01/31 22:16:06  cdj
// added reminder to users in storage routine to register new versions
//
// Revision 1.2  2000/01/21 21:07:37  cdj
// helper skeletons now include SMPackInfo.h
//
// Revision 1.1  1999/10/18 18:13:04  cdj
// first submission
//
// Revision 1.2  1999/09/30 22:38:33  cdj
// no longer need default constructor
//
// Revision 1.1.1.1  1999/09/13 21:21:45  cdj
// imported package
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
#include "Navigation/Storage/NavTrackStorageHelper.h"
//#include "StorageManagement/SMSinkStream.h"
//#include "StorageManagement/SMSourceStream.h"
//#include "StorageManagement/SMPackInfo.h" //defines sm_pack*
//#include "FrameAccess/extract.h"

//#include "DataHandler/Record.h"

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
namespace Navigation
{
   const NavTrack& getDefault(NavTrack*) {
      static const NavTrack temp(
	 NavTrack::Identifier ( 1 ), // aId 
	 *static_cast<NavTrackMaster*>(0)); // aMaster 
      return temp;
   }
}

#include "Navigation/Storage/Template/NavTypeStorageHelper.cc"
template class NavTypeStorageHelper<NavTrack>;

//
// static member functions
//
template<>
NavTrack*
SMStorageHelper<NavTrack>::make( const NavTrack& iTempData ) const
{
   //change this line if NavTrack does not have a copy constructor
   return 0;
}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( NavTrackStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( NavTrackStorageHelper, NavTrack )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(NavTrack)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<NavTrack>;

//------------------------------------------------------
