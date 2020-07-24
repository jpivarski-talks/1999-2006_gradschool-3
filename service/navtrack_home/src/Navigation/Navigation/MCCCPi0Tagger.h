#if !defined(NAVIGATION_MCCCPI0TAGGER_H)
#define NAVIGATION_MCCCPI0TAGGER_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      MCCCPi0Tagger
// 
/**\class MCCCPi0Tagger MCCCPi0Tagger.h Navigation/MCCCPi0Tagger.h

 Description: A Pi0 Tagger

 Usage:
 
    Determine the Monte Carlo Pi0 that matches a reconstructed Pi0 or
    vice-versa. 
    
    The algorithm is to determine the photons that belong to the
    reconstructed Pi0 then match them to MC photons. These MC photons
    are then checked to see if they come from the same MC Pi0. If they
    do, then a pointer to the MC Pi0 object is returned (swap
    reconstructed for MC to tag the other direction). There is no
    "half" tagging - that is if one of the photons of a Pi0 cannot be
    tagged, the whole tagging operation fails.  This is the most
    straight-forward thing to do.

    Here's what you have to do to find the MC Pi0 that corresponds to a 
    reconstructed Pi0 (NavPi0ToGG obejct):

    Before you loop over the reconstructed Pi0's, create a MCCCPi0Tagger object.

    MCCCPi0Tagger pi0Tagger(iFrame);

    Now, within your Pi0 loop, ask the tagger to find a match

    for ( ... )  // Over Pi0's with pi0Iter iterator
    {
       const MCParticle* mcPi0 = pi0Tagger( (*pi0Iter) );
       
       // Check that it worked
       if ( mcPi0 )
       {
          // Do stuff with the mcPi0 pointer, for example
	  report(INFO, kFacilityString) << "Hey, I found a MC "
	                                << mcPi0->properties().name()
					<< endl;
       }
    }


    If you want to go the other way (find the reconstructed Pi0 that corresponds
    to a MC Pi0), do:

    MCCCPi0Tagger pi0Tagger(iFrame);
    
    // Let's assume that mcParticle holds a Pi0 you want to tag
    const NavPi0ToGG* recoPi0 = pi0Tagger( mcParticle );

    if ( recoPi0 )
    {
      // Do stuff with the NavPi0ToGG pointer
    }
    
*/
//
// Author:      Adam Lyon
// Created:     Thu Mar  7 22:22:46 EST 2002
// $Id: MCCCPi0Tagger.h,v 1.1 2003/06/04 20:16:34 tom Exp $
//
// Revision history
//
// $Log: MCCCPi0Tagger.h,v $
// Revision 1.1  2003/06/04 20:16:34  tom
// Move higher-level Eta, Pi0, and Ks tagging code to Navigation to
//   live with the NavEta, NavPi0, and NavKs objects, which are built
//   from the more basic tracks and showers. Also eliminates circular
//   linking dependency that resulted from earlier location of these files
//   in the tagging libraries.
//
// Revision 1.2  2003/02/03 19:53:34  bkh
// Restructure interface.
//
// Revision 1.1  2002/03/14 23:35:43  lyon
// A Pi0 tagger
//

// system include files

// user include files
#include "MCCCTagger/MCCCGGTagger.h"
#include "MCCCTagger/MCCCTagger.h"
#include "Navigation/NavPi0ToGG.h"
#include "MCInfo/MCDecayTree/MCParticle.h"

// forward declarations

class MCCCPi0Tagger : public MCCCGGTagger< NavPi0ToGG >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MCCCPi0Tagger( const Frame& theFrame ) :
	 MCCCGGTagger< NavPi0ToGG > ( theFrame ) {};
      virtual ~MCCCPi0Tagger() {} ;

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // Return the NavPi0 given a MCParticle Pi0
      // (Pointer is zero if MCParticle is not a Pi0 or no match was found)
      const NavPi0ToGG* matchNavPi0ToMCPi0( const MCParticle& particle ) const
	 { return matchNavToMC( particle ) ; } ;
      
      // Return the MCParticle given a NavPi0
      // (Pointer is zero if no match was found)
      const MCParticle* matchMCPi0ToNavPi0( const NavPi0ToGG& navPi0 ) const
	 { return matchMCToNav( navPi0 ) ; } ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MCCCPi0Tagger( const MCCCPi0Tagger& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MCCCPi0Tagger& operator=( const MCCCPi0Tagger& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      // ---------- static data members ------------------------

};

// inline function definitions


#endif /* NAVIGATION_MCCCPI0TAGGER_H */
