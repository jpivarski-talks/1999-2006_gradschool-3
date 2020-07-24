#if !defined(NAVIGATION_NAVMUONID_H)
#define NAVIGATION_NAVMUONID_H
// -*- C++ -*-
//
// Package:     Navigation
// Module:      NavMuonId
// 
/**\class NavMuonId NavMuonId.h Navigation/NavMuonId.h

 Description: Muon id info

 Usage:  through member functions

         Note that there are two pre-packaged muon id criteria, 1 and 2.

         simpleMuonId1() is better for analyses that are extremely
                         sensitive to fakes in the momentum range
                         1.5-2 GeV/c.
                         The efficiency in this momentum range
                         is slightly smaller (~20%) than
                         the other simple cut (below), but has
                         8-10 times lower fake rates in that region.

         simpleMuonId2() is better for analyses that are extremely
                         sensitive to having high eff in the momentum
                         range 1.5-2 GeV/c.
                         The efficiency in this momentum range
                         is slightly larger (~20%) than
                         the other simple cut (above), but has
                         8-10 times higher fake rates in that region.

         Both "simple" criteria require track momentum (pion hypothesis)
         exceeding 1 GeV/c, matched calorimeter energy between
         0.1 and 0.6 GeV, muon quality word muqual=0, and
         the depth of penetration into the muon system of
         3 interaction lengths at "low" momentum and 5 at "high"
         momentum. They differ only in the borderline between
         "low" and "high" momentum, which is 1.5 GeV/c for
         the first criterion and 2.0 GeV/c for the second.
         The user is referred to CBX 95-35, 96-41(Appendix A), 97-59.

	 Of course, the user is free to bypass these prepackaged
	 criteria but using the depth(), matchedEnergy(), and/or
         member functions of the associated MuTrack object.

	 Please note that the depth() function returns 0
         in two cases: there was no muon system response for
         this track, or there was a matching muon response
         but it had non-zero muqual. Experience shows muon
         id is only reliable if muqual=0 is required.

*/
//
// Author:      Brian K. Heltsley
// Created:     Thu Jun  1 08:59:07 EDT 2000
// $Id: NavMuonId.h,v 1.4 2002/08/06 17:09:20 cleo3 Exp $
//
// Revision history
//
// $Log: NavMuonId.h,v $
// Revision 1.4  2002/08/06 17:09:20  cleo3
// removed use of FAItem to decrease dependencies
//
// Revision 1.3  2001/09/07 18:04:02  cleo3
// removed forward declaration of ostream
//
// Revision 1.2  2000/08/11 00:21:09  bkh
// Add operator<< functionality to these classes
//
// Revision 1.1  2000/06/06 18:38:59  bkh
// Install dedx, electron id, and muon id interfaces
//

// system include files

// user include files

#include "FrameAccess/FAItem.h"

#include "C3cc/CcTypes.h"

// forward declarations
#include "C++Std/fwd_ostream.h"

class MuTrack ;
class NavTrack ;

class NavMuonId ;

ostream& operator<<( ostream& os, const NavMuonId& aMu ) ;

class NavMuonId
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      NavMuonId( const NavTrack& aNavTrack ,
		 const MuTrack*  aMuTrack    ) ;

      virtual ~NavMuonId() {}

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      DABoolean simpleMuonId1()   const ; // simple, reliable muon id
                                          // "one-stop shopping"
                                          // 0.1 < Ematched < 0.6 GeV &&
                                          // p>1.0 && muqual==0       &&
                                          // dpthmu>3(5) for p<1.5(>1.5)

      DABoolean simpleMuonId2()   const ; // simple, reliable muon id
                                          // "one-stop shopping"
                                          // 0.1 < Ematched < 0.6 GeV &&
                                          // p>1.0 && muqual==0       &&
                                          // dpthmu>3(5) for p<2(>2)

      Real      depth()           const ; // dpthmu if muqual==0; else 0

      CcGeV     matchedEnergy()   const ; // 0 if no match (from tk-shw match)

      FAItem< MuTrack > muTrack() const ; // access for the expert

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      // ---------- protected const member functions -----------
   private:
      // ---------- Constructors and destructor ----------------
      NavMuonId( const NavMuonId& ); // stop default
      // ---------- assignment operator(s) ---------------------
      const NavMuonId& operator=( const NavMuonId& ); // stop default
      // ---------- private member functions -------------------
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      DABoolean         m_simple1 ;
      DABoolean         m_simple2 ;
      Real              m_depth   ;
      CcGeV             m_energy  ;
      const MuTrack*    m_muTrack ;

      // ---------- static data members ------------------------
};

// inline function definitions

#endif /* NAVIGATION_NAVMUONID_H */
