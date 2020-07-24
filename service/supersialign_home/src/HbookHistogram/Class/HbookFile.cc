// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookFile
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Thu Feb  4 09:30:26 EST 1999
// $Id: HbookFile.cc,v 1.3 2002/04/18 13:13:01 ajm36 Exp $
//
// Revision history
//
// $Log: HbookFile.cc,v $
// Revision 1.3  2002/04/18 13:13:01  ajm36
// print warning if path name for histogram file has capital letters
//
// Revision 1.2  2002/02/22 18:34:08  cdj
// changed which package to get closelun
//
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

#include "Experiment/Experiment.h"

// system include files
#include <string.h>
#include <stdlib.h> // For 'exit'

#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HbookHistogram/HbookFile.h"
#include "lunmgr/closelun.h"
#include "Utility/MultipleHook.h"
#include "HbookHistogram/hbook_functions.h"
#include "HbookHistogram/HbookRZItem.h"


// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogram.HbookFile" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HbookFile.cc,v 1.3 2002/04/18 13:13:01 ajm36 Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HbookFile::HbookFile( const char* pRequestedFileName,
		      const char* pRequestedTopDir,
		      SmallCount  iLogicalUnitNumber,
		      SmallCount  iRecordLength ) :
  m_name( new FChar[ strlen( pRequestedFileName ) + 1 ] ) ,
  m_topdir( new FChar[ strlen( pRequestedTopDir ) + 1 ] ) ,
  m_logicalUnitNumber( iLogicalUnitNumber ) ,
  m_itemsInFile( new HbookRZItemHooks( kMaxItemsInFile ) ) ,
  m_open( false )
{
  const FChar cChopt[2] = { "N" } ; // Create new file
  const FInteger recordLength = iRecordLength;

  if ( (0 == m_name ) || ( 0 == m_topdir ) || ( 0 == m_itemsInFile ) ) {
      delete [] m_name ;
      delete [] m_topdir ;
      delete m_itemsInFile ;

    report ( EMERGENCY,
	     kFacilityString )
	       << "Unable to allocate memory" ;
    exit( 1 );
  }

  strcpy( m_name , pRequestedFileName ) ;
  strcpy( m_topdir , pRequestedTopDir ) ;
  FInteger lengthTopdir = strlen( m_topdir ) ;
  FInteger lengthFileName = strlen( pRequestedFileName ) ;
  FInteger lengthChopt = strlen( cChopt ) ;
  FInteger istat;
  hbookinit_();

  int iChar = 0;
  DABoolean hasCapitalLetter = false;
  while(m_name[iChar] != '\0')
  {
     if(isupper(m_name[iChar])) hasCapitalLetter = true;
     if(m_name[iChar] == '/' && hasCapitalLetter)
     {
        report(WARNING, kFacilityString)
           <<"File name "<<m_name<<" has capital letters in directory path.\n"
           <<"Hbook will use all lower case letters.\n"
           <<"Your histogram file may not be opened properly!!!!"<<endl;
        break;
     }
     ++iChar;
  }
  
  hropen_( &m_logicalUnitNumber,
	   m_topdir,
	   m_name,
	   cChopt,
	   &recordLength,
	   &istat,
	   lengthTopdir,
	   lengthFileName,
	   lengthChopt );
  
  m_open = true ;

}

// HbookFile::HbookFile( const HbookFile& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookFile::~HbookFile()
{
  close() ;
  delete m_itemsInFile ;
  delete [] m_topdir ;
  delete [] m_name ;
}

//
// assignment operators
//
// const HbookFile& HbookFile::operator=( const HbookFile& rhs )
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
// Write a histogram out to this file
//
void 
HbookFile::storeHistogram( Count iId )
{
   const int id = iId;

   const FChar cSpace[2] = { "2" } ;
   //
   // Read in previous directory
   //
   const FChar cChopt[2] = { "R" } ;  // Required CHOPT
   char* cOlddir = new char[ 255 ] ;
   cOlddir[ 0 ] = '\0' ;
   FInteger lengthOlddir = 255 ;
   FInteger lengthChopt = strlen( cChopt ) ;
   
   hcdir_( &cOlddir[ 0 ],
	   &cChopt[ 0 ],
	   lengthOlddir,
	   lengthChopt ) ;
   
   cOlddir[ strcspn( cOlddir, cSpace ) ] = '\0' ;
   //
   // Set current directory to this file
   //
   const char cPrefix[ 3 ] = { "//" } ;
   char* cName =  new char[ strlen( cPrefix ) + strlen( m_topdir ) + 1 ] ;
   cName[ 0 ] = '\0' ;
   strcat( cName , cPrefix ) ;
   strcat( cName , m_topdir ) ;
   FInteger lengthName = strlen( cName ) ;
   FInteger lengthSpace = strlen( cSpace ) ;
   
   hcdir_( &cName[ 0 ],
	   &cSpace[ 0 ],
	   lengthName,
	   lengthSpace );
   
   delete [] cName ;
   //
   // Store histogram
   //
   FInteger iCycle ;
   
   hrout_( &id,
	   &iCycle,
	   &cSpace[ 0 ],
	   lengthSpace ) ;
   //
   // Reset directory
   //
   lengthOlddir = strlen( cOlddir ) ;
   
   hcdir_( &cOlddir[ 0 ],
	   &cSpace[ 0 ],
	   lengthOlddir,
	   lengthSpace );
   
   delete [] cOlddir ;
}

//
// Close the file and shut down all links
//
void 
HbookFile::close( void )
{
  if ( !m_open ) {
    return ;
  }
  if ( m_itemsInFile->hooked() ) {
    vector< HbookRZItem* > stackOfItems( kMaxItemsInFile ) ;
    m_itemsInFile->partners( &stackOfItems ) ;
    
    while ( !stackOfItems.empty() ) {
      HbookRZItem* currentItem = *( stackOfItems.begin() ) ;
      if ( ! ( ( m_itemsInFile->unHook( currentItem ) ) &&
	       ( currentItem->unHookFile( this ) ) ) ) {
	report ( EMERGENCY,
		 kFacilityString )
		   << "Cannot cleanly close HbookRZItem associated "
		   << "with HbookFile" ;
	return ;
      }
    }
  }

  FInteger lengthTopdir = strlen( m_topdir ) ;

  hrend_( m_topdir,
	  lengthTopdir ) ;

  closelun_( &m_logicalUnitNumber ) ;

  m_open = false ;
}
//
// Hookup item
//
// Set up the Hook - to - Partner relationship
//
DABoolean HbookFile::hookUpHbookRZItem( HbookRZItem *pItem )
{
  if ( !m_open ) {
    return ( false ) ;
  }
  return ( m_itemsInFile->hookUp( pItem ) ) ;
}
//
// Unhook item
//
// Shut down the Hook - to - Partner relationship
//
DABoolean HbookFile::unHookHbookRZItem( HbookRZItem *pItem )
{
  if ( !m_open ) {
    return ( false ) ;
  }
  return ( m_itemsInFile->unHook( pItem ) ) ;
}
//
// Get top directory
//
const char* HbookFile::getTopDir( void ) const
{
  return ( m_topdir ) ;
}
//
// Return a pointer to a filename
//
const char* HbookFile::name( void ) const
{
  return ( m_name ) ;
}
  
// const member functions
//

//
// static member functions
//




