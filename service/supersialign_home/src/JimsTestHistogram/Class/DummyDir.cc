// -*- C++ -*-
//
// Package:     <DummyHistogram>
// Module:      DummyDir
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Tue Sep 28 14:20:30 EDT 1999
// $Id: DummyDir.cc,v 1.4 2001/07/21 15:44:52 cdj Exp $
//
// Revision history
//
// $Log: DummyDir.cc,v $
// Revision 1.4  2001/07/21 15:44:52  cdj
// conform to new HIHistDir interface
//
// Revision 1.3  1999/10/07 18:06:26  hufnagel
// Numbers now use float instead of double
//
// Revision 1.2  1999/10/01 18:43:07  mkl
// fix up memory leaks
//
// Revision 1.1  1999/09/29 17:53:38  cdj
// added histogram directories
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "DummyHistogram/DummyDir.h"
#include "DummyHistogram/DummyHist1D.h"
#include "DummyHistogram/DummyHist2D.h"
#include "DummyHistogram/DummyHistProf.h"
#include "DummyHistogram/DummyNtuple.h"
#include "DummyHistogram/DummyNumber.h"
#include "DummyHistogram/DummyNumberWithErrors.h"

// STL classes
#include <string>
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "DummyHistogram.DummyDir" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DummyDir.cc,v 1.4 2001/07/21 15:44:52 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DummyDir::DummyDir( const string& iName ) 
   : HIHistDir( iName ),
     m_nextId( 0 )
{
}

// DummyDir::DummyDir( const DummyDir& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

DummyDir::~DummyDir()
{
}

//
// assignment operators
//
// const DummyDir& DummyDir::operator=( const DummyDir& rhs )
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
DummyDir::makeDir( const string& iName ) 
{
   HIHistDir* histDir = new DummyDir( iName );
   return histDir;
}

HIHist1D* 
DummyDir::makeHistogram( Count          iId,
		     const string&  iTitle,
		     Count   iNChannels,
		     float          iLowEdge,
		     float          iHighEdge,
		     float          iBitsPerChannel )
{
   report( DEBUG, kFacilityString )
      << "New Dummy 1D histogram." << endl;

   Count id = checkValidId( iId );
   DummyHist1D* hist = new DummyHist1D( id,
					iTitle,
					iNChannels,
					iLowEdge,
					iHighEdge,
					iBitsPerChannel ) ;
   return hist;
}

// 2D histogram
HIHist2D* 
DummyDir::makeHistogram( Count          iId,
			      const string&  iTitle,
			      unsigned int   iNChannelsX, 
			      float          iLowEdgeX, 
			      float          iHighEdgeX,
			      unsigned int   iNChannelsY, 
			      float          iLowEdgeY, 
			      float          iHighEdgeY,
			      float          iBitsPerChannel )
{
   report( DEBUG, kFacilityString )
      << "New Dummy 2D histo." << endl;
   
   Count id = checkValidId( iId );
   DummyHist2D* hist = new DummyHist2D( id,
					iTitle,
					iNChannelsX,
					iLowEdgeX,
					iHighEdgeX,
					iNChannelsY,
					iLowEdgeY,
					iHighEdgeY,
					iBitsPerChannel ) ;
   return hist;
}

// profile histogram
HIHistProf* 
DummyDir::makeProfile( Count          iId,
		   const string&  iTitle,
		   unsigned int   iNChannels, 
		   float          iLowEdgeX, 
		   float          iHighEdgeX,
		   float          iLowEdgeY, 
		   float          iHighEdgeY,
		   HIHistProf::DisplayErrorsAs iDisplayErrorsAs )
{
   report( DEBUG, kFacilityString )
      << "New Dummy profile histo." << endl;

   Count id = checkValidId( iId );
   DummyHistProf* hist = new DummyHistProf( id,
					    iTitle,
					    iNChannels,
					    iLowEdgeX,
					    iHighEdgeX,
					    iLowEdgeY,
					    iHighEdgeY,
					    iDisplayErrorsAs ) ;
   return ( hist ) ;
}

// ntuple
HINtuple* 
DummyDir::makeNtuple( Count          iId,
			   const string&  iTitle,
			   unsigned int   iNumberOfVariables,
			   unsigned int   iChunkSize,
			   const char* const* iLabels )
{
   report( DEBUG, kFacilityString )
      << "New Dummy ntuple." << endl;
   
   Count id = checkValidId( iId );
   DummyNtuple* ntuple = new DummyNtuple( id,
					  iTitle,
					  iNumberOfVariables,
					  iChunkSize,
					  iLabels ) ;
   return ntuple;
}

HINumber* 
DummyDir::makeNumber( const string& iName,
		  float         iValue )
{
   report( DEBUG, kFacilityString )
      << "New Dummy number." << endl;
   
   DummyNumber* number( new DummyNumber( iName,
					 iValue ) );
   
   return number;
}

HINumberWithErrors* 
DummyDir::makeNumberWithErrors( const string& iName,
			    float         iValue,
			    float         iPositiveError,
			    float         iNegativeError )
{
   report( WARNING, kFacilityString )
      << "New Dummy number." << endl;
   
   DummyNumberWithErrors* numberWithErrors =
      new DummyNumberWithErrors( iName,
				 iValue,
				 iPositiveError,
				 iNegativeError );
   
   return numberWithErrors;
}

Count 
DummyDir::nextId()
{
   // dummy implementation
   return ++m_nextId;
}

Count
DummyDir::checkValidId( Count iId )
{
   // dummy implementation
   return m_nextId;
}

//
// const member functions
//

//
// static member functions
//
