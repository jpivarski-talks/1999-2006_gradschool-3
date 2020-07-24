// -*- C++ -*-
//
// Package:     HbookHistogram
// Module:      HbookManager
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Marc Soussa
// Created:     Tue Feb  2 13:44:21 EST 1999
// $Id: HbookManager.cc,v 1.8 2000/01/31 22:54:52 cdj Exp $
//
// Revision history
//
// $Log: HbookManager.cc,v $
// Revision 1.8  2000/01/31 22:54:52  cdj
// manager's root zebra directory's name now includes logical unit number
//
// Revision 1.7  1999/10/08 21:21:37  cdj
// Manager should not delete default directory
//
// Revision 1.6  1999/10/07 18:07:51  hufnagel
// Numbers now use float instead of double
//
// Revision 1.5  1999/10/07 17:44:16  mkl
// allow repeated calls to setDefaultDir method; fix up warning message about changing ids when id is already taken
//
// Revision 1.4  1999/10/06 20:27:20  cdj
// added setDefaultDir to Manager
//
// Revision 1.3  1999/09/29 17:55:48  cdj
// added histogram directories
//
// Revision 1.2  1999/09/10 01:12:33  mkl
// now allow booking of histograms with automatic id allocation
//
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:58  cdj
// imported HbookHistogram source
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string.h>
#include <vector>
#include <stdlib.h> // For 'exit'
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"      // For error report
#include "HbookHistogram/HbookManager.h"
#include "HbookHistogram/HbookFile.h"
#include "HbookHistogram/HbookHistDir.h"

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

static const char* const kFacilityString = "HbookHistogram.HbookManager" ;
const char* kDefaultTopDir = "HBKMGR";

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: HbookManager.cc,v 1.8 2000/01/31 22:54:52 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
HbookManager::HbookManager( const char* iFileName,
			    SmallCount  iLogicalUnitNumber,
			    SmallCount  iRecordLength ) 
   : m_file( *(new HbookFile( iFileName,
			      string( 
				 string(kDefaultTopDir)+
				 char('0'+((iLogicalUnitNumber/10)%10) ) +
				 char('0'+(iLogicalUnitNumber%10)) ).c_str(),
			      iLogicalUnitNumber,
			      iRecordLength ) ) ),
     m_root(  new HbookHistDir( &m_file ) ),
     m_default(0),
     m_defaultDirName("")
{
  if ( ( 0 == &m_file ) ||
       ( 0 == m_root ) ) {
    report ( EMERGENCY,
	     kFacilityString )
	       << "Unable to allocate memory" ;
    assert( (0 != &m_file) && ( 0 != m_root ) );
    exit( 1 ) ;
  }
}

// HbookManager::HbookManager( const HbookManager& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookManager::~HbookManager()
{
   // delete root 
   delete m_root;
//
// Close file (which will write out Ntuples)
//
  delete &m_file ;
}

//
// assignment operators
//
// const HbookManager& HbookManager::operator=( const HbookManager& rhs )
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
HIHistDir*
HbookManager::mkdir( const string& iName )
{
   return makeDefaultDir()->mkdir( iName );
}

// 1D histogram
HIHist1D* 
HbookManager::histogram( Count iId,
			 const string&  iTitle,
			 unsigned int   iNChannels,
			 float          iLowEdge,
			 float          iHighEdge,
			 float          iBitsPerChannel )
{
   return makeDefaultDir()->histogram( iId,
				iTitle,
				iNChannels,
				iLowEdge,
				iHighEdge,
				iBitsPerChannel );
}

// 1D histogram with automatic id assignment
HIHist1D* 
HbookManager::histogram( const string&  iTitle,
			 unsigned int   iNChannels,
			 float          iLowEdge,
			 float          iHighEdge,
			 float          iBitsPerChannel )
{
   return makeDefaultDir()->histogram( iTitle,
				iNChannels,
				iLowEdge,
				iHighEdge,
				iBitsPerChannel );
}

// 2D histogram
HIHist2D* 
HbookManager::histogram( Count         iId,
			 const string& iTitle,
			 unsigned int  iNChannelsX,
			 float         iLowEdgeX,
			 float         iHighEdgeX,
			 unsigned int  iNChannelsY,
			 float         iLowEdgeY,
			 float         iHighEdgeY,
			 float         iBitsPerChannel )
{
   return makeDefaultDir()->histogram( iId,
				iTitle,
				iNChannelsX,
				iLowEdgeX,
				iHighEdgeX,
				iNChannelsY,
				iLowEdgeY,
				iHighEdgeY,
				iBitsPerChannel );
}

