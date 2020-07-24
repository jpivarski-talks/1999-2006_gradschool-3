#if !defined(NAVIGATION_MCCCETATAGGER_H)
#define NAVIGATION_MCCCETATAGGER_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      MCCCEtaTagger
// 
/**\class MCCCEtaTagger MCCCEtaTagger.h Navigation/MCCCEtaTagger.h

 Description: A Eta Tagger

 Usage:
 
    Determine the Monte Carlo Eta that matches a reconstructed Eta or
    vice-versa. 
    
    The algorithm is to determine the photons that belong to the
    reconstructed Eta then match them to MC photons. These MC photons
    are then checked to see if they come from the same MC Eta. If they
    do, then a pointer to the MC Eta object is returned (swap
    reconstructed for MC to tag the other direction). There is no
    "half" tagging - that is if one of the photons of a Eta cannot be
    tagged, the whole tagging operation fails.  This is the most
    straight-forward thing to do.

    Here's what you have to do to find the MC Eta that corresponds to a 
    reconstructed Eta (NavEtaToGG obejct):

    Before you loop over the reconstructed Eta's, create a MCCCEtaTagger object.

    MCCCEtaTagger EtaTagger(iFrame);

    Now, within your Eta loop, ask the tagger to find a match

    for ( ... )  // Over Eta's with EtaIter iterator
    {
       const MCParticle* mcEta = EtaTagger( (*EtaIter) );
       
       // Check that it worked
       if ( mcEta )
       {
          // Do stuff with the mcEta pointer, for example
	  report(INFO, kFacilityString) << "Hey, I found a MC "
	                                << mcEta->properties().name()
					<< endl;
       }
    }


    If you want to go the other way (find the reconstructed Eta that corresponds
    to a MC Eta), do:

    MCCCEtaTagger EtaTagger(iFrame);
    
    // Let's assume that mcParticle holds a Eta you want to tag
    const NavEtaToGG* recoEta = EtaTagger( mcParticle );

    if ( recoEta )
    {
      // Do stuff with the NavEtaToGG pointer
    }
    
*/
//
// Author:      Adam Lyon
// Created:     Thu Mar  7 22:22:46 EST 2002
// $Id: MCCCEtaTagger.h,v 1.1 2003/06/04 20:16:34 tom Exp $
//
// Revision history
//
// $Log: MCCCEtaTagger.h,v $
// Revision 1.1  2003/06/04 20:16:34  tom
// Move higher-level Eta, Pi0, and Ks tagging code to Navigation to
//   live with the NavEta, NavPi0, and NavKs objects, which are built
//   from the more basic tracks and showers. Also eliminates circular
//   linking dependency that resulted from earlier location of these files
//   in the tagging libraries.
//
// Revision 1.1  2003/02/03 19:53:34  bkh
// Restructure interface.
//
// Revision 1.1  2002/03/14 23:35:43  lyon
// A Eta tagger
//

// system include files

// user include files
#include "MCCCTagger/MCCCGGTagger.h"
#include "MCCCTagger/MCCCTagger.h"
#include "Navigation/NavEtaToGG.h"
#include "MCInfo/MCDecayTree/MCParticle.h"

// forward declarations

class MCCCEtaTagger : public MCCCGGTagger< NavEtaToGG >
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      MCCCEtaTagger( const Frame& theFrame ) :
	 MCCCGGTagger< NavEtaToGG > ( theFrame ) {};
      virtual ~MCCCEtaTagger() {} ;

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // Return the NavEta given a MCParticle Eta
      // (Pointer is zero if MCParticle is not a Eta or no match was found)
      const NavEtaToGG* matchNavEtaToMCEta( const MCParticle& particle ) const
	 { return matchNavToMC( particle ) ; } ;
      
      // Return the MCParticle given a NavEta
      // (Pointer is zero if no match was found)
      const MCParticle* matchMCEtaToNavEta( const NavEtaToGG& navEta ) const
	 { return matchMCToNav( navEta ) ; } ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MCCCEtaTagger( const MCCCEtaTagger& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MCCCEtaTagger& operator=( const MCCCEtaTagger& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      // ---------- static data members ------------------------

};

// inline function definitions


#endif /* NAVIGATION_MCCCETATAGGER_H */
