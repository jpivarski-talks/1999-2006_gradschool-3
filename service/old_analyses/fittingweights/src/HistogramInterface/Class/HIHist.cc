// -*- C++ -*-
//
// Package:     <package>
// Module:      HIHist
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Tue Jan 12 13:48:03 EST 1999
// $Id: HIHist.cc,v 1.3 1999/12/23 23:25:21 cdj Exp $
//
// Revision history
//
// $Log: HIHist.cc,v $
// Revision 1.3  1999/12/23 23:25:21  cdj
// added arithmetic operations on histograms
//
// Revision 1.2  1999/09/07 18:53:18  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:04  cdj
// first submission
//
#include "Experiment/Experiment.h"
#include "Experiment/types.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"

#include "HistogramInterface/HIHist.h"
#include "HistogramInterface/HIHistFitFunc.h"

#include <string>
//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HIHist" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HIHist.cc,v 1.3 1999/12/23 23:25:21 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HIHist::HIHist( Count         iId,
		const string& iTitle ) :
  m_id( iId ) ,
  m_title( iTitle ) ,
  m_fitFunc( NULL )
{
}

// HIHist::HIHist( const HIHist& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

//
HIHist::~HIHist()
{
}
//
//
// assignment operators
//
// const HIHist& HIHist::operator=( const HIHist& rhs )
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
// Identifiers
//
Count 
HIHist::id ( void ) const
{
  return ( m_id ) ;
}
//
const string& 
HIHist::title ( void ) const
{
  return ( m_title ) ;
}

void
HIHist::attachFitFunc( HIHistFitFunc* fitFunc )
{
   m_fitFunc = fitFunc;
}

//
//
// const member functions
//

//
// static member functions
//




