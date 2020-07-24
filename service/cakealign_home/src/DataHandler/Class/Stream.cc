// -*- C++ -*-
//
// Package:     DataHandler
// Module:      Stream
// 
// Description: Class used to hold related typedefs and constants
//
// Implimentation:
//     This class can not be instantiated.
//
// Author:      Chris D. Jones
// Created:     Sun Mar 16 11:45:36 EST 1997
// $Id: Stream.cc,v 1.8 1997/11/19 15:16:52 sjp Exp $
//
// Revision history
//
// $Log: Stream.cc,v $
// Revision 1.8  1997/11/19 15:16:52  sjp
// Moved Experiment dependent info to Experiment dir
//
// Revision 1.7  1997/08/27 03:39:08  sjp
// Updated to new name for bug flags
//
// Revision 1.6  1997/07/09 17:59:10  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.5  1997/06/29 20:22:44  cdj
// 1) Removed Stream::kNoStops
// 2) Added priorities to Standard Streams
// 3) moved isStandard() to Stream::Type
//
// Revision 1.4  1997/05/07 16:57:45  sjp
// Cleaned up include file dependencies
//
// Revision 1.3  1997/04/29 19:52:10  cdj
// Changed static constant initialization to use constructor rather than
// operator=.  Necessary since Stream::Type is no long a string.
//
// Revision 1.2  1997/03/19 23:07:11  cdj
// Added isStandard method at the request of mkl
//
// Revision 1.1  1997/03/17 21:54:27  cdj
// First submission
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "DataHandler/Stream.h"

// STL classes
#if defined(OLD_CXX_STRING_CLASS_BUG)
#include <String.h>
#else
#include <string>
#endif /* OLD_CXX_STRING_CLASS_BUG */

//
// constants, enums and typedefs
//
const Stream::Type
Stream::kNone("none");

//The standard Streams
#include "Experiment/StreamConstants_defn.h"

//
// static data member definitions
//


//
// constructors and destructor
//
//Stream::Stream()
//{
//}

// Stream::Stream( const Stream& )
// {
// }

//Stream::~Stream()
//{
//}

//
// assignment operators
//
// const Stream& Stream::operator=( const Stream& )
// {
// }

//
// member functions
//

//
// const member functions
//

//
// static member functions
//

