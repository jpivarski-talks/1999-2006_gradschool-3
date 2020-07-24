// -*- C++ -*-
//
// Package:     <ConstantsDelivery>
// Module:      ConstantsSourceBinder_defaultStreams
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Jun 30 18:40:38 EDT 1999
// $Id: ConstantsSourceBinder_defaultStreams.cc,v 1.6 2000/10/31 20:07:28 hubert Exp $
//
// Revision history
//
// $Log: ConstantsSourceBinder_defaultStreams.cc,v $
// Revision 1.6  2000/10/31 20:07:28  hubert
// Added special treatment for MetaListRetrieval in defaultStreams
//
// Revision 1.5  2000/06/20 18:40:59  cdj
// when using Online MetaVersion tag, use pregenerated list of Constants types
//
// Revision 1.4  2000/06/17 19:15:01  cdj
// change no stream for constant report message a debug message
//
// Revision 1.3  2000/04/27 21:14:01  cdj
// now works on OSF using MICO ORB
//
// Revision 1.2  2000/04/22 18:38:05  cdj
// now finds streams for each MetaVersion Tag
//
// Revision 1.1  2000/04/14 20:52:27  cdj
// now looks up what constants are in the MetaVersion tag
//
// Revision 1.1  1999/06/30 23:13:14  cdj
// autogenerate code to create default Stream list
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ConstantsDelivery/ConstantsSourceBinder.h"
#include "ConstantsDelivery/ConstantsStreamMap.h"
#include "ConstantsDelivery/CDBinderException.h"

//_stream_incs_

#include "DBMetaVersion.hh"
#include "DBMetaVersionFactory.hh"
#include "DBConstants.hxx"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

static const char* const kFacilityString = "ConstantsDelivery.ConstantsSourceBinder" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ConstantsSourceBinder_defaultStreams.cc,v 1.6 2000/10/31 20:07:28 hubert Exp $";
static const char* const kTagString = "$Name:  $";


Stream::Set
ConstantsSourceBinder::defaultStreams() const {
   Stream::Set defaults;
   
   //loop over each selection Tag to determine what constants
   // are part of that tag 
   STL_VECTOR(string )::const_iterator itEnd = mSelectTag.selectionTags().end();
   for( STL_VECTOR(string)::const_iterator itTag = 
	   mSelectTag.selectionTags().begin();
	itTag != itEnd;
	++itTag ) {

      // Dont try to synchronize special tag "MetaListRetrieval". This tag is
      // not a regular tag and is used by the constants db server as flag to 
      // switch on retrieval by MetaList.
      if ( "MetaListRetrieval" == *itTag ) continue;

      //found out which constants are in this meta version
      // connect to the default meta version server
      DBConstants<DBMetaVersion> meta( dataSourceID(), //string()
				       STL_VECTOR(string)(1, *itTag),
				       mSelectTag.type() );
      
      //Check to see if server is available
      if( 0 == meta.bindStatus() ) {
	 throw CDBinderException( "could not find MetaVersion CORBA server" ) ;
      }

      meta.synchronize();
      
      if( ! meta.isValid() ) {
	 throw CDBinderException( "MetaVersion was invalid, see if using proper MetaVersion tag" );
      }

      if( "Online" == *itTag ) {
	 //We keep a special list of the Online constants since each 
	 // Online MetaVersion is allowed to have a different set
	 // of constant types

	 const STL_VECTOR(string) onlineConstants = 
	    ConstantsStreamMap::onlineConstants();

	 STL_VECTOR(string)::const_iterator itEnd = onlineConstants.end();
	 for( STL_VECTOR(string)::const_iterator itConstName = 
		 onlineConstants.begin();
	      itConstName != itEnd;
	      ++itConstName ) {
	    Stream::Type stream = 
	       ConstantsStreamMap::streamWithConstant( *itConstName );
	    if( stream != Stream::kNone ) {
	       defaults.add( stream );
	    } else {
	    report( DEBUG, kFacilityString ) 
	       << "could not find a Stream for Constant type " 
	       << *itConstName <<endl;
	    }
	 }
	 continue;
      }
      
      for ( int i = 0; i < meta.size(); i++ ) {
	 string thisType = meta[i].get_constantsType();
	 Stream::Type stream = 
	    ConstantsStreamMap::streamWithConstant( thisType );
	 
	 if ( stream != Stream::kNone ) {
	    // constant is available in this MetaVersion.
	    defaults.add( stream );
	 } else {
	    report( DEBUG, kFacilityString ) 
	       << "could not find a Stream for Constant type " << thisType <<endl;
	 }
	 
      }
   }
   return defaults;
}

