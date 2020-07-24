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
// $Id: HbookHistogramModuleCommand.cc,v 1.4 2002/01/25 15:34:10 cdj Exp $
//
// Revision history
//
// $Log: HbookHistogramModuleCommand.cc,v $
// Revision 1.4  2002/01/25 15:34:10  cdj
// ntuple command can now specify full DataKey as well as buffer size
//
// Revision 1.3  2001/12/20 19:12:17  ajm36
// add cdj's FAXXXX->ntuple facility
//
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

#include "DataHandler/StreamSet.h"

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
   addSubcommand( 
      new Subcommand< HbookHistogramModuleCommand >(
	 "ntuple", 3, 100,
	 &HbookHistogramModuleCommand::ntupleHandler,
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
      string( "    ntuple help       print all commands available to    \n" )+
      string( "                      create ntuple sources/sinks        \n" )+
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


static const unsigned int kNtupleSubCommandIndex = 3;

int
HbookHistogramModuleCommand::ntupleHandler()
{
   Command::Result returnValue = Command::COMMAND_OK;

   string commandName( m_argv[2] );
   if( commandName == "ls" ) {
      return ntupleListHandler();
   } else if( commandName == "help") {
      report( INFO, kFacilityString ) 
	 << "\n"
	 << "hbook ntuple SubCommands \n"
	 << "  help \n"
	 << "      print out this message\n"
	 << "  ls \n"
	 << "      list all classes that can be used for ntuple \n"
	 << "  out <filename> <key1> [<key2> ...] [lun <lun#>] [recl <recl#>]  [buffer_size <size>]\n"
	 << "    [ streams <stream1> [<stream2> ...] ]\n "
	 << "      create a file containing one ntuple for each <key>. If no\n"
	 << "      streams specified, will use 'event'.\n" 
	 << "      A <key> is defined as <type> [<usage tag>] [<production tag>].\n"
	 << "    examples:\n"
	 << "        hbook ntuple out test.hbook FATable<TRSeedTrack>\n"
	 << "        hbook ntuple out test.hbook {FATable<TRSeedTrack> good} recl 32000\n"<< endl;

   } else if( commandName == "out") {
      return ntupleOutHandler();
   } else {
      report( ERROR, kFacilityString ) <<" unknown ntuple subcommand "
				       << commandName << endl;
      returnValue = COMMAND_ERROR;
   }
   return returnValue;
}

int
HbookHistogramModuleCommand::ntupleListHandler()
{
   Command::Result returnValue = Command::COMMAND_OK;

   target()->listUsableStorageHelpers();

   return returnValue;
}

DABoolean
HbookHistogramModuleCommand::setKeyWordArg(const string& iKeyWord, 
                                           unsigned int iIndex, 
                                           unsigned long& oArg) const
{
   if( iIndex >= m_argc ) {
      setResult( iKeyWord +" needs an argument" );
      return false;
   }
   DABoolean isOK = true;
   oArg = Conversion::stringToDigit(m_argv[iIndex], isOK);
   if (!isOK ) {
      setResult( string("non numeric argument given for ")+iKeyWord+ ":"+m_argv[iIndex]);
   }
   return isOK;
}

int
HbookHistogramModuleCommand::ntupleOutHandler()
{
   Command::Result returnValue = Command::COMMAND_OK;

   if( m_argc < kNtupleSubCommandIndex + 2 ) {
      setResult(" too few arguments for 'hbook ntuple out' Command");
      return COMMAND_ERROR;
   } 

   string fileName( m_argv[kNtupleSubCommandIndex] );
   unsigned long logicalUnitNumber = 0;
   unsigned long recordLength = 0;
   unsigned long bufferSize = 0;

   //try to find all the types
   STL_VECTOR( string ) typeNameList;
   typeNameList.reserve( m_argc - kNtupleSubCommandIndex );

   static const string lunKeyWord( "lun");
   static const string reclKeyWord( "recl");
   static const string streamsKeyWord( "streams");
   static const string bufferSizeKeyWord( "buffer_size");
   
   unsigned int index = kNtupleSubCommandIndex + 1;
   for(; index < m_argc; ++index ) {
      string input = m_argv[ index ] ;

      if( ( input == lunKeyWord ) ||
          ( input == reclKeyWord ) ||
          ( input == streamsKeyWord ) ||
          ( input == bufferSizeKeyWord ) ) {
         break;
      }
      typeNameList.push_back( input ) ;
   }

   for(; index < m_argc; ++index ) {
      string input = m_argv[ index ] ;
      if( input == lunKeyWord ) {
         if(! setKeyWordArg(lunKeyWord,++index,logicalUnitNumber) ) {
            return COMMAND_ERROR;
         }
      } else if(input == reclKeyWord) {
         if(! setKeyWordArg(reclKeyWord,++index,recordLength) ) {
            return COMMAND_ERROR;
         }
      } else if( input == bufferSizeKeyWord ) {
         if(! setKeyWordArg(bufferSizeKeyWord,++index,bufferSize) ) {
            return COMMAND_ERROR;
         }         
      } else if(input == streamsKeyWord) {
         setResult(streamsKeyWord+" not yet supported");
         return COMMAND_ERROR;
      }
   }
   StreamSet streams;
   if ( index != m_argc ) {


   }

   if( ! target()->createNtupleSink( fileName,
				     logicalUnitNumber,
				     recordLength,
				     bufferSize,
				     typeNameList,
				     streams ) ) {
      returnValue = COMMAND_ERROR;
   }
   
   return returnValue ;
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
