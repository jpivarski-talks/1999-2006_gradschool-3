// -*- C++ -*-
//
// Package:     <ConstantsModule>
// Module:      ConstantsModule
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Wed Apr 21 10:20:17 EDT 1999
// $Id: ConstantsModule.cc,v 1.11 2003/03/04 18:49:52 vk Exp $
//
// Revision history
//
// $Log: ConstantsModule.cc,v $
// Revision 1.11  2003/03/04 18:49:52  vk
//
// Add ConstantsPhase2Delivery & CP2SourceBinder support
//
// Revision 1.10  2001/01/15 16:03:15  cdj
// now uses ConstantsMetaListDelivery
//
// Revision 1.9  2000/04/27 21:17:19  cdj
// now works on OSF using MICO ORB
//
// Revision 1.8  2000/04/22 19:33:17  cdj
// added streams_exclude sub-command
//
// Revision 1.7  2000/04/14 20:54:59  cdj
// catch the exception thrown if problem with ConstantsSourceBinder's construction
//
// Revision 1.6  1999/06/23 16:58:48  cdj
// added specification of token name when adding source to SourceManager
//
// Revision 1.5  1999/04/29 19:20:47  cdj
// rewritten to allow multiple databases at the same time
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
#include <string>
//#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "ConstantsModule/ConstantsModule.h"
#include "ConstantsModule/ConstantsModuleCommand.h"

#include "JobControl/JobControl.h"

// source binder
#include "ConstantsDelivery/ConstantsSourceBinder.h"
#include "DataDelivery/DataSourceBinder.h"
#include "ConstantsPhase2Delivery/CP2SourceBinder.h"
#include "ConstantsMetaListDelivery/CMLSourceBinder.h"
#include "ConstantsDelivery/ConstantsSourceController.h"
#include "ConstantsDelivery/CDBinderException.h"

#include "JobControl/SourceManager.h"
#include "JobControl/SinkManager.h"
#include "JobControl/Binder.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//
static const char* const kFacilityString = "ConstantsModule.ConstantsModule" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: ConstantsModule.cc,v 1.11 2003/03/04 18:49:52 vk Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
ConstantsModule::ConstantsModule( SourceManager& iSourceManager )
   : Module( "ConstantsModule", "ConstantsModule" ),
     m_command( *( new ConstantsModuleCommand("constants", this ) ) ),
     m_sourceManager( iSourceManager )
{
   if( 0 == &m_command ) {
      report(EMERGENCY, kFacilityString) << "out of memory" << endl;
      assert(false);
      ::exit( -1 );
   }
}

ConstantsModule::ConstantsModule()
   : Module( "ConstantsModule", "ConstantsModule" ),
     m_command( *(new ConstantsModuleCommand("constants", this )) ),
     m_sourceManager( JobControl::instance()->sourceManager() )
{
   if( 0 == &m_command ) {
      report(EMERGENCY, kFacilityString) << "out of memory" << endl;
      assert(false);
      ::exit( -1 );
   }
}
// ConstantsModule::ConstantsModule( const ConstantsModule& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

ConstantsModule::~ConstantsModule()
{
   delete &m_command;
}

//
// assignment operators
//
// const ConstantsModule& ConstantsModule::operator=( const ConstantsModule& rhs )
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
DABoolean
ConstantsModule::openSource( const string& iFDBName,
			     const DBConstantsSelectionTag& iTag,
			     const StreamSet& iBindStreams,
			     const StreamSet& iExcludeStreams )
{

   DABoolean returnValue = false;
   try {

      // First let's check SelectType of DBVersion
      DBVersion::SelectType stype = iTag.type();
      DataSourceBinder* binder = 0;
      BinderBase* binderBase = 0;
      
      if( stype == DBVersion::Standard ) { 

        binder = new CP2SourceBinder( iFDBName, iTag, iBindStreams );

      } else if( stype == DBVersion::Meta ) {

        //See if the selection tag contains "MetaListRetrieval"
        // which signals the use of a MetaList
        DABoolean useMetaList = false;

        const string kMetaListTag( "MetaListRetrieval");

        STL_VECTOR(string)::const_iterator itEnd = iTag.selectionTags().end();
        for( STL_VECTOR(string)::const_iterator itTag = 
	      iTag.selectionTags().begin();
	   itTag != itEnd;
	   ++itTag ) {
	  if( kMetaListTag == *itTag ) {
	    useMetaList = true;
	    break;
	  }
        }
      
        if( useMetaList ) {
	  binder = new CMLSourceBinder( iFDBName, iTag, iBindStreams );
        } else {
	  binder = new ConstantsSourceBinder( iFDBName, iTag, iBindStreams );
        }

      } else {
	report(EMERGENCY, kFacilityString) 
	  << "neither standard or meta version specified" << endl;
	return false;
      }

      if( 0 != iExcludeStreams.size() ) {
        // exclude specified streams
        StreamSet::const_iterator itExcludeEnd = iExcludeStreams.end();
        StreamSet boundStreams;
        StreamSet::const_iterator itEnd = binder->boundStreams().end();
        for (StreamSet::const_iterator itStream = binder->boundStreams().begin();
	      itStream != itEnd;
	      ++itStream ) {
          if( ! iExcludeStreams.contains( *itStream ) ) {
            boundStreams.add( *itStream );
          }
        }
        binder->bindStreams( boundStreams );
      }

      binderBase =
        new Binder< DataSourceBinder >( binder, BinderBase::kConstants );

      if( (0 == binder) || (0 == binderBase) ) {
	 report(EMERGENCY, kFacilityString) << "out of memory" << endl;
	 assert(false);
	 ::exit(1);
      }
      
      //use iFDBName as the name of the sources token
      string tokenName( iFDBName );
      returnValue = m_sourceManager.addSource( binderBase,
					       tokenName );
      if( false == returnValue ) {
	 report(WARNING, kFacilityString ) 
	    <<"you probably tried to open the constants database twice!"
	    <<endl;
      }
   } catch( CDBinderException& iException  ) {
      report( ERROR, kFacilityString ) << iException.what() << endl;
      returnValue = false;
   }
   
   return returnValue;
}
//
// static member functions
//

