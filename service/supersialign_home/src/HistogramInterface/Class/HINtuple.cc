// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HINtuple
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Wed Feb 17 15:44:27 EST 1999
// $Id: HINtuple.cc,v 1.3 2001/08/28 15:17:28 cdj Exp $
//
// Revision history
//
// $Log: HINtuple.cc,v $
// Revision 1.3  2001/08/28 15:17:28  cdj
// now holds onto title and labels
//
// Revision 1.2  1999/09/07 18:53:22  mkl
// new histogram interface
//
// Revision 1.1  1999/02/23 20:27:46  cdj
// first submission
//

#include "Experiment/Experiment.h"
#include "Experiment/types.h"
#include <string.h>

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "HistogramInterface/HINtuple.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HistogramInterface.HINtuple" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HINtuple.cc,v 1.3 2001/08/28 15:17:28 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HINtuple::HINtuple( Count         iId,
		    const string& iTitle,
		    unsigned int  iNumberOfVariables,
		    unsigned int  iChunkSize,
		    const char* const* iLabels ) :
  m_id( iId ) ,
  m_title( iTitle ) ,
  m_numberofvariables( iNumberOfVariables ) ,
  m_chunksize( iChunkSize ) ,
  m_labels(0)
{
   //need to copy labels
   m_labels = new char* [iNumberOfVariables];
   for( unsigned int index = 0; 
        index < iNumberOfVariables;
        ++index ) {
      m_labels[index] = new char[ strlen(iLabels[index] ) + 1];
      strcpy( m_labels[index] , iLabels[index] );
   }
}

// HINtuple::HINtuple( const HINtuple& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HINtuple::~HINtuple()
{
   for( unsigned int index = 0;
        index < m_numberofvariables;
        ++index ) {
      delete m_labels[index];
   }
   delete m_labels;
}

//
// assignment operators
//
// const HINtuple& HINtuple::operator=( const HINtuple& rhs )
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
Count 
HINtuple::id ( void ) const 
{
  return ( m_id ) ;
}

const string& 
HINtuple::title ( void ) const
{
  return ( m_title ) ;
}
//
unsigned int 
HINtuple::numberOfVariables ( void ) const
{
  return ( m_numberofvariables ) ;
}
//
unsigned int 
HINtuple::chunkSize ( void ) const
{
  return ( m_chunksize ) ;
}
//
const char* const* 
HINtuple::labels ( void ) const
{
  return ( m_labels ) ;
}
//
// static member functions
//
