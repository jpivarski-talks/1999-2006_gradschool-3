// -*- C++ -*-
//
// Package:     <HbookHistogramModule>
// Module:      HbookHistogramModule
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Martin Lohner
// Created:     Fri Nov 20 01:16:34 EST 1998
// $Id: HbookHistogramModule.cc,v 1.5 2002/01/25 15:34:10 cdj Exp $
//
// Revision history
//
// $Log: HbookHistogramModule.cc,v $
// Revision 1.5  2002/01/25 15:34:10  cdj
// ntuple command can now specify full DataKey as well as buffer size
//
// Revision 1.3  2001/12/20 19:12:17  ajm36
// add cdj's FAXXXX->ntuple facility
//
// Revision 1.2  2001/09/07 19:09:12  cleo3
// fixed problems found with Solaris 8
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
#if defined(OSTRSTREAM_NO_TERMINATION_BUG)
#include <stdio.h>
#endif
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "HbookHistogramModule/HbookHistogramModule.h"
#include "HbookHistogram/HbookManager.h"
#include "JobControl/JobControl.h"
#include "JobControl/SinkManager.h"

#include "NTupleStorage/NTSStorageHelperTester.h"
#include "NTupleStorage/NTupleSinkBinder.h"

#include "StorageManagement/SMStorageHelperManager.h"

// STL classes
#include <string>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "HbookHistogramModule.HbookHistogramModule" ;

//
// static data member definitions
//

//
// constructors and destructor
//
HbookHistogramModule::HbookHistogramModule()
   : HistogramModule( "HbookHistogramModule", "Histogram Module for hbook" ),
     m_command( "hbook", this ),
     m_fileName( "hist.rzn" ),
     m_logicalUnitNumber( 13 ), // reasonable value
     m_recordLength( 1024 )     // reasonable value
{
}

// HbookHistogramModule::HbookHistogramModule( const HbookHistogramModule& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

HbookHistogramModule::~HbookHistogramModule()
{
   closeHistogramManager();
}

//
// assignment operators
//
// const HbookHistogramModule& HbookHistogramModule::operator=( const HbookHistogramModule& rhs )
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
HIHistoManager*
HbookHistogramModule::newHistogramManager() 
{
   return new HbookManager( m_fileName.c_str(), 
			    m_logicalUnitNumber,
			    m_recordLength );
}

void
HbookHistogramModule::closeHistogramManager() 
{
}

void
HbookHistogramModule::setFileName( const string& iFileName ) 
{
   m_fileName = iFileName;
}

void
HbookHistogramModule::setLogicalUnitNumber( SmallCount iLogicalUnitNumber ) 
{
   m_logicalUnitNumber = iLogicalUnitNumber;
}

void
HbookHistogramModule::setRecordLength( SmallCount iRecordLength ) 
{
   m_recordLength = iRecordLength;
}

#if !defined(NO_COVARIANT_RETURN_TYPE_BUG)
HbookManager*
#else
HIHistoManager*
#endif
HbookHistogramModule::manager()
{ 
   return static_cast< HbookManager* >( HistogramModule::manager() ); 
}

//
// const member functions
//
#if !defined(NO_COVARIANT_RETURN_TYPE_BUG)
const HbookManager*
#else
const HIHistoManager*
#endif
HbookHistogramModule::manager() const
{ 
   return static_cast< const HbookManager* >( HistogramModule::manager() ); 
}

string
HbookHistogramModule::statusInfo() const
{
   string returnValue( "\nHbook Status:\n" );
   returnValue += string( "Filename    : " ) 
      + m_fileName + string( "\n" );

// buggy ostrstream implementation and no stringstream in sight 
#if defined(OSTRSTREAM_NO_TERMINATION_BUG)
   const unsigned int kSize=100;
   char logicalUnitNumber[kSize];
   snprintf( logicalUnitNumber, kSize, "%d", m_logicalUnitNumber );
   returnValue += string( "LogicalUnit#: " ) 
      + string( logicalUnitNumber ) + string( "\n" );
#else
   //ostringstream number; // not yet available: <sstream>
   ostrstream logicalUnitNumber;
   logicalUnitNumber << m_logicalUnitNumber;
   returnValue += string( "LogicalUnit#: " ) 
      + logicalUnitNumber.str() + string( "\n" );
#endif

// buggy ostrstream implementation and no stringstream in sight 
#if defined(OSTRSTREAM_NO_TERMINATION_BUG)
   const unsigned int kSize2=100;
   char recordLength[kSize2];
   snprintf( recordLength, kSize2, "%d", m_recordLength );
   returnValue += string( "RecordLength: " ) 
      + string( recordLength ) + string( "\n" );
#else
   //ostringstream number; // not yet available: <sstream>
   ostrstream recordLength;
   recordLength << m_recordLength;
   returnValue += string( "RecordLength: " ) 
      + recordLength.str() + string( "\n" );
#endif

   return returnValue;
}

