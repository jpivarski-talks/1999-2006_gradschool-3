#if !defined(CONSTANTSDELIVERY_CONSTANTSBINDER_CC)
#define CONSTANTSDELIVERY_CONSTANTSBINDER_CC
// -*- C++ -*-
//
// Library:     ConstantsDelivery
// Module:      ConstantsSourceBinder
// 
// Description: 

// Usage:

// Invariants:
//
// Author:      Rich Baker
// Created:     Tues Nov 18 1997
//
// $Id: ConstantsSourceBinder.cc,v 1.5 2000/04/27 21:14:00 cdj Exp $
//
// Revision history
//
// $Log: ConstantsSourceBinder.cc,v $
// Revision 1.5  2000/04/27 21:14:00  cdj
// now works on OSF using MICO ORB
//
// Revision 1.4  1999/06/30 23:13:10  cdj
// autogenerate code to create default Stream list
//
// Revision 1.3  1999/04/29 18:45:35  cdj
// updated to use new constants implementation
//
// Revision 1.2  1998/11/24 20:12:43  baker
// Change normal constructor for ConstantsSourceBinder
// to allow specification of the desired Federated
// Database.
// Added a Constructor for DBConstantsRecordInfo
// that takes a single string variable.
//
// Revision 1.1  1998/11/16 22:03:32  mkl
// renamed ConstantsBinder/Controller ConstantsSourceBinder/Controller
//
// Revision 1.6  1998/11/10 22:12:07  baker
// Several Fixes.
// Constants Delivery Tested and Working!
// Some kludges still need to be worked on though.
//
// Revision 1.5  1998/11/07 19:12:45  baker
// Added Data Source ID information to Controller
//
// Revision 1.4  1998/11/04 18:55:36  baker
// Fix stupid errors so that this library actually compiles (finally!)
//
// Revision 1.3  1998/11/04 15:39:08  baker
// Continuing updates to get this library compiled
//
// Revision 1.2  1998/09/30 20:39:57  baker
// Changed class names and include file names as
// necessary for migration from old DataBaseDelivery
// to new ConstantsDelivery packages.
//
// Revision 1.1  1998/09/30 18:22:58  baker
// // Renamed DataBaseDelivery files as ConstantsDelivery
//
// Revision 1.1.1.2  1998/01/14 20:56:52  baker
// make DataBaseDelivery library compileable
//
// Revision 1.1.1.1  1997/12/17 20:25:11  cleo3
// imported DataBaseDelivery sources
//

// system include files
// Jan 14, 1998 Rich Baker
// Added includes of Experiment.h and <vector>
#include "Experiment/Experiment.h"
#include <vector>
#include <map>

// user include files
#include "ConstantsDelivery/ConstantsSourceBinder.h"
#include "DataHandler/Stream.h"
#include "DataHandler/StreamSet.h"
// Jan 14, 1998 Rich Baker
// Added include of DataBaseBinder.h
// September 30, 1998 Rich Baker
// Renamed DataBase -> Constants
#include "ConstantsDelivery/ConstantsSourceController.h"

// 11/7/98 Rich Baker
// Pass Data Source ID "ConstantsDatabase" to the base class.
// 11/24/98 Rich Baker
// New constructor to allow the user to specify the 
// Federated Database path to bind to and pass this 
// fdb Name to the base class.
ConstantsSourceBinder::ConstantsSourceBinder( 
   const string& fdbName,
   const DBConstantsSelectionTag& tag,
   const Stream::Set& iBindStreams ) :

   DataSourceBinder(  fdbName, 
		      tag.toString(),
		      iBindStreams ),
   mSelectTag( tag ) 
{
   if( 0 == iBindStreams.size() ) {
      //use defaults
      bindStreams( defaultStreams() );
   }
}

ConstantsSourceBinder::~ConstantsSourceBinder() {}

// 11/7/98 Rich Baker
// Pass Data Source ID from the base class to the ConstantsSourceController
// so the Binder and Controller will have the same source ID.

DataSourceController* 
   ConstantsSourceBinder::createDataSourceController( void ) const {

// 11/9/98 Rich Baker
// Added a Stream::Set data member to this class so that
// the streams to bind can be passed to the ConstantsSourceController
//   return new ConstantsSourceController( boundStreams(), dataSourceID(), mSelectTag );
   return new ConstantsSourceController( boundStreams(), dataSourceID(), mSelectTag );

}

const DBConstantsSelectionTag& ConstantsSourceBinder::selTag( void ) const {
   return mSelectTag;
}

#endif /* DATABASE_ConstantsSourceBinder_CC */

