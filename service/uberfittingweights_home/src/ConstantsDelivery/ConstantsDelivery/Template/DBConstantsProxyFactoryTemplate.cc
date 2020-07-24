#if !defined(DBCONSTANTSPROXYFACTORYTEMPLATE_CC)
#define DBCONSTANTSPROXYFACTORYTEMPLATE_CC
// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      DBConstantsProxyFactoryTemplate
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Tue Aug 18 22:16:27 EDT 1998
// $Id: DBConstantsProxyFactoryTemplate.cc,v 1.7 1999/11/03 03:33:28 cdj Exp $
//
// Revision history
//
// $Log: DBConstantsProxyFactoryTemplate.cc,v $
// Revision 1.7  1999/11/03 03:33:28  cdj
// now handles multiple constants per stream
//
// Revision 1.6  1999/07/14 21:22:37  cdj
// now uses bootfile path to find servers
//
// Revision 1.5  1999/04/29 18:45:44  cdj
// updated to use new constants implementation
//
// Revision 1.4  1998/11/10 22:12:30  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.3  1998/11/05 18:52:19  baker
// Another batch of updates to get this library finally compiled
// *AND* linkable
//
// Revision 1.2  1998/09/30 20:40:28  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:33:48  baker
// // Rich Baker, September 30, 1998
// // Renamed files from old ConstantsDelivery
// // for new ConstantsDelivery library
//
// Revision 1.1  1998/08/19 03:17:53  cdj
// first submission
//

#include "Experiment/Experiment.h"
#include "Experiment/report.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ConstantsDelivery/DBConstantsProxyFactoryTemplate.h"
#include "ConstantsDelivery/DBConstantsProxy.h"
#include "DataHandler/RecordMethods.h"

// STL classes
//
// constants, enums and typedefs
//

// 11/5/98 Commented out kFacilityString in this templated class
// implementation because of "multiple declaration" errors.
//static const char* const kFacilityString = "ConstantsDelivery.DBConstantsProxyFactoryTemplate" ;

//
// static data member definitions
//

//
// constructors and destructor
//
template< class T >
DBConstantsProxyFactoryTemplate<T>::DBConstantsProxyFactoryTemplate()
{
}

// DBConstantsProxyFactoryTemplate::DBConstantsProxyFactoryTemplate( const DBConstantsProxyFactoryTemplate& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

template< class T >
DBConstantsProxyFactoryTemplate<T>::~DBConstantsProxyFactoryTemplate()
{
}

//
// assignment operators
//
// const DBConstantsProxyFactoryTemplate& DBConstantsProxyFactoryTemplate::operator=( const DBConstantsProxyFactoryTemplate& rhs )
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
// 11/9/98 Rich Baker
// Removed the KeyedProxy from this function (obsolete)
// and changed Info* to Info*&
template< class T >
void
DBConstantsProxyFactoryTemplate<T>::createProxy( 
   const string& iBootFilePath,
   const DBConstantsSelectionTag& iTags,
   KeyedProxy& oKeyedProxy,
   DBConstantsInfo*& oRecordInfo ) const
{
   DBConstantsProxy<T>* pProxy = new DBConstantsProxy<T>( iBootFilePath,
							  iTags );
   
   if( 0 == pProxy ) {
      report( ERROR, "DBConstantsProxyFactoryTemplate::createProxy" ) 
	 << "unable to allocate memory"<< endl;
      assert(false);
      ::exit( -1 );
   }

   oKeyedProxy = KeyedProxy( 
      RecordMethods< typename DBConstantsProxy<T>::value_type >::makeKey(
	 UsageTag(), 
	 ProductionTag()),
      pProxy );
   oRecordInfo = pProxy;

}

//
// static member functions
//
#endif
