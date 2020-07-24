// -*- C++ -*-
//
// Package:     DataDelivery
// Module:      RecordProvider
// 
// Description: Factory function to produce Record  or subclasses
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Simon Patton
// Created:     Thu Oct 30 16:41:02 EST 1997
// $Id: RecordProvider_create.cc,v 1.1 1997/11/19 15:23:43 sjp Exp $
//
// Revision history
//
// $Log: RecordProvider_create.cc,v $
// Revision 1.1  1997/11/19 15:23:43  sjp
// Replaceable routine to create Records
//

#include "Experiment/Experiment.h"

// system include files
#include <stdlib.h>  // For 'exit'
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "Experiment/report.h"
#include "DataHandler/Record.h"
#include "DataHandler/RecordProvider.h"

// STL classes

//
// constants, enums and typedefs
//

const string kFacilityString = "DataDelivery.RecordProvider" ;

//
// static member functions
//

Record*
RecordProvider::createRecord( const Stream::Type& aStream )
{
   if ( Stream::kNone == aStream ) {
      report( ERROR ,
	      kFacilityString )
		 << "Record is not associated with a Stream."
		 << endl ;
      exit( 1 ) ;
   }
   return ( new Record( aStream ) ) ;
}

