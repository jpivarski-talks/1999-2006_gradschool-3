// -*- C++ -*-
//
// package:     <TrackFinder>
// Module:      TrackFinder
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Thu Apr 16 16:04:52 EDT 1998
// $Id: TrackFinder.cc,v 1.14 2001/06/05 14:01:36 cdj Exp $
//
// Revision history
//
// $Log: TrackFinder.cc,v $
// Revision 1.14  2001/06/05 14:01:36  cdj
// no longer leak memory when exception thrown
//
// Revision 1.13  2001/02/02 21:16:35  lyon
// Remove return 0 if exception -- instead return an empty vector
//
// Revision 1.12  2000/02/08 18:27:33  lyon
// Allow for no bunch finder mode
//
// Revision 1.11  2000/01/13 22:14:47  lyon
// Added set functions for the flags and now checks for valid before filling hit lists (in case extract failed and exception was caught)
//
// Revision 1.10  2000/01/11 20:03:35  wsun
// Added flags to control how/if exceptions are handled.
//
// Revision 1.9  1999/12/14 04:50:53  lyon
// Added try and catch blocks
//
// Revision 1.8  1999/06/21 22:59:19  pg
//  Modifications for new Si hits
//
// Revision 1.7  1999/06/04 06:53:40  wsun
// Added argument to event() to dis/enable insertion of seeds.
//
// Revision 1.6  1999/04/23 19:13:16  marsh
// Changed "No XXXXX Calibrated Hits" message from EMERGENCY to NOTICE,
// since it's more of a notice than an actual emergency, _per se_.
//
// Revision 1.5  1999/04/08 07:36:05  wsun
// Added extraction and filtering of Si and Cathode hits in TrackFitter.
//
// Revision 1.4  1998/10/06 13:41:25  lyon
// Using STL Macros
//
// Revision 1.3  1998/08/18 22:50:15  wsun
// added parameter to toggle diagnostics printout
//
// Revision 1.2  1998/07/26 19:19:07  lyon
// Working version
//

#include "Experiment/Experiment.h"

// system include files
#if defined(AMBIGUOUS_STRING_FUNCTIONS_BUG)
#include <string>
#endif             
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <memory>

// user include files
#include "Experiment/report.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"

#include "TrackFinder/TrackFinder.h"

#include "CalibratedData/CalibratedDRHit.h"
#include "CalibratedData/CDPreliminaryID.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
#include "CalibratedData/CalibratedCathodeHit.h"

#include "DAException/DANoDataException.h"

// STL classes
#include <vector>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "TrackFinder.TrackFinder" ;

//
// static data member definitions
//

//
// constructors and destructor
//
TrackFinder::TrackFinder( DABoolean aContinueIfHitsMissing,
			  DABoolean aContinueIfAnyException )
   : m_continueIfHitsMissing( aContinueIfHitsMissing ),
     m_continueIfAnyException( aContinueIfAnyException )
{}

// TrackFinder::TrackFinder( const TrackFinder& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

TrackFinder::~TrackFinder()
{
}

////////////////////////////////////////////////////////////////////////
// Set the exception continuation flags
void TrackFinder::setExceptionContinuationFlags(
					    DABoolean continueIfHitsMissing,
					    DABoolean continueIfAnyException)
{
  m_continueIfHitsMissing = continueIfHitsMissing;
  m_continueIfAnyException = continueIfAnyException;
}
						

////////////////////////////////////////////////////////////////////////
// Special main track finding loop -- just DR hits and no BunchFinder
//
DABoolean
TrackFinder::eventNoBunchFinder(Frame& theFrame, int diagLevel, 
		   DABoolean insertTracks) const
{
 
  report(DEBUG, kFacilityString) << "In no bunch finder mode" << endl;
  
  // Get the record from the frame
  const Record& theRecord = theFrame.record(Stream::kEvent);
  
  // Filter the DR hits - but get hits from Preliminary
  auto_ptr<STL_VECTOR(const CalibratedDRHit*) > theDRHits( 
	       filterDRHits(theRecord, 
		     CDPreliminaryID::kSecondaryLabel) );

  auto_ptr<STL_VECTOR( const CalibratedSVRphiHit* ) > theSVRphiHits(
			       new STL_VECTOR(const CalibratedSVRphiHit* ) );
							  

  auto_ptr<STL_VECTOR( const CalibratedSVZHit* ) > theSVZHits(
			       new STL_VECTOR(const CalibratedSVZHit* ) );

  auto_ptr<STL_VECTOR( const CalibratedCathodeHit* ) > theCathodeHits(
			       new STL_VECTOR(const CalibratedCathodeHit* ) );
  
   // Do tracking (the tracking program holds the resultant tracks)
   DABoolean success = findTracks( theDRHits.get(),
				   theSVRphiHits.get(),
				   theSVZHits.get(),
				   theCathodeHits.get(),
				   theRecord,
				   diagLevel );

   if( success )
   {
      if( insertTracks )
      {
	 insertTracksIntoFrame( theFrame, diagLevel );
      }
   }
   else
   {
      report( WARNING, kFacilityString ) << "findTracks FAILED :-( " << endl ;
   }

   return success;
}


