#if !defined(TRACKROOT_TRHELIXFIT_H)
#define TRACKROOT_TRHELIXFIT_H
// -*- C++ -*-
//
// Package:     TrackRoot
// Module:      TRHelixFit
// 
/**\class TRHelixFit TRHelixFit.h TrackRoot/TRHelixFit.h

 Description: Helix representation of a fit track.

 Usage:
    TRHelixFit is created by track fitters, and is a KTHelix with
    a track identifier added.

*/
//
// Author:      Michael Marsh
// Created:     Wed Jun 24 11:29:40 EDT 1998
// $Id: TRHelixFit.h,v 1.4 2002/04/04 21:26:51 cdj Exp $
//
// Revision history
//
// $Log: TRHelixFit.h,v $
// Revision 1.4  2002/04/04 21:26:51  cdj
// avoid creation of temporary KTHelix when reading back HelixFits
//
// Revision 1.3  2000/01/05 18:57:55  cdj
// have compiler write copy constructor
//
// Revision 1.2  1999/10/07 16:33:50  marsh
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
#include "KinematicTrajectory/KTHelix.h"
#include "CleoDB/DBCandidate.h" // for DBCandidate::Identifier

// forward declarations

class TRHelixFit : public KTHelix
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      typedef DBCandidate::Identifier  Identifier;
      
      // ---------- Constructors and destructor ----------------
      TRHelixFit( const Identifier& aId,
                  ValueType aCurvature,
                  ValueType aPhi0,
                  ValueType aD0,
                  ValueType aCotTheta,
                  ValueType aZ0,
                  const HepSymMatrix& aErrorMatrix,
                  const HepPoint3D& aReferencePoint ) :
         KTHelix( aCurvature,
                  aPhi0,
                  aD0,
                  aCotTheta,
                  aZ0,
                  aErrorMatrix,
                  aReferencePoint ),
         m_id(aId) {}
         
      TRHelixFit( const Identifier& aId, const KTHelix& aHelix );
      virtual ~TRHelixFit();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      /**
       * \return unique (distinct) track number.
       * This identifier is shared by all representations of a track.
       */
      const Identifier& identifier() const;
      
      // ---------- static member functions --------------------
      
   private:
      // ---------- Constructors and destructor ----------------
      TRHelixFit(); // stop default
//      TRHelixFit( const TRHelixFit& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const TRHelixFit& operator=( const TRHelixFit& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Identifier m_id;
      
      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* TRACKROOT_TRHELIXFIT_H */
