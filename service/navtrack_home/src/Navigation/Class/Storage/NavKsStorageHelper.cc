// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavKsStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: NavKsStorageHelper.cc,v 1.1 2003/09/02 01:53:07 cdj Exp $
//
// Revision history
//
// $Log: NavKsStorageHelper.cc,v $
// Revision 1.1  2003/09/02 01:53:07  cdj
// can now store NavKs and NavShowers using templated storage helper
//
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
#include "Navigation/Storage/NavKsStorageHelper.h"

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
   const NavKs& getDefault(NavKs*) {

   static const VXFitVeeKShort fit(
      VXFitVeeKShort::Identifier ( 1 ), // identifier 
      HepVector3D ( 2., 2., 2. ), // momentum 
      HepPoint3D ( 3., 3., 3. ), // position 
      VXFitVeeKShort::ValueType ( 4 ), // mass 
      VXFitVeeKShort::ValueType ( 5 ), // charge 
      HepSymMatrix ( 7, 0 ), // errorMatrix 
      double ( 7 ), // fitChiSquare 
      double ( 8 ), // fitChiContrib 
      int ( 9 ), // fatherId 
      int ( 10 ), // fatherCharge 
      int ( 11 ), // motherId 
      int ( 12 ) ); // motherCharge 

      static const NavKs temp(
	 &fit, // aId 
	 reinterpret_cast<NavTrack*>(1),
	 reinterpret_cast<NavTrack*>(1)); // aMaster 
      return temp;
   }
}

#include "Navigation/Storage/Template/NavTypeStorageHelper.cc"
template class NavTypeStorageHelper<NavKs>;

//
// static member functions
//
template<>
NavKs*
SMStorageHelper<NavKs>::make( const NavKs& iTempData ) const
{
   //change this line if NavKs does not have a copy constructor
   return 0;
}

//------------------------------------------------------
// DO NOT DELETE 
SM_REGISTER_STORAGE( NavKsStorageHelper );

#include "StorageManagement/SMDeliverer.h"
SM_REGISTER_VERSION_DEF( NavKsStorageHelper, NavKs )

//this is done to create a TypeTag and its name
#include "FrameAccess/instantiate_macros.h"
INSTANTIATE_ONE(NavKs)
#include "StorageManagement/instantiate_one_typetag.h"

#include "StorageManagement/Template/SMStorageHelper.cc"
template class SMStorageHelper<NavKs>;

//------------------------------------------------------
