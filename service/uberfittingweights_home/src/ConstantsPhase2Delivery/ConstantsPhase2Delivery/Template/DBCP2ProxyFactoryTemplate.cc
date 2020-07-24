#if !defined(CONSTANTSPHASE2DELIVERY_DBCP2PROXYFACTORYTEMPLATE_CC)
#define CONSTANTSPHASE2DELIVERY_DBCP2PROXYFACTORYTEMPLATE_CC
// -*- C++ -*-
//
// Package:     <ConstantsPhase2Delivery>
// Module:      DBCP2ProxyFactoryTemplate
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Valentine Kouznetsov
// Created:     Tue Jan 28 12:47:41 EST 2003
// $Id: DBCP2ProxyFactoryTemplate.cc,v 1.1.1.1 2003/01/28 21:49:58 vk Exp $
//
// Revision history
//
// $Log: DBCP2ProxyFactoryTemplate.cc,v $
// Revision 1.1.1.1  2003/01/28 21:49:58  vk
// imported ConstantsPhase2Delivery sources
//
//
// First working version

#include "Experiment/Experiment.h"
#include "Experiment/report.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "ConstantsPhase2Delivery/DBCP2ProxyFactoryTemplate.h"
#include "ConstantsPhase2Delivery/DBCP2Proxy.h"
#include "DataHandler/RecordMethods.h"

// STL classes
//
// constants, enums and typedefs
//

//static const char* const kFacilityString = "ConstantsPhase2Delivery.DBCP2ProxyFactoryTemplate" ;

//
// static data member definitions
//

//
// constructors and destructor
//
template< class T >
DBCP2ProxyFactoryTemplate<T>::DBCP2ProxyFactoryTemplate()
{
}

// DBCP2ProxyFactoryTemplate::DBCP2ProxyFactoryTemplate( const DBCP2ProxyFactoryTemplate& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

template< class T >
DBCP2ProxyFactoryTemplate<T>::~DBCP2ProxyFactoryTemplate()
{
}

//
// assignment operators
//
// const DBCP2ProxyFactoryTemplate& DBCP2ProxyFactoryTemplate::operator=( const DBCP2ProxyFactoryTemplate& rhs )
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
template< class T >
void
DBCP2ProxyFactoryTemplate<T>::createProxy( 
   const string& iBootFilePath,
   const DBConstantsSelectionTag& iTags,
   KeyedProxy& oKeyedProxy,
   DBCP2Info*& oRecordInfo ) const
{
   DBCP2Proxy<T>* pProxy = new DBCP2Proxy<T>( iBootFilePath,
							  iTags );
   
   if( 0 == pProxy ) {
      report( ERROR, "DBCP2ProxyFactoryTemplate::createProxy" ) 
	 << "unable to allocate memory"<< endl;
      assert(false);
      ::exit( -1 );
   }

   oKeyedProxy = KeyedProxy( 
      RecordMethods< typename DBCP2Proxy<T>::value_type >::makeKey(
	 UsageTag(), 
	 ProductionTag()),
      pProxy );
   oRecordInfo = pProxy;

}

//
// static member functions
//
#endif
