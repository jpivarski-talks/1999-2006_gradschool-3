// -*- C++ -*-
//
// Package:     <RunStatisticsSourceModule>
// Module:      RunStatisticsSourceModule
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D Jones
// Created:     Tue Jan 25 09:04:46 EST 2000
// $Id: RunStatisticsSourceModule.cc,v 1.2 2000/05/08 19:52:08 cdj Exp $
//
// Revision history
//
// $Log: RunStatisticsSourceModule.cc,v $
// Revision 1.2  2000/05/08 19:52:08  cdj
// now compiles on OSF with MICO
//
// Revision 1.1.1.1  2000/01/29 20:47:37  cdj
// imported RunStatisticsSourceModule source
//
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
#include <vector>
//#include <set>
#include <map>
//#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "RunStatisticsSourceModule/RunStatisticsSourceModule.h"
#include "RunStatisticsSourceModule/RSSourceBinder.h"

#include "JobControl/JobControl.h"
#include "JobControl/SourceManager.h"
#include "JobControl/Binder.h"

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
static const char* const kFacilityString = "RunStatisticsSourceModule.RunStatisticsSourceModule" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: RunStatisticsSourceModule.cc,v 1.2 2000/05/08 19:52:08 cdj Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
RunStatisticsSourceModule::RunStatisticsSourceModule()
   : Module( "RunStatisticsSourceModule", "RunStatisticsSourceModule" ),
     m_command( "runstatistics", this )
{
}

// RunStatisticsSourceModule::RunStatisticsSourceModule( const RunStatisticsSourceModule& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

RunStatisticsSourceModule::~RunStatisticsSourceModule()
{
}

//
// assignment operators
//
// const RunStatisticsSourceModule& RunStatisticsSourceModule::operator=( const RunStatisticsSourceModule& rhs )
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
RunStatisticsSourceModule::openSource(const StreamSet& iBindStreams ) const
{
   RSSourceBinder* binder = new RSSourceBinder(iBindStreams);

   BinderBase* binderBase =
      new Binder< RSSourceBinder >( binder, BinderType("runstatistics",true) );
   if( (0 == binder) || (0 == binderBase) ) {
      report(EMERGENCY, kFacilityString) << "out of memory" << endl;
      assert(false);
      ::exit(1);
   }

   //use iFDBName as the name of the sources token
   string sourceID( binder->dataSourceID());
   DABoolean returnValue = JobControl::instance()->sourceManager().addSource( 
      binderBase,
      sourceID );
   if( false == returnValue ) {
      report(WARNING, kFacilityString ) 
         <<"you probably tried to open the Run Statistics database twice!"
         <<endl;
   }
   return returnValue;
}

//
// static member functions
//
#include "JobControl/Template/Binder.cc"
template class Binder< RSSourceBinder >;
