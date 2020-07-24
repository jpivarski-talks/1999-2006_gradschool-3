// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavShowerStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: NavShowerStorageHelper.cc,v 1.1 2003/09/02 01:53:07 cdj Exp $
//
// Revision history
//
// $Log: NavShowerStorageHelper.cc,v $
// Revision 1.1  2003/09/02 01:53:07  cdj
// can now store NavKs and NavShowers using templated storage helper
//
// Revision 1.1  2003/08/31 23:25:32  cdj
// added the ability to store NavShowers
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
#include "Navigation/Storage/NavShowerStorageHelper.h"
#include "C3cc/CcRunGain.h"
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
   const NavShower& getDefault(NavShower*) {
      static const CcGeV ar[6]={0,0,0,0,0,0};
      static const CcBasicShowerAttsArg basic(
	 CcBasicShowerAttsArg::Identifier ( 1 ), // identifier 
	 CcGeV ( 2 ), // uncEnergy 
	 CcAngle ( 3 ), // uncTheta 
	 CcAngle ( 4 ), // uncPhi 
	 CcMeters ( 5 ), // uncDistance 
	 CcReal ( .9 ), // e9oe25 
	 CcReal ( .9 ), // e9oe25Unf 
	 CcGeV ( 1 ), // mass 
	 CcMeters ( .1 ), // widthAll 
	 CcMeters ( .1 ), // widthN2 
	 CcMeters ( .1 ), // zWidth 
	 UInt16 ( 12 ), // nHits 
	 CcReal ( 11 ), // nHitsUsed 
	 CcGeV ( 1 ), // naiveEnergy 
	 CcGeV ( 1 ), // s3x3Energy 
	 CcGeV ( 1 ), // s5x5Energy 
	 Double ( 1 ), // nEffective 
	 CcCellIdType ( 18 ), // bigCellId 
	 UInt16 ( 1 ), // ringId 
	 ar,
	 CcReal ( .8 ), // e1oe9 
	 CcReal ( .8 ), // e1oe9Unf 
	 CcMeters ( .1 ) // width 
	 ); // CcGeV 

      static const CcShowerAttributes atts(basic,
					   HepPoint3D(),
					   HepSymMatrix(1),
					   true,
					   CcRunGain());
      static const NavShower temp(
	 atts, // aId 
	 *static_cast<NavShowerServer*>(0)); // aMaster 
      return temp;
   }
}

#include "Navigation/Storage/Template/NavTypeStorageHelper.cc"
template class NavTypeStorageHelper<NavShower>;

//
// static member functions
//
template<>
NavShower*
SMStorageHelper<NavShower>::make( const NavShower& iTempData ) const
{
   //change this line if NavShower does not have a copy constructor
   return 0;
}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( NavShowerStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( NavShowerStorageHelper, NavShower )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(NavShower)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<NavShower>;

//------------------------------------------------------
