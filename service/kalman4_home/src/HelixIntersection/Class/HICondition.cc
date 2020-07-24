// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HICondition
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 19:05:23 EDT 1998
// $Id: HICondition.cc,v 1.1.1.1 1998/12/07 04:58:22 wsun Exp $
//
// Revision history
//
// $Log: HICondition.cc,v $
// Revision 1.1.1.1  1998/12/07 04:58:22  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HICondition.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HelixIntersection.HICondition" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HICondition::HICondition() : m_printDiagnostics( false )
{
}

// HICondition::HICondition( const HICondition& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HICondition::~HICondition()
{
}

//
// assignment operators
//
// const HICondition& HICondition::operator=( const HICondition& rhs )
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
