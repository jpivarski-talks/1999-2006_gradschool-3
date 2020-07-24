// -*- C++ -*-
//
// Package:     DataHandler
// Module:      ProxyBase
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Thu Sep 24 14:02:01 EDT 1998
// $Id: ProxyBase.cc,v 1.1 1998/09/25 19:31:26 cdj Exp $
//
// Revision history
//
// $Log: ProxyBase.cc,v $
// Revision 1.1  1998/09/25 19:31:26  cdj
// no longer use ProxyDict
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/ProxyBase.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DataHandler.ProxyBase" ;

//
// static data member definitions
//

//
// constructors and destructor
//
ProxyBase::ProxyBase() : m_cacheIsValid( false )
{
}

// ProxyBase::ProxyBase( const ProxyBase& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ProxyBase::~ProxyBase()
{
}

//
// assignment operators
//
// const ProxyBase& ProxyBase::operator=( const ProxyBase& rhs )
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

//
// static member functions
//

