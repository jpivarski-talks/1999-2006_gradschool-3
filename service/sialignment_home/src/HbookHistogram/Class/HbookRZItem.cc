// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookRZItem
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Thu Feb  4 13:03:45 EST 1999
// $Id: HbookRZItem.cc,v 1.1.1.1 1999/09/06 22:02:30 mkl Exp $
//
// Revision history
//
// $Log: HbookRZItem.cc,v $
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h> // For 'exit'
#include <string.h>

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
#include "HbookHistogram/HbookRZItem.h"
#include "Experiment/report.h"
#include "HbookHistogram/HbookFile.h"
#include "Utility/SingleHook.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogram.HbookRZItem" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HbookRZItem.cc,v 1.1.1.1 1999/09/06 22:02:30 mkl Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HbookRZItem::HbookRZItem( HbookFile* pFile ) :
  m_linkToFile( new FileHook )
{
  if ( 0 == m_linkToFile ) {
    report ( EMERGENCY,
	     kFacilityString )
	       << "Unable to allocate memory" ;
    exit( 1 ) ;
  }

  if ( ! ( ( pFile->hookUpHbookRZItem( this ) ) &&
	   ( m_linkToFile->hookUp( pFile ) ) ) ) {
    report ( EMERGENCY,
	     kFacilityString )
	       << "Can not link an Ntuple to the given file" ;
    exit( 1 );
  }

  m_open = true ;
}

// HbookRZItem::HbookRZItem( const HbookRZItem& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookRZItem::~HbookRZItem()
{
  close() ;
  delete m_linkToFile ;
}

//
//
// assignment operators
//
// const HbookRZItem& HbookRZItem::operator=( const HbookRZItem& rhs )
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
// Set up the Hook - to - Partner relationship
//
DABoolean HbookRZItem::hookUpFile( HbookFile* pFile )
{
  return ( m_linkToFile->hookUp ( pFile ) ) ;
}
//
// Shut down the Hook - to - Partner relationship
//
DABoolean HbookRZItem::unHookFile( HbookFile* pFile )
{
  if ( !m_linkToFile->unHook( pFile ) ) {
    return ( false ) ;
  }
  close() ;
  return ( true ) ;
}
//
// Close down the HbookRZItem for use
//
void HbookRZItem::close( void )
{
  if ( !m_open ) {
    return ;
  }

  if ( m_linkToFile->hooked() ) {
    (m_linkToFile->partner() )->unHookHbookRZItem( this ) ;
  }

  m_open = false ;
}
//
// Check to see if HbookRZItem is still open
//
DABoolean HbookRZItem::isOpen( void ) 
{
  return ( m_open ) ;
}
//
// const member functions
//

//
// static member functions
//