// 2D histogram with automatic id assignment
HIHist2D* 
HbookManager::histogram( const string& iTitle,
			 unsigned int  iNChannelsX,
			 float         iLowEdgeX,
			 float         iHighEdgeX,
			 unsigned int  iNChannelsY,
			 float         iLowEdgeY,
			 float         iHighEdgeY,
			 float         iBitsPerChannel )
{
   return makeDefaultDir()->histogram( iTitle,
				iNChannelsX,
				iLowEdgeX,
				iHighEdgeX,
				iNChannelsY,
				iLowEdgeY,
				iHighEdgeY,
				iBitsPerChannel );
}

// profile histogram
HIHistProf* 
HbookManager::profile( Count         iId,
		       const string& iTitle,
		       unsigned int  iNChannels,
		       float         iLowEdgeX,
		       float         iHighEdgeX,
		       float         iLowEdgeY,
		       float         iHighEdgeY,
		       HIHistProf::DisplayErrorsAs iDisplayErrorsAs )
{
   return makeDefaultDir()->profile( iId,
			      iTitle,
			      iNChannels,
			      iLowEdgeX,
			      iHighEdgeX,
			      iLowEdgeY,
			      iHighEdgeY,
			      iDisplayErrorsAs) ;
   
}

// profile histogram with automatic id assignment
HIHistProf* 
HbookManager::profile( const string& iTitle,
		       unsigned int  iNChannels,
		       float         iLowEdgeX,
		       float         iHighEdgeX,
		       float         iLowEdgeY,
		       float         iHighEdgeY,
		       HIHistProf::DisplayErrorsAs iDisplayErrorsAs )
{

   return makeDefaultDir()->profile( iTitle,
			      iNChannels,
			      iLowEdgeX,
			      iHighEdgeX,
			      iLowEdgeY,
			      iHighEdgeY,
			      iDisplayErrorsAs );
}

// ntuple 
HINtuple* 
HbookManager::ntuple( Count         iId,
		      const string& iTitle,
		      unsigned int  iNumberOfVariables,
		      unsigned int  iChunkSize,
		      const char* const*  iLabels )
{
   return makeDefaultDir()->ntuple( iId,
			     iTitle,
			     iNumberOfVariables,
			     iChunkSize,
			     iLabels );
}

// ntuple with automatic id assignment
HINtuple* 
HbookManager::ntuple( const string& iTitle,
		      unsigned int  iNumberOfVariables,
		      unsigned int  iChunkSize,
		      const char* const*  iLabels )
{
   return makeDefaultDir()->ntuple( iTitle,
			     iNumberOfVariables,
			     iChunkSize,
			     iLabels );
}

HINumber* 
HbookManager::number( const string& iName,
		      float         iValue )
{
   return makeDefaultDir()->number( iName, iValue );
}

HINumberWithErrors* 
HbookManager::numberWithErrors( const string& iName,
				float         iValue,
				float         iPositiveError,
				float         iNegativeError )
{
   return makeDefaultDir()->numberWithErrors( iName, iValue, 
				      iPositiveError, iNegativeError );
}


//HIDirSwitch* 
//HbookManager::mkSwitch()
//{
//   report( ERROR, kFacilityString )
//      << "mkSwitch() not implemented yet"
//      << endl;
//   assert( false );
//   return 0;
//}

HIHistDir*
HbookManager::makeDefaultDir()
{
   if( 0 == m_default ) {
      //use root directory if no defaultDirName has been set
      if( 0 == m_defaultDirName.size() ) {
	 m_default = m_root;
      } else {
	 m_default = m_root->mkdir( m_defaultDirName );
      }
   } 
   return m_default;
}

void
HbookManager::setDefaultDir( const string& iName )
{
   m_default = 0;
   m_defaultDirName = iName;
}

//
//
// const member functions
//
HIHistDir*
HbookManager::defaultDir() const
{
   return const_cast<HbookManager*>(this)->makeDefaultDir();
}

HIHistDir*
HbookManager::rootDir() const
{
   return m_root;
}

//
// static member functions
//