DABoolean
HbookHistogramModule::createNtupleSink(
   const string& iFileName,
   SmallCount iLogicalUnitNumber,
   SmallCount iRecordLength,
   unsigned long iBufferSize,
   const STL_VECTOR(string) & iKeyNames,
   const StreamSet& iStreams )
{
   //set up default values if necessary
   SmallCount logicalUnitNumber = iLogicalUnitNumber;
   if( logicalUnitNumber == 0 ) {
      logicalUnitNumber = m_logicalUnitNumber+1;
   }

   SmallCount recordLength = iRecordLength;
   if( recordLength == 0 ) {
      recordLength = m_recordLength;
   }
   unsigned long bufferSize = iBufferSize;
   if( bufferSize == 0 ) {
      bufferSize == recordLength;
   }

   StreamSet streams = iStreams;

   if( streams.size() == 0 ) {
      streams.add( Stream::kEvent ) ;
   }

   //Find the StorageHelper for each type name and check to see if it can
   //  be stored in an Ntuple
   NTSStorageHelperTester tester;

   STL_VECTOR(DurableDataKey) dataKeys;
   dataKeys.reserve( iKeyNames.size() );

   SMStorageHelperManager& shm = SMStorageHelperManager::instance();

   SMStorageHelperManager::iterator itSHEnd = shm.end();

   for( STL_VECTOR(string)::const_iterator itKeyName = iKeyNames.begin();
        itKeyName != iKeyNames.end();
        ++itKeyName ) {
       const char* const kDelimiter = " \"";

      //look for corresponding type
      string noInitialSpace( itKeyName->substr( itKeyName->find_first_not_of(kDelimiter) ) );
      string typeName( noInitialSpace.substr(0, noInitialSpace.find_first_of(kDelimiter)) );
      string usage;
      string production;
      if( typeName.size() != noInitialSpace.size() ) {
         noInitialSpace = noInitialSpace.substr(typeName.size());
         noInitialSpace = noInitialSpace.substr(noInitialSpace.find_first_not_of(kDelimiter) );
         usage = noInitialSpace.substr(0, noInitialSpace.find_first_of(kDelimiter));
         if( usage.size() != noInitialSpace.size() ) {
            noInitialSpace = noInitialSpace.substr(typeName.size());
            noInitialSpace = noInitialSpace.substr(noInitialSpace.find_first_not_of(kDelimiter) );
            production = noInitialSpace.substr(0, noInitialSpace.find_first_of(kDelimiter));
         }
      }
      DABoolean foundType = false;
      for( SMStorageHelperManager::iterator itHelper = shm.begin();
           itSHEnd != itHelper;
           ++itHelper ) {
         if( typeName == (*itHelper)->typeTag().name() ) {
            foundType = true;
            if( ! tester.test(*itHelper) ) {
               dataKeys.push_back( DurableDataKey( DataKey((*itHelper)->typeTag(), usage.c_str(), production.c_str())) );
               break;
            } else {
               report(ERROR, kFacilityString ) 
                  <<"can not store "<<(*itKeyName) << "because \n"
                  <<tester.errorMessage()<<endl;
               return false;
            }
         }
      }
      if ( ! foundType ) {
         report(ERROR, kFacilityString ) << "unable to find type " 
                                         << (*itKeyName ) << endl;
         return false;
      }
   }

   //Create the HbookManager
   HbookManager* manager = new HbookManager( iFileName.c_str(),
                                             logicalUnitNumber,
                                             recordLength );

   //now create the sink
   NTupleSinkBinder* binder = new NTupleSinkBinder( iFileName,
                                                    manager,
                                                    dataKeys,
                                                    bufferSize,
                                                    streams);
   
   if( ! JobControl::instance()->sinkManager().addSink( binder ) ) {
      report(ERROR, kFacilityString ) 
         <<"you probably used the same file name as another sink" << endl;
      return false;
   }

   return true;
}

void
HbookHistogramModule::listUsableStorageHelpers() 
{
   //Loop through all ProxyFactories and determine which ones can
   //  be used for input/output
   
   SMStorageHelperManager& shm = SMStorageHelperManager::instance();

   NTSStorageHelperTester tester;

   ostream& reportStream = report(INFO, kFacilityString );
   reportStream << "\n";
   SMStorageHelperManager::factory_iterator itFactoryEnd = shm.factory_end();
   for( SMStorageHelperManager::factory_iterator itFactory = 
           shm.factory_begin();
        itFactory != itFactoryEnd;
        ++itFactory ) {
      SMStorageHelperManager::iterator itHelper = shm.find( 
         (*itFactory)->proxyTypeTag() );

      if( ! tester.test( *itHelper ) ) {
         reportStream << (*itFactory)->proxyTypeTag().name() <<"\n";
      }
   }
   reportStream << endl;
}

//
// static member functions
//
