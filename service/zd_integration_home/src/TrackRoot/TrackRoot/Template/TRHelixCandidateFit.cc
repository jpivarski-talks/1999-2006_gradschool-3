#if !defined(TRACKROOT_TRHELIXCANDIDATEFIT_CC)
#define TRACKROOT_TRHELIXCANDIDATEFIT_CC
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixCandidateFit
// 
// Description: Templated wrapper for TRHelixFit to distinguish hypotheses
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Michael Marsh
// Created:     Wed Jun 24 12:08:45 EDT 1998
// $Id: TRHelixCandidateFit.cc,v 1.2 2000/01/05 18:59:40 cdj Exp $
//
// Revision history
//
// $Log: TRHelixCandidateFit.cc,v $
// Revision 1.2  2000/01/05 18:59:40  cdj
// have compiler write copy constructor
//
// Revision 1.1.1.1  1998/07/20 21:16:11  marsh
// New library for base-level track objects
//
// Revision 1.2  1998/07/18 21:05:40  cdj
// Fixed cpp logic bug
//
// Revision 1.1.1.1  1998/07/13 23:28:05  marsh
// New library for track objects
//

#include "Experiment/Experiment.h"

// system include files
//#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
//#include <string>
//#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
#include "TrackRoot/TRHelixCandidateFit.h"

// STL classes
//#if defined(OLD_CXX_STRING_CLASS_BUG)
//#include "C++Std/fwd_string.h"
//#include <String.h>
//#else
//#include <string>
//#endif /* OLD_CXX_STRING_CLASS_BUG */

//
// constants, enums and typedefs
//

//
// constructors and destructor
//
#if !defined(TYPEDEFS_IN_TEMPLATES_IN_ARGS_BUG)
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
TRHelixCandidateFit< H >::TRHelixCandidateFit( const Identifier& aId ,
                                               const KTHelix& aHelix ) :
   TRHelixFit( aId, aHelix )
{
}

#endif /* TYPEDEFS_IN_TEMPLATES_IN_ARGS_BUG */

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
TRHelixCandidateFit< H >::~TRHelixCandidateFit()
{
}

// #if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
//template < int H >
//#else
//template < DBCandidate::Hypo H >
//#endif
//TRHelixCandidateFit<H>::TRHelixCandidateFit(const TRHelixCandidateFit<H> &rhs)
//{
//  aId = rhs.aId;
//  aHelix = rhs.aHelix
//}

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
DBCandidate::Hypo
TRHelixCandidateFit< H >::hypo()
{
#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
   return ( DBCandidate::Hypo(H) ) ;
#else
   return ( H ) ;
#endif
}

#endif /* TRACKROOT_TRHELIXCANDIDATEFIT_CC */
