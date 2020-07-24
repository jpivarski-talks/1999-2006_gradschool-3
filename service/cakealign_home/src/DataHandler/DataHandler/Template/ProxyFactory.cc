#if !defined(DATAHANDLER_PROXYFACTORY)
#define DATAHANDLER_PROXYFACTORY
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      ProxyFactory
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Sun Nov  1 11:35:37 EST 1998
// $Id: ProxyFactory.cc,v 1.2 2001/04/17 01:15:28 cleo3 Exp $
//
// Revision history
//
// $Log: ProxyFactory.cc,v $
// Revision 1.2  2001/04/17 01:15:28  cleo3
// added typename to ProxyFactory methods
//
// Revision 1.1  1998/11/02 16:19:39  mkl
// moved ProxyFactory* stuff to DataHandler
//
// Revision 1.1  1998/11/01 20:14:52  mkl
// switched to new proxy factory registration scheme
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/ProxyFactory.h"
#include "DataHandler/RecordMethods.h"

// STL classes

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
template< class ProxyType >
ProxyFactory< ProxyType >::ProxyFactory()
{
}

// ProxyFactory::ProxyFactory( const ProxyFactory& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

template< class ProxyType >
ProxyFactory< ProxyType >::~ProxyFactory()
{
}

//
// assignment operators
//
// const ProxyFactory& ProxyFactory::operator=( const ProxyFactory& rhs )
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
template< class ProxyType >
ProxyBase*
ProxyFactory< ProxyType >::makeProxy() const
{
   return new ProxyType;
}

template< class ProxyType >
DataKey 
ProxyFactory< ProxyType >::makeKey( const UsageTag& iUsage,
				    const ProductionTag& iProduction ) const
{
   return RecordMethods< typename ProxyType::value_type >::makeKey( iUsage, iProduction );
}

//
// static member functions
//

#endif /*DATAHANDLER_PROXYFACTORY*/
