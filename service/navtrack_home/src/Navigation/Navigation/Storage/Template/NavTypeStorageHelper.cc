// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavTypeStorageHelper
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Fri May 14 17:00:02 EDT 1999
// $Id: NavTypeStorageHelper.cc,v 1.1 2003/09/02 01:53:17 cdj Exp $
//
// Revision history
//
// $Log: NavTypeStorageHelper.cc,v $
// Revision 1.1  2003/09/02 01:53:17  cdj
// can now store NavKs and NavShowers using templated storage helper
//
// Revision 1.1  2003/08/31 23:25:32  cdj
// added the ability to store NavTypes
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
#include "Navigation/Storage/NavTypeStorageHelper.h"
#include "StorageManagement/SMSinkStream.h"
#include "StorageManagement/SMSourceStream.h"
#include "StorageManagement/SMPackInfo.h" //defines sm_pack*
#include "FrameAccess/extract.h"

#include "DataHandler/Record.h"
#include "DAException/DAException.h"

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

//
// static data member definitions
//

//
// constructors and destructor
//
template<class T>
NavTypeStorageHelper<T>::NavTypeStorageHelper() :
  m_recordCache(Record::kCacheIdentifierNotSet)
{
   //registration of the different versions
   // NOTE: version number is determined by the order in which the
   // methods are registered
   registerVersion( &NavTypeStorageHelper<T>::deliverV1 );
}

// NavTypeStorageHelper<T>::NavTypeStorageHelper( const NavTypeStorageHelper& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//NavTypeStorageHelper<T>::~NavTypeStorageHelper()
//{
//}

//
// assignment operators
//
// const NavTypeStorageHelper& NavTypeStorageHelper<T>::operator=( const NavTypeStorageHelper& rhs )
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
template<class T>
void 
NavTypeStorageHelper<T>::implementStore( 
   SMSinkStream& iSink,
   const T& iData ) 
{
// NOTE: Any change (adding/removing variable OR packing info) to this routine
//    MUST be accompanied by registering a new version deliver routine in
//    this class's constructor.  
//    If only the packing info is changes, you can register the same deliver
//    routine a second time.

   iSink 
      << SM_VAR( iData, identifier )   
      ;
}


template<class T>
T* 
NavTypeStorageHelper<T>::deliverV1( SMSourceStream& iSource )
{
  if(m_recordCache != iSource.record().cacheIdentifier() ) {
    extract( iSource.record(), m_masterList);
    m_recordCache = iSource.record().cacheIdentifier();
  }
   typename T::Identifier identifier; 

   iSource 
      >> identifier 
	 ;
   
   typename FATable<T>::const_iterator itFind =
      m_masterList.find(identifier);
   if( itFind == m_masterList.end() ) {
      throw DAException("Requested Navigation object not in master list");
   }
   return const_cast<T*>( &(*itFind) );
}

//
// const member functions
//
template<class T>
const T& 
NavTypeStorageHelper<T>::getDefault() const
{
   //give constructor unique values to allow test of store and deliver 
   // functions
   return Navigation::getDefault(static_cast<T*>(0));
}

template<class T>
DABoolean 
NavTypeStorageHelper<T>::compare( 
   const T* iNewData, 
   const T& iOldData ) const
{
   //write a comparison operation if operator== doesn't exist for T
   return (iNewData->identifier() == iOldData.identifier() ) ;
}

//
// static member functions
//
