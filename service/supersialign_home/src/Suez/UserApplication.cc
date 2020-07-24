// -*- C++ -*-
//
// Package:     Suez
// Module:      UserApplication
// 
// Description: creates all modules that are the basis of the user application.
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Martin Lohner
// Created:     Thu Mar 20 15:13:11 EST 1997
// $Id: UserApplication.cc,v 1.21 2000/03/14 20:55:21 mkl Exp $
//
// Revision history (at end of file)
//
#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <vector>
#include <map>
#include <set>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */
#include <assert.h>

// user include files
#include "Suez/UserApplication.h"

#include "JobControl/JobControl.h"
#include "Processor/MasterProcessor.h"
#include "Processor/MasterProducer.h"
#include "JobControl/SourceFormatMap.h"
#include "JobControl/SinkFormatMap.h"

#include "CommandPattern/FactoryTemplate.h"
#include "CommandPattern/Interpreter.h"

// proc/ducers to be statically linked
//#include "ExampleProc/ExampleProc.h"
//#include "ExampleProd/ExampleProd.h"

// source/sink formats to be statically linked
//#include "AsciiDelivery/AsciiSourceFormat.h"
//#include "DriverDelivery/DriverSourceFormat.h"
//#include "AsciiStorage/AsciiSinkFormat.h"

// modules to be statically linked
//#include "DBModule/DBModule.h"
//#include "CorbaModule/CorbaModule.h"

// STL classes
#include <vector>
#include <map>
#include <set>

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
UserApplication::UserApplication( JobControl* JOBC )
// --------------------------------------------------------
// This is a sample implementation; each user should 
// implement their own to build their very own application!
// -------------------------------------------------------
{
   assert( 0 != JOBC );

   // --------- register Processors with MasterProcessor -----------
   MasterProcessor& MPROC = JOBC->masterProcessor();
   //MPROC.add( "ExampleProc", new FactoryTemplate<ExampleProc> );

   // --------- register Producers with MasterProducer -------------
   MasterProducer& MPROD = JOBC->masterProducer();
   //MPROD.add( "ExampleProd", new FactoryTemplate<ExampleProd> );

   // -------- register Formats with Source/SinkFormatMaps ---------

   SourceFormatMap& SoFM = JOBC->sourceFormats();
   //SoFM.add( "AsciiSourceFormat",  new FactoryTemplate< AsciiSourceFormat > );
   //SoFM.add( "DriverSourceFormat", new FactoryTemplate< DriverSourceFormat > );

   //SinkFormatMap& SiFM = JOBC->sinkFormats();
   //SiFM.add( "AsciiSinkFormat", new FactoryTemplate< AsciiSinkFormat > );

   // -------- register Modules with JobControl ---------
   //(*JOBC).add( "DBModule",    new FactoryTemplate< DBModule > );
   //(*JOBC).add( "CorbaModule", new FactoryTemplate< CorbaModule > );

}

// UserApplication::UserApplication( const UserApplication& )
// {
// }

UserApplication::~UserApplication()
{
}

//
// assignment operators
//
// const UserApplication& UserApplication::operator=( const UserApplication& )
// {
// }

//
// member functions
//
DABoolean
UserApplication::runEmbeddedCommands( Interpreter& interpreter )
{
   DABoolean returnValue = true;

   // sample embedded commands
   //interpreter.runCommand( "echo hello" );
   //interpreter.runCommand( "help" );

   return returnValue;
}

//
// const member functions
//
DABoolean
UserApplication::interactiveMode() const
{
   // interactive or not? 
   // If not, you will never see a prompt, only embedded commands running by
   return true;
}

//
// static member functions
//


// ----------------------------------------------------------------------
// Revision history
//
// $Log: UserApplication.cc,v $
// Revision 1.21  2000/03/14 20:55:21  mkl
// new UserApplication functionality: new embeddedCommand and interactiveMode methods for running Suez in standalone mode in online
//
// Revision 1.20  1999/10/29 15:24:52  mkl
// print proper error message when trying to invoke purify or quantify on anything other than Solaris
//
// Revision 1.19  1998/11/30 19:51:12  mkl
// properly format SuezMain
//
// Revision 1.18  1998/11/10 23:20:08  mkl
// static linking of proc/ducers/source-/sink-formats/modules whatever; and cleanup
//
// Revision 1.17  1998/09/25 23:56:46  cdj
// no longer use ProxyDict
//
// Revision 1.16  1998/07/28 15:36:08  mkl
// include stl headers properly
//
// Revision 1.15  1998/07/28 14:49:49  mkl
// commented out (mistakenly) uncommented static linking of sources/sinks
//
// Revision 1.14  1998/07/27 23:49:39  mkl
// use new shortened names for source/sink factories (make g++ happy)
//
// Revision 1.13  1998/07/25 16:14:30  mkl
// add static linking of sources/sinks
//
// Revision 1.12  1998/04/20 19:26:07  mkl
// fix static linking of producers
//
// Revision 1.11  1998/01/16 23:35:34  mkl
// added ProxyExample in linking list for hardwired debugging Producers for now
//
// Revision 1.10  1997/09/10 01:53:31  mkl
// minor
//
// Revision 1.9  1997/09/05 22:34:46  mkl
// undid singleton patteron for MasterProcessor
//
// Revision 1.8  1997/09/04 06:30:27  mkl
// playing with more than one statically linked processor
//
// Revision 1.7  1997/09/03 02:45:47  mkl
// restructuring suez libraries
//
// Revision 1.6  1997/08/29 04:20:11  mkl
// bug flag name change
//
// Revision 1.5  1997/08/13 07:10:06  mkl
// Chains of Sources, new activation of streams, and ProxyDict
//
// Revision 1.4  1997/07/22 05:20:38  mkl
// use factory method for statically linked processors
//
// Revision 1.3  1997/07/11 06:53:52  mkl
// // New <Package>/<File>.h structure
// // Modified so can also compile under cxx v5.5
//
// Revision 1.2  1997/05/29 19:38:44  mkl
// fixes of mkl-specific stuff
//
// Revision 1.1.1.1  1997/05/26 19:52:34  mkl
// imported main suez
//
// Revision 1.5  1997/05/23 20:22:39  mkl
// separated Processor stuff from JobControl
//
// Revision 1.4  1997/05/15 17:42:25  mkl
// MasterProcessor now contains a SharedObjectHandler. Added "processor
// reorder" command.
//
// Revision 1.3  1997/05/14 17:15:26  mkl
// added capability to load/unload shared objects.
//
// Revision 1.2  1997/04/01 22:30:13  mkl
// more comments; added DstarAnalysis template
//
// Revision 1.1  1997/03/27 06:54:31  mkl
// imported sources.
//
