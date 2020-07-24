// -*- C++ -*-
//
// Package:     <DataHandler>
// Module:      DataKeyTags
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Wed Sep 23 17:06:37 EDT 1998
// $Id: DataKeyTags.cc,v 1.1 1998/09/25 19:31:25 cdj Exp $
//
// Revision history
//
// $Log: DataKeyTags.cc,v $
// Revision 1.1  1998/09/25 19:31:25  cdj
// no longer use ProxyDict
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <string.h>

// user include files
//#include "Experiment/report.h"
#include "DataHandler/DataKeyTags.h"

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

//
// member functions
//

//
// const member functions
//
DABoolean
SimpleStringTag::operator==( const SimpleStringTag& iRHS ) const
{
   return ( 0 == strcmp( m_string, iRHS.m_string ) );
}

DABoolean
SimpleStringTag::operator<( const SimpleStringTag& iRHS ) const
{
   return ( 0 > strcmp( m_string, iRHS.m_string ) );
}

//
// static member functions
//
