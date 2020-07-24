// -*- C++ -*-
//
// Package:     HelixIntersection
// Module:      HIHelixOperation
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Werner Sun
// Created:     Tue Sep 22 19:06:38 EDT 1998
// $Id: HIHelixOperation.cc,v 1.1.1.1 1998/12/07 04:58:23 wsun Exp $
//
// Revision history
//
// $Log: HIHelixOperation.cc,v $
// Revision 1.1.1.1  1998/12/07 04:58:23  wsun
// First submission.
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HelixIntersection/HIHelixOperation.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"HelixIntersection.HIHelixOperation" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HIHelixOperation::HIHelixOperation() : m_printDiagnostics( false )
{
}

// HIHelixOperation::HIHelixOperation( const HIHelixOperation& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HIHelixOperation::~HIHelixOperation()
{
}

//
// assignment operators
//
// const HIHelixOperation& HIHelixOperation::operator=( const HIHelixOperation& rhs )
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
