#if !defined(DATAHANDLER_PROXYFACTORYFUNCTION_CC)
#define DATAHANDLER_PROXYFACTORYFUNCTION_CC
// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      ProxyFactoryFunction
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Tue Nov 3 11:35:37 EST 1998
// $Id: ProxyFactoryFunction.cc,v 1.2 2001/04/17 01:15:28 cleo3 Exp $
//
// Revision history
//
// $Log: ProxyFactoryFunction.cc,v $
// Revision 1.2  2001/04/17 01:15:28  cleo3
// added typename to ProxyFactory methods
//
// Revision 1.1  1998/11/06 00:14:41  mkl
// added ProxyFactoryFunction
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/ProxyFactoryFunction.h"
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
template< class Producer, class ProxyType >
ProxyFactoryFunction< Producer, ProxyType >::ProxyFactoryFunction(
   Producer& iProducer,
   FactoryFunction iFunction )
   : m_producer( iProducer ),
     m_function( iFunction ),
     m_useConstFunction( false )
{
}

template< class Producer, class ProxyType >
ProxyFactoryFunction< Producer, ProxyType >::ProxyFactoryFunction(
   const Producer& iProducer,
   ConstFactoryFunction iFunction )
   : m_producer( (Producer&)iProducer ),  // have to fool compiler, else it's more
     m_constFunction( iFunction ),        // complicated than it has to be!
     m_useConstFunction( true )
{
}

// ProxyFactoryFunction::ProxyFactoryFunction( const ProxyFactoryFunction& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

template< class Producer, class ProxyType >
ProxyFactoryFunction< Producer, ProxyType >::~ProxyFactoryFunction()
{
}

//
// assignment operators
//
// const ProxyFactoryFunction& ProxyFactoryFunction::operator=( const ProxyFactoryFunction& rhs )
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
template< class Producer, class ProxyType >
ProxyBase*
ProxyFactoryFunction< Producer, ProxyType >::makeProxy() const
{
   if( true == m_useConstFunction ) {
      return (m_producer.*m_constFunction)();
   }
   else {
      return (m_producer.*m_function)();
   }
}

template< class Producer, class ProxyType >
DataKey 
ProxyFactoryFunction< Producer, ProxyType >::makeKey( 
   const UsageTag& iUsage,
   const ProductionTag& iProduction ) const
{
   return RecordMethods< typename ProxyType::value_type >::makeKey( iUsage, iProduction );
}

//
// static member functions
//

#endif /*DATAHANDLER_PROXYFACTORYFUNCTION_CC*/
