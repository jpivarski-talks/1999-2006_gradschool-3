// -*- C++ -*-
//
// Package:     DataHandler
// Module:      StreamType
// 
// Description: Instantiate TBIdentifer< string , Stream >
//
// Implimentation:
//     <Notes on implimentation>
//
// Author:      Simon Patton
// Created:     Sat May 25 14:32:44 EDT 1996
// $Id: T_UTIdStringStream.cc,v 1.6 1998/06/24 17:15:24 cdj Exp $
//
// Revision history
//
// $Log: T_UTIdStringStream.cc,v $
// Revision 1.6  1998/06/24 17:15:24  cdj
// added new bug flag needed by cxx 6.0
//
// Revision 1.5  1997/09/01 16:23:13  sjp
// Moved TBIdentifier to UTIdentifier
//
// Revision 1.4  1997/08/27 03:39:20  sjp
// Updated to new name for bug flags
//
// Revision 1.3  1997/07/09 18:00:19  sjp
// New <Package>/<File>.h structure
// Modified so can also compile under cxx v5.5
//
// Revision 1.2  1997/05/07 20:10:40  sjp
// Instantiated input and output operators
//
// Revision 1.1  1997/05/07 16:58:29  sjp
// Instantiate TBIdentifier for Stream::Type
//

#include "Experiment/Experiment.h"

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
#include <string>
#include <vector>
#include <map>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */


// user include files
#include "DataHandler/Stream.h"
#if defined(OLD_CXX_STRING_CLASS_BUG)
#include <String.h>
#else
#include <string>
#endif /* OLD_CXX_STRING_CLASS_BUG */

#include "Utility/Template/UTIdentifier.cc"


#if ( defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX) )
// for AXP
#  pragma define_template UTIdentifier< string, Stream >
#  pragma define_template operator<<< string, Stream >
#  pragma define_template operator>>< string, Stream >
#else
template class UTIdentifier< string , Stream > ;

#if defined( NEED_NAMESPACE_TO_INSTANTIATE_OVERLOADED_FUNCTION_BUG )
template ostream& ::operator<<( ostream& iOStream , 
                const UTIdentifier< string , Stream >& iIdentifier ) ;
template istream& ::operator>>( istream& iIstream , 
                UTIdentifier< string , Stream >& iIdentifier ) ;
#else
template ostream& operator<<( ostream& iOStream , 
                const UTIdentifier< string , Stream >& iIdentifier ) ;
template istream& operator>>( istream& iIstream , 
                UTIdentifier< string , Stream >& iIdentifier ) ;
#endif /* NEED_NAMESPACE_TO_INSTANTIATE_OVERLOADED_FUNCTION_BUG */
#endif /* (defined(PRAGMA_TEMPLATE_INSTANTIATION_BUG) && defined(__DECCXX)) */
