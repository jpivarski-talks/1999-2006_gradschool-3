// -*- C++ -*-
//
// Package:     DataHandler
// Module:      StreamPriorityLess
// 
// Description: <one line class summary>
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Chris D. Jones
// Created:     Wed Jun 25 14:33:02 EDT 1997
// $Id: StreamPriorityLess.cc,v 1.3 1997/09/18 21:10:19 cdj Exp $
//
// Revision history
//
// $Log: StreamPriorityLess.cc,v $
// Revision 1.3  1997/09/18 21:10:19  cdj
// changed Boolean to DABoolean
//
// Revision 1.2  1997/07/09 17:59:14  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.1  1997/06/29 20:24:08  cdj
// First sumbission
//

#include "Experiment/Experiment.h"
// system include files

// user include files
#include "DataHandler/StreamPriorityLess.h"


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
//StreamPriorityLess::StreamPriorityLess()
//{
//}

// StreamPriorityLess::StreamPriorityLess( const StreamPriorityLess& )
// {
// }

//StreamPriorityLess::~StreamPriorityLess()
//{
//}

//
// assignment operators
//
// const StreamPriorityLess& StreamPriorityLess::operator=( const StreamPriorityLess& )
// {
// }

//
// member functions
//

//
// const member functions
//
DABoolean
StreamPriorityLess::operator()( const Stream::Type& iLHS,
				const Stream::Type& iRHS ) const
{
   return ( iLHS.priority() < iRHS.priority() );
}

//
// static member functions
//
