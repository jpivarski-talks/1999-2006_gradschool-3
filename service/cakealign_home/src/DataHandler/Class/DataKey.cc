// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      DataKey
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Wed Sep 23 17:06:29 EDT 1998
// $Id: DataKey.cc,v 1.1 1998/09/25 19:31:23 cdj Exp $
//
// Revision history
//
// $Log: DataKey.cc,v $
// Revision 1.1  1998/09/25 19:31:23  cdj
// no longer use ProxyDict
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/DataKey.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DataHandler.DataKey" ;

//
// static data member definitions
//

//
// constructors and destructor
//
DataKey::DataKey()
{
}

// DataKey::DataKey( const DataKey& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//DataKey::~DataKey()
//{
//}

//
// assignment operators
//
// const DataKey& DataKey::operator=( const DataKey& rhs )
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
DABoolean
DataKey::operator==( const DataKey& iRHS ) const 
{
   return ( ( m_type == iRHS.m_type ) &&
	    ( m_usage == iRHS.m_usage ) &&
	    ( m_production == iRHS.m_production ) );
}

DABoolean
DataKey::operator<( const DataKey& iRHS ) const 
{
   if( m_type < iRHS.m_type ) {
      return true;
   } else if ( m_type == iRHS.m_type ) {
      if( m_usage < iRHS.m_usage ) {
	 return true;
      } else if ( m_usage == iRHS.m_usage ) {
	 if( m_production < iRHS.m_production ) {
	    return true;
	 }
      }
   }
   return false;
}

//
// static member functions
//
