#if !defined(DATAHANDLER_PROXYTEMPLATE_CC)
#define DATAHANDLER_PROXYTEMPLATE_CC
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      ProxyTemplate
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Thu Sep 24 14:05:07 EDT 1998
// $Id: ProxyTemplate.cc,v 1.3 2002/08/16 14:32:41 cleo3 Exp $
//
// Revision history
//
// $Log: ProxyTemplate.cc,v $
// Revision 1.3  2002/08/16 14:32:41  cleo3
// replaced iostream.h with C++/iostream.h
//
// Revision 1.2  1999/12/10 14:12:16  cdj
// added throwing exceptions when Proxy's faultHandler returns 0
//
// Revision 1.1  1998/09/25 19:32:02  cdj
// no longer use ProxyDict
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/ProxyTemplate.h"
#include "DataHandler/DHProxyErrorException.h"
#include "DataHandler/Record.h"

// STL classes

//
// constants, enums and typedefs
//

//static const char* const kFacilityString = "DataHandler.ProxyTemplate" ;

//
// static data member definitions
//

//
// constructors and destructor
//
template< class T >
ProxyTemplate<T>::ProxyTemplate() :
   m_cache(0)
{
}

// ProxyTemplate::ProxyTemplate( const ProxyTemplate& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

template< class T>
ProxyTemplate<T>::~ProxyTemplate()
{
}

//
// assignment operators
//
// const ProxyTemplate& ProxyTemplate::operator=( const ProxyTemplate& rhs )
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
template < class T >
const T*
ProxyTemplate<T>::get( const Record& iRecord, const DataKey& iKey ) const
{
   if( ! cacheIsValid() ) {

//
// until mutable is implemented have to cast way const to fill cache
//
      const_cast<ProxyTemplate<T>*>(this)->m_cache = 
	 const_cast<ProxyTemplate<T>*>(this)->faultHandler(iRecord, iKey ) ;
//
// as get is now a const function, once the cache is filled it should not
//   be filled again (with possibly different contents) until `invalidate'
//   has been called.
//
      const_cast<ProxyTemplate<T>*>(this)->setCacheIsValid() ;
   }
   if( 0 == m_cache ) {
      throwFaultHandlerException( iRecord, iKey );
   }
   return ( m_cache ) ;
}

template < class T >
void
ProxyTemplate<T>::throwFaultHandlerException( const Record& iRecord, 
					      const DataKey& iKey ) const
{
   throw DHProxyErrorException<T>(iRecord.stream(), iKey);
}

//
// static member functions
//
#endif /* DATAHANDLER_PROXYTEMPLATE_CC */
