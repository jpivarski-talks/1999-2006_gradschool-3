#if !defined(NAVIGATION_MCTKSHORTTAGGER_H)
#define NAVIGATION_MCTKSHORTTAGGER_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      MCTKShortTagger
// 
/**\class MCTKShortTagger MCTKShortTagger.h Navigation/MCTKShortTagger.h

 Description: A KShort tagger

 Usage:
    Determine the Monte Carlo K0s that matches a reconstructed K0s or
    vice-versa. 
    
    The algorithm is to determine the pions that belong to the
    reconstructed K0s then match them to MC pions. These MC pions
    are then checked to see if they come from the same MC K0s. If they
    do, then a pointer to the MC K0s object is returned (swap
    reconstructed for MC to tag the other direction). There is no
    "half" tagging - that is if one of the pions of a K0s cannot be
    tagged, the whole tagging operation fails.  This is the most
    straight-forward thing to do.

    Here's what you have to do to find the MC K0s that corresponds to a 
    reconstructed K0s (NavK0s object):

    Before you loop over the reconstructed K0s's, create a MCTK0sTagger object.

    MCTK0sTagger k0sTagger(iFrame);

    Now, within your K0s loop, ask the tagger to find a match

    for ( ... )  // Over K0s's with k0sIter iterator
    {
       const MCParticle* mcK0s = k0sTagger( (*k0sIter) );
       
       // Check that it worked
       if ( mcK0s )
       {
          // Do stuff with the mcK0s pointer, for example
	  report(INFO, kFacilityString) << "Hey, I found a MC "
	                                << mcK0s->properties().name()
					<< endl;
       }
    }


    If you want to go the other way (find the reconstructed K0s that corresponds
    to a MC K0s), do:

    MCTK0sTagger k0sTagger(iFrame);
    
    // Let's assume that mcParticle holds a K0s you want to tag
    const NavK0s* recoK0s = k0sTagger( mcParticle );

    if ( recoK0s )
    {
      // Do stuff with the NavK0s pointer
    }

*/
//
// Author:      Adam Lyon
// Created:     Fri Mar  8 14:14:23 EST 2002
// $Id: MCTKShortTagger.h,v 1.1 2003/06/04 20:16:35 tom Exp $
//
// Revision history
//
// $Log: MCTKShortTagger.h,v $
// Revision 1.1  2003/06/04 20:16:35  tom
// Move higher-level Eta, Pi0, and Ks tagging code to Navigation to
//   live with the NavEta, NavPi0, and NavKs objects, which are built
//   from the more basic tracks and showers. Also eliminates circular
//   linking dependency that resulted from earlier location of these files
//   in the tagging libraries.
//
// Revision 1.2  2003/02/03 19:55:31  bkh
// Restructure interface
//
// Revision 1.1  2002/03/14 23:41:14  lyon
// A KShort tagger
//

// system include files

// user include files

// forward declarations
#include "DataHandler/Frame.h"
#include "Navigation/NavKs.h"
#include "MCInfo/MCDecayTree/MCParticle.h"

#include "TrackRoot/TRSeedTrack.h"

class MCTKShortTagger
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MCTKShortTagger(Frame& theFrame);
      virtual ~MCTKShortTagger();

      // ---------- member functions ---------------------------
      // Return the NavKs given a MCParticle Ks
      // (Pointer is zero if MCParticle is not a Ks or no match was found)
      const NavKs* matchNavKsToMCKs(const MCParticle& particle);
      
      // Return the MCParticle given a NavKs
      // (Pointer is zero if no match was found)
      const MCParticle* matchMCKsToNavKs(const NavKs& navKs);

      // ---------- const member functions ---------------------


      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MCTKShortTagger( const MCTKShortTagger& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MCTKShortTagger& operator=( const MCTKShortTagger& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Frame& m_theFrame;

      // ---------- static data members ------------------------

};

// inline function definitions

#endif /* NAVIGATION_MCTKSHORTTAGGER_H */
