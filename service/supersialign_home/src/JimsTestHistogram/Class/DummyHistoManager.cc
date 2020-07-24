// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyHistoManager
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 23:05:58 EST 1999
// $Id: DummyHistoManager.cc,v 1.8 1999/10/08 21:17:57 cdj Exp $
//
// Revision history
//
// $Log: DummyHistoManager.cc,v $
// Revision 1.8  1999/10/08 21:17:57  cdj
// Manager should not delete default directory
//
// Revision 1.7  1999/10/07 18:06:26  hufnagel
// Numbers now use float instead of double
//
// Revision 1.6  1999/10/06 20:26:15  cdj
// added setDefaultDir to Manager
//
// Revision 1.5  1999/09/29 17:53:39  cdj
// added histogram directories
//
// Revision 1.4  1999/09/14 18:33:00  mkl
// don't print warning from dtor -- superfluous
//
// Revision 1.3  1999/09/10 01:11:55  mkl
// now allow booking of histograms with automatic id allocation
//
// Revision 1.2  1999/09/06 21:47:24  mkl
// move to new histogramming interface: dummy implementation
//
// Revision 1.1.1.1  1999/05/12 00:10:10  billm
// imported DummyHistogram sources
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <assert.h>

// user include files
#include "Experiment/report.h"
#include "DummyHistogram/DummyHistoManager.h"
#include "DummyHistogram/DummyHist1D.h"
#include "DummyHistogram/DummyHist2D.h"
#include "DummyHistogram/DummyHistProf.h"
#include "DummyHistogram/DummyNtuple.h"
#include "DummyHistogram/DummyNumber.h"
#include "DummyHistogram/DummyNumberWithErrors.h"
#include "DummyHistogram/DummyDir.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DummyHistogram.DummyHistoManager" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DummyHistoManager.cc,v 1.8 1999/10/08 21:17:57 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DummyHistoManager::DummyHistoManager()
   : m_root( new DummyDir(string()) ),
     m_default( m_root )
{
   report( WARNING, kFacilityString )
      << "Creating Dummy HistogramManager." << endl;  

   assert( 0!= m_root);
}

// DummyHistoManager::DummyHistoManager( const DummyHistoManager& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DummyHistoManager::~DummyHistoManager()
{
   delete m_root;
}

//
// assignment operators
//
// const DummyHistoManager& DummyHistoManager::operator=( const DummyHistoManager& rhs )
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
DummyHistoManager::mkdir( const string& iName ) 
{
   return m_default->mkdir( iName );
}

// 1D histogram
HIHist1D* 
DummyHistoManager::histogram( Count          iId,
			      const string&  iTitle,
			      Count   iNChannels,
			      float          iLowEdge,
			      float          iHighEdge,
			      float          iBitsPerChannel )
{
   return m_default->histogram( iId,
				iTitle,
				iNChannels,
				iLowEdge,
				iHighEdge,
				iBitsPerChannel ) ;
}

// 1D histogram with automatic id assignment
HIHist1D* 
DummyHistoManager::histogram( const string&  iTitle,
			      Count   iNChannels,
			      float          iLowEdge,
			      float          iHighEdge,
			      float          iBitsPerChannel )
{
   return m_default->histogram( iTitle,
				iNChannels,
				iLowEdge,
				iHighEdge,
				iBitsPerChannel );
}

// 2D histogram
HIHist2D* 
DummyHistoManager::histogram( Count          iId,
			      const string&  iTitle,
			      unsigned int   iNChannelsX, 
			      float          iLowEdgeX, 
			      float          iHighEdgeX,
			      unsigned int   iNChannelsY, 
			      float          iLowEdgeY, 
			      float          iHighEdgeY,
			      float          iBitsPerChannel )
{
   return m_default->histogram( iId,
				iTitle,
				iNChannelsX,
				iLowEdgeX,
				iHighEdgeX,
				iNChannelsY,
				iLowEdgeY,
				iHighEdgeY,
				iBitsPerChannel ) ;
}

// 2D histogram with automatic id assignment
HIHist2D* 
DummyHistoManager::histogram( const string&  iTitle,
			      unsigned int   iNChannelsX, 
			      float          iLowEdgeX, 
			      float          iHighEdgeX,
			      unsigned int   iNChannelsY, 
			      float          iLowEdgeY, 
			      float          iHighEdgeY,
			      float          iBitsPerChannel )
{
   return m_default->histogram(iTitle,
			       iNChannelsX,
			       iLowEdgeX,
			       iHighEdgeX,
			       iNChannelsY,
			       iLowEdgeY,
			       iHighEdgeY,
			       iBitsPerChannel ) ;
}

// profile histogram
HIHistProf* 
DummyHistoManager::profile( Count          iId,
			    const string&  iTitle,
			    unsigned int   iNChannels, 
			    float          iLowEdgeX, 
			    float          iHighEdgeX,
			    float          iLowEdgeY, 
			    float          iHighEdgeY,
			    HIHistProf::DisplayErrorsAs iDisplayErrorsAs )
{
   return m_default->profile( iId,
			      iTitle,
			      iNChannels,
			      iLowEdgeX,
			      iHighEdgeX,
			      iLowEdgeY,
			      iHighEdgeY,
			      iDisplayErrorsAs ) ;
}

// profile histogram with automatic id assignment
HIHistProf* 
DummyHistoManager::profile( const string&  iTitle,
			    unsigned int   iNChannels, 
			    float          iLowEdgeX, 
			    float          iHighEdgeX,
			    float          iLowEdgeY, 
			    float          iHighEdgeY,
			    HIHistProf::DisplayErrorsAs iDisplayErrorsAs )
{
   return m_default->profile(iTitle,
			     iNChannels,
			     iLowEdgeX,
			     iHighEdgeX,
			     iLowEdgeY,
			     iHighEdgeY,
			     iDisplayErrorsAs ) ;
}

// ntuple
HINtuple* 
DummyHistoManager::ntuple( Count          iId,
			   const string&  iTitle,
			   unsigned int   iNumberOfVariables,
			   unsigned int   iChunkSize,
			   const char* const* iLabels )
{
   return m_default->ntuple( iId,
			     iTitle,
			     iNumberOfVariables,
			     iChunkSize,
			     iLabels ) ;
}

// ntuple with automatic id assignment
HINtuple* 
DummyHistoManager::ntuple( const string&  iTitle,
			   unsigned int   iNumberOfVariables,
			   unsigned int   iChunkSize,
			   const char* const* iLabels )
{
   return m_default->ntuple( iTitle,
			     iNumberOfVariables,
			     iChunkSize,
			     iLabels ) ;
}

HINumber* 
DummyHistoManager::number( const string& iName,
			   float         iValue )
{
   return m_default->number( iName, iValue );
}

HINumberWithErrors* 
DummyHistoManager::numberWithErrors( const string& iName,
				     float         iValue,
				     float         iPositiveError,
				     float         iNegativeError )
{
   return m_default->numberWithErrors( iName, iValue,
				       iPositiveError, iNegativeError );
}

//HIDirSwitch* 
//DummyHistoManager::mkSwitch()
//{
//   report( ERROR, kFacilityString )
//      << "mkSwitch() not implemented yet"
//      << endl;
//   assert( false );
//   return 0;
//}

void
DummyHistoManager::setDefaultDir( const string& iDirectoryName )
{
   m_default = m_root->mkdir( iDirectoryName );
}

//
// const member functions
//
HIHistDir*
DummyHistoManager::defaultDir() const
{
   return m_default;
}

HIHistDir*
DummyHistoManager::rootDir() const
{
   return m_root;
}

//
// static member functions
//
