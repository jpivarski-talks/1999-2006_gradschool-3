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
// $Id: HbookHistogramModule.cc,v 1.2 2001/09/07 19:09:12 cleo3 Exp $
//
// Revision history
//
// $Log: HbookHistogramModule.cc,v $
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

//
// static member functions
//
