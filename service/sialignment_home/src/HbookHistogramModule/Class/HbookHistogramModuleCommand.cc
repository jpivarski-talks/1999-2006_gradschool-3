// -*- C++ -*-
//
// Package:     HbookHistogramModule
// Module:      HbookHistogramModuleCommand
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Fri Nov 20 01:16:35 EST 1998
// $Id: HbookHistogramModuleCommand.cc,v 1.2 1999/09/10 02:41:07 mkl Exp $
//
// Revision history
//
// $Log: HbookHistogramModuleCommand.cc,v $
// Revision 1.2  1999/09/10 02:41:07  mkl
// location of Conversion class has moved
//
// Revision 1.1.1.1  1999/09/06 22:05:31  mkl
// imported HbookHistogramModule sources
//
// Revision 1.1.1.1  1998/11/20 07:08:27  mkl
// new HbookHistogramModule
//
//

#include "Experiment/Experiment.h"

// system include files
#include <assert.h>
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HbookHistogramModule/HbookHistogramModuleCommand.h"
#include "HbookHistogramModule/HbookHistogramModule.h"
#include "CommandPattern/Conversion.h"

// STL classes

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogramModule.HbookHistogramModuleCommand" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HbookHistogramModuleCommand::HbookHistogramModuleCommand( 
   const Command::Name& iName, 
   HbookHistogramModule* target )
   : HistogramModuleCommand( iName, target )
{
   addSubcommand( 
      new Subcommand< HbookHistogramModuleCommand >(
	 "file", 3, 3,
	 &HbookHistogramModuleCommand::fileHandler,
	 this )
      );
   addSubcommand( 
      new Subcommand< HbookHistogramModuleCommand >(
	 "recl", 3, 3,
	 &HbookHistogramModuleCommand::recordLengthHandler,
	 this )
      );
   addSubcommand( 
      new Subcommand< HbookHistogramModuleCommand >(
	 "lun", 3, 3,
	 &HbookHistogramModuleCommand::logicalUnitHandler,
	 this )
      );
}

// HbookHistogramModuleCommand::HbookHistogramModuleCommand( const HbookHistogramModuleCommand& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookHistogramModuleCommand::~HbookHistogramModuleCommand()
{
}

//
// assignment operators
//
// const HbookHistogramModuleCommand& HbookHistogramModuleCommand::operator=( const HbookHistogramModuleCommand& rhs )
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
int
HbookHistogramModuleCommand::helpHandler()
{
   // call base class's method
   HistogramModuleCommand::helpHandler();

   // print help from header
   static const string helpMessage =
      string( "    file <filename>   specify histogram file             \n" )+
      string( "                      (default='hist.rzn')               \n" )+
      string( "    recl <recl#>      specify record length              \n" )+
      string( "                      (default=1024)                     \n" )+
      string( "    lun <lun#>        specify logical unit number        \n" )+
      string( "                      (default=13)                       \n" )+
      string( "                                                         \n" )+
      string( " Please select file, recl, or lun before \"init\": e.g.  \n" )+
      string( "  Suez> hbook file /home/mkl/cleo3/myhist.rzn            \n" )+
      string( "  Suez> hbook recl 4096                                  \n" )+
      string( "  Suez> hbook init                                       \n" );

   report( INFO, kFacilityString )
      << "\n"
      << helpMessage
      << endl;
   
   return COMMAND_OK;
}

int
HbookHistogramModuleCommand::fileHandler()
{
   assert( m_argc==3 );
   string fileName( m_argv[2] );

   target()->setFileName( fileName );

   return COMMAND_OK;
}

int
HbookHistogramModuleCommand::recordLengthHandler()
{
   Command::Result returnValue = Command::COMMAND_OK;

   assert( m_argc==3 );

   DABoolean isNumber;
   int recordLength = Conversion::stringToDigit( m_argv[2], isNumber );
   if( true == isNumber )
   {
      target()->setRecordLength( recordLength );
    }
   else
   {
      report( ERROR, kFacilityString ) << "bad numerical value" << endl;
      returnValue = COMMAND_ERROR;
   }

   return returnValue;
}

int
HbookHistogramModuleCommand::logicalUnitHandler()
{
   Command::Result returnValue = Command::COMMAND_OK;

   assert( m_argc==3 );

   DABoolean isNumber;
   SmallCount logicalUnitNumber 
      = Conversion::stringToDigit( m_argv[2], isNumber );
   if( true == isNumber )
   {
      target()->setLogicalUnitNumber( logicalUnitNumber );
    }
   else
   {
      report( ERROR, kFacilityString ) << "bad numerical value" << endl;
      returnValue = COMMAND_ERROR;
   }

   return returnValue;
}

HbookHistogramModule*
HbookHistogramModuleCommand::target()
{
   return (HbookHistogramModule*)Command::target();
}

//
// const member functions
//
const HbookHistogramModule*
HbookHistogramModuleCommand::target() const
{
   return (const HbookHistogramModule*)Command::target();
}

//
// static member functions
//
