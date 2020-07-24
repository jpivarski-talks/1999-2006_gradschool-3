#if !defined(DATAHANDLER_RECORDMETHODS_CC)
#define DATAHANDLER_RECORDMETHODS_CC
// -*- C++ -*-
//
// Package:     DataHandler
// Module:      RecordMethods
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Chris D. Jones
// Created:     Thu Sep 24 10:30:20 EDT 1998
// $Id: RecordMethods.cc,v 1.1 1998/09/25 19:32:04 cdj Exp $
//
// Revision history
//
// $Log: RecordMethods.cc,v $
// Revision 1.1  1998/09/25 19:32:04  cdj
// no longer use ProxyDict
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "DataHandler/RecordMethods.h"
#include "DataHandler/DataKey.h"
#include "DataHandler/DataKeyTags.h"
#include "DataHandler/ProxyTemplate.h"
#include "DataHandler/Record.h"
#include "HeteroContainer/HCMethods.h"

// STL classes

//
// constants, enums and typedefs
//

//useful macro
#define RECORD_METHODS HCMethods< T, ProxyTemplate< T >, Record, DataKey, IdTags >


//static const char* const kFacilityString = "DataHandler.RecordMethods" ;

//
// static member functions
//
template< class T >
DataKey
RecordMethods< T >::makeKey( const UsageTag& iUsage, 
			     const ProductionTag& iProduction )
{
   return RECORD_METHODS::makeKey( IdTags( iUsage, iProduction ) );
}


template< class T >
ProxyTemplate< T >*
RecordMethods< T >::extractProxy( const Record& iRecord,
				  const UsageTag& iUsage,
				  const ProductionTag& iProduction )
{
   return RECORD_METHODS::find( iRecord, 
				IdTags( iUsage, iProduction ) );
}

template< class T >
DABoolean
RecordMethods< T >::insertProxy( Record& iRecord,
				  ProxyTemplate< T >* iProxy,
				  const UsageTag& iUsage,
				  const ProductionTag& iProduction )
{
   return RECORD_METHODS::insert( iRecord, iProxy,
				IdTags( iUsage, iProduction ) );
}

#endif /* DATAHANDLER_RECORDMETHODS_CC */
