//
//  File:	T_HbookHook.cc
//  Author: 	Simon Patton
//  Contents:	Build all the Hook classes that required for the
//		  Hbook Classes
//
// $Id: T_HbookHook.cc,v 1.2 1999/09/07 21:20:17 mkl Exp $
//

#include "Utility/Template/SingleHook.cc"
#include "Utility/Template/MultipleHook.cc"


#include "HbookHistogram/HbookRZItem.h"

#if defined(__DECCXX)
// for AXP
#  pragma define_template SingleHook<HbookRZItem>
#  pragma define_template MultipleHook<HbookRZItem>
#elif defined(__SymantecCxx__)
// for Macintosh
#  pragma template SingleHook<HbookRZItem>
#  pragma template MultipleHook<HbookRZItem>
#  pragma template_access public
#else
template class SingleHook<HbookRZItem> ;
template class MultipleHook<HbookRZItem> ;
#endif

#include "HbookHistogram/HbookFile.h"

#if defined(__DECCXX)
// for AXP
#  pragma define_template SingleHook<HbookFile>
#elif defined(__SymantecCxx__)
// for Macintosh
#  pragma template SingleHook<HbookFile>
#  pragma template_access public
#else
template class SingleHook<HbookFile> ;
#endif


//----------------------------------------------------------------------
//
// $Log: T_HbookHook.cc,v $
// Revision 1.2  1999/09/07 21:20:17  mkl
// make cxx happy
//
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.7  1998/07/29 02:25:05  mkl
// handle solaris as well (no open if before)
//
// Revision 1.6  1997/09/07 17:54:04  sjp
// Reworked to run with "Experiment" environment
//
// Revision 1.5  1997/07/09 17:49:22  sjp
// Removed obselete include file
//
// Revision 1.4  1997/07/03 20:17:47  sjp
// New <Package>/<File>.h structure
//
// Revision 1.3  1996/06/21 20:55:21  sjp
// Added commands to instantiate `cxx' STL
//
// Revision 1.2  1996/04/11 15:37:16  sjp
// Added necessary instantiations
//
// Revision 1.1  1995/07/13 15:00:05  sjp
// Routine moved here from Class/Utility driectroy as this seems to be
//   its more natural home.
//