//////////////////////////////////////////////////////////////////////
// Do the main track finding loop for this event
//
DABoolean
TrackFinder::event( Frame& theFrame,
		    int diagLevel,
		    DABoolean insertTracks ) const
{
   // Get the record from the frame
   const Record& theRecord = theFrame.record(Stream::kEvent);
   
   // We could make a templated filterHits() function to handle all the
   // different CalibratedHit types, but 

   // Filter the DR hits
   STL_VECTOR(const CalibratedDRHit*)* theDRHits( 
		   filterDRHits(theRecord) );

   // Filter SVX hits
   STL_VECTOR( const CalibratedSVRphiHit* )* theSVRphiHits(
      filterSVRphiHits( theRecord ) ) ;
   STL_VECTOR( const CalibratedSVZHit* )* theSVZHits(
      filterSVZHits( theRecord ) ) ;

   // Filter DR cathode hits.
   STL_VECTOR( const CalibratedCathodeHit* )* theCathodeHits(
      filterCathodeHits( theRecord ) ) ;

   // Filter seeds
   //  TBA

   // Do tracking (the tracking program holds the resultant tracks)
   DABoolean success = findTracks( theDRHits,
				   theSVRphiHits,
				   theSVZHits,
				   theCathodeHits,
				   theRecord,
				   diagLevel );

   if( success )
   {
      if( insertTracks )
      {
	 insertTracksIntoFrame( theFrame, diagLevel );
      }
   }
   else
   {
      report( WARNING, kFacilityString ) << "findTracks FAILED :-( " << endl ;
   }

   // Delete the vector of hits - BUT DO NOT DELETE THE HITS INSIDE (they
   //          belong to the frame!!)

   delete theDRHits;
   delete theSVRphiHits ;
   delete theSVZHits ;
   delete theCathodeHits ;

   return success;
}




//////////////////////////////////////////////////////////////////////
// Filter the CalibratedDRHits 
//
//   Extract the hits and filter them if desired  - no cuts here
//
STL_VECTOR(const CalibratedDRHit*)* TrackFinder::filterDRHits(
   const Record& theRecord, const char* const drHitUsageTag ) const
{

   // Extract the hits
   FATable<CalibratedDRHit> allOfTheDRHits;
   try
   {
     // Use a usage tag if we got it
     if ( drHitUsageTag )
     {
       extract( theRecord, allOfTheDRHits, drHitUsageTag );
     }
     else
     {
       extract( theRecord, allOfTheDRHits );
     }
   }
   catch( NO_TABLE_EXCEPTION( CalibratedDRHit )& aException )
   {
      // Exception is caught if specified by user.  Otherwise,
      // it is rethrown.
      if( m_continueIfHitsMissing )
      {
	 report( WARNING, kFacilityString )
	    << "Continuing without DR hits." << endl;
      }
      else
      {
	 // This rethrows the exception currently being handled.
	 throw ;
      }
   }
   catch ( DAException& aException )
   {
      if( m_continueIfAnyException )
      {
	 report( WARNING, kFacilityString )
	    << "Continuing despite exception: \n " << aException.what()
	    << endl;
      }
      else
      {
	 throw ;
      }
   }

   // Make the vector of hits on which to run tracking 
   STL_VECTOR(const CalibratedDRHit*)* theDRHits = 
      new STL_VECTOR(const CalibratedDRHit*);

   // Did we actually get the hits (did the extract fail and the 
   // exception caught)
   if ( allOfTheDRHits.valid() )
   {

     // Loop over hits and put them into vector
     for ( FATable<CalibratedDRHit>::const_iterator iter = 
	     allOfTheDRHits.begin();
	   iter != allOfTheDRHits.end(); ++iter )
       {
	 theDRHits->push_back(&(*(iter))); 
       }
     
   }



   // done
   return theDRHits;
}
   
