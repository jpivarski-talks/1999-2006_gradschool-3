#if !defined(TRACKROOT_TRHELIXCANDIDATEFIT_H)
#define TRACKROOT_TRHELIXCANDIDATEFIT_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixCandidateFit
//
/**\class TRHelixCandidateFit TRHelixCandidateFit.h TrackRoot/TRHelixCandidateFit.h

 Description: Templated wrapper for TRHelixFit to distinguish hypotheses.

 Usage:
    This IS-A TRHelixFit, which in turn IS-A KTHelix.  It adds nothing
    to the interface, just type-separation for proxies.  Users should
    never see this class, since the base classes are all that is of
    interest.

*/
//
// Author:      Michael Marsh
// Created:     Wed Jun 24 11:29:36 EDT 1998
// $Id: TRHelixCandidateFit.h,v 1.3 2000/01/05 18:58:34 cdj Exp $
//
// Revision history
//
// $Log: TRHelixCandidateFit.h,v $
// Revision 1.3  2000/01/05 18:58:34  cdj
// have compiler write copy constructor
//
// Revision 1.2  1999/10/07 16:33:49  marsh
// Added doxygen-style comments.
//
// Revision 1.1.1.1  1998/07/20 21:16:11  marsh
// New library for base-level track objects
//
// Revision 1.1.1.1  1998/07/13 23:28:03  marsh
// New library for track objects
//

// system include files

// user include files
#include "TrackRoot/TRHelixFit.h"
#include "CleoDB/DBCandidate.h" // for Hypo

#if defined(CANNOT_USE_ENUMS_AS_TEMPLATE_ARGS_BUG)
template < int H >
#else
template < DBCandidate::Hypo H >
#endif
class TRHelixCandidateFit : public TRHelixFit
{
   public:
      // ---------- Constructors and destructor ----------------
#if defined(TYPEDEFS_IN_TEMPLATES_IN_ARGS_BUG)
      TRHelixCandidateFit( const Identifier& aId ,
                           const KTHelix& aHelix ) :
      TRHelixFit( aId, aHelix )
{
}
#else
      TRHelixCandidateFit( const Identifier& aId ,
                           const KTHelix& aHelix );
#endif /* TYPEDEFS_IN_TEMPLATE_IN_ARGS_BUG */

      virtual ~TRHelixCandidateFit();

      // static member function
      static DBCandidate::Hypo hypo() ;

   private:
      // ---------- Constructors and destructor ----------------
      TRHelixCandidateFit(); // stop default
//      TRHelixCandidateFit( const TRHelixCandidateFit& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRHelixCandidateFit& operator=(
         const TRHelixCandidateFit& ); // stop default
};

// inline function definitions

#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
# include "TrackRoot/Template/TRHelixCandidateFit.cc"
#endif

#endif /* TRACKROOT_TRHELIXCANDIDATEFIT_H */
