// -*- C++ -*-
//
// Package:     DataHandler
// Module:      StreamPriorityGreater
// 
// Description: <one line class summary>
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Chris D. Jones
// Created:     Wed Jun 25 14:33:02 EDT 1997
// $Id: StreamPriorityGreater.cc,v 1.3 1997/09/18 21:10:18 cdj Exp $
//
// Revision history
//
// $Log: StreamPriorityGreater.cc,v $
// Revision 1.3  1997/09/18 21:10:18  cdj
// changed Boolean to DABoolean
//
// Revision 1.2  1997/07/09 17:59:13  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/06/29 20:24:07  cdj
// First sumbission
//

#include "Experiment/Experiment.h"
// system include files

// user include files
#include "DataHandler/StreamPriorityGreater.h"


// STL classes

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
//StreamPriorityGreater::StreamPriorityGreater()
//{
//}

// StreamPriorityGreater::StreamPriorityGreater( const StreamPriorityGreater& )
// {
// }

//StreamPriorityGreater::~StreamPriorityGreater()
//{
//}

//
// assignment operators
//
// const StreamPriorityGreater& StreamPriorityGreater::operator=( const StreamPriorityGreater& )
// {
// }

//
// member functions
//

//
// const member functions
//
DABoolean
StreamPriorityGreater::operator()( const Stream::Type& iLHS,
				const Stream::Type& iRHS ) const
{
   return ( iLHS.priority() > iRHS.priority() );
}

//
// static member functions
//