STL_VECTOR(const CalibratedSVRphiHit*)* TrackFinder::filterSVRphiHits(
   const Record& theRecord ) const
{
   // Extract the hits
   FATable<CalibratedSVRphiHit> allOfTheSVRphiHits;

   // Catch the NoData exception - just don't fill the vector
   try
   {
     extract( theRecord, allOfTheSVRphiHits );
   }
   catch( NO_TABLE_EXCEPTION( CalibratedSVRphiHit )& aException )
   {
      // Exception is caught if specified by user.  Otherwise,
      // it is rethrown.
      if( m_continueIfHitsMissing )
      {
	 report( WARNING, kFacilityString )
	    << "Continuing without SVRphi hits." << endl;
      }
      else
      {
	 // This rethrows the exception currently being handled.
	 throw ;
      }
   }
   catch ( DAException& aException )
   {
      if( m_continueIfAnyException )
      {
	 report( WARNING, kFacilityString )
	    << "Continuing despite exception: \n " << aException.what()
	    << endl;
      }
      else
      {
	 throw ;
      }
   }

   // Make the vector of hits on which to run tracking 
   STL_VECTOR(const CalibratedSVRphiHit*)* theSVRphiHits = 
      new STL_VECTOR(const CalibratedSVRphiHit*);

   // Did we actually get the hits (did the extract fail and the 
   // exception caught)
   if ( allOfTheSVRphiHits.valid() )
   {

     // Loop over hits and put them into vector
     for ( FATable<CalibratedSVRphiHit>::const_iterator iter = 
	     allOfTheSVRphiHits.begin();
	   iter != allOfTheSVRphiHits.end(); ++iter )
     {
       theSVRphiHits->push_back(&(*(iter))); 
     }

   }

   // done
   return theSVRphiHits;
}

STL_VECTOR(const CalibratedSVZHit*)* TrackFinder::filterSVZHits(
   const Record& theRecord ) const
{
   // Extract the hits
   FATable<CalibratedSVZHit> allOfTheSVZHits;

   try
   {
     extract( theRecord, allOfTheSVZHits );
   }
   catch( NO_TABLE_EXCEPTION( CalibratedSVZHit )& aException )
   {
      // Exception is caught if specified by user.  Otherwise,
      // it is rethrown.
      if( m_continueIfHitsMissing )
      {
	 report( WARNING, kFacilityString )
	    << "Continuing without SVZ hits." << endl;
      }
      else
      {
	 // This rethrows the exception currently being handled.
	 throw ;
      }
   }
   catch ( DAException& aException )
   {
      if( m_continueIfAnyException )
      {
	 report( WARNING, kFacilityString )
	    << "Continuing despite exception: \n " << aException.what()
	    << endl;
      }
      else
      {
	 throw ;
      }
   }

   // Make the vector of hits on which to run tracking 
   STL_VECTOR(const CalibratedSVZHit*)* theSVZHits = 
      new STL_VECTOR(const CalibratedSVZHit*);

   // Did we actually get the hits (did the extract fail and the 
   // exception caught)
   if ( allOfTheSVZHits.valid() )
   {

     // Loop over hits and put them into vector
     for ( FATable<CalibratedSVZHit>::const_iterator iter = 
	    allOfTheSVZHits.begin();  
  	   iter != allOfTheSVZHits.end(); ++iter )
     {  
        theSVZHits->push_back(&(*(iter))); 
     }

   }

   // done
   return theSVZHits;
}

STL_VECTOR(const CalibratedCathodeHit*)* TrackFinder::filterCathodeHits(
   const Record& theRecord ) const
{
   // Extract the hits
   FATable<CalibratedCathodeHit> allOfTheCathodeHits;
   try
   {
     extract( theRecord, allOfTheCathodeHits );
   }
   catch( NO_TABLE_EXCEPTION( CalibratedCathodeHit )& aException )
   {
      // Exception is caught if specified by user.  Otherwise,
      // it is rethrown.
      if( m_continueIfHitsMissing )
      {
	 report( WARNING, kFacilityString )
	    << "Continuing without cathode hits." << endl;
      }
      else
      {
	 // This rethrows the exception currently being handled.
	 throw ;
      }
   }
   catch( DAExceptionBase& aException )
   {
      if( m_continueIfAnyException )
      {
	 report( WARNING, kFacilityString )
	    << "Continuing despite exception: \n " << aException.what()
	    << endl;
      }
      else
      {
	 throw ;
      }
   }

   // Make the vector of hits on which to run tracking 
   STL_VECTOR(const CalibratedCathodeHit*)* theCathodeHits = 
      new STL_VECTOR(const CalibratedCathodeHit*);

   // Did we actually get the hits (did the extract fail and the 
   // exception caught)
   if ( allOfTheCathodeHits.valid() )
   {

     // Loop over hits and put them into vector
     for ( FATable<CalibratedCathodeHit>::const_iterator iter = 
	     allOfTheCathodeHits.begin();
	   iter != allOfTheCathodeHits.end(); ++iter )
       {
	 theCathodeHits->push_back(&(*(iter))); 
       }

   }
     
     // done
   return theCathodeHits;
}
   

//
// assignment operators
//
// const TrackFinder& TrackFinder::operator=( const TrackFinder& rhs )
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
