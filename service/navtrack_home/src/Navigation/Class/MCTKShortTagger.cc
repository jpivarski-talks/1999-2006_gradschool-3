// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      MCTKShortTagger
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Fri Mar  8 14:14:33 EST 2002
// $Id: MCTKShortTagger.cc,v 1.1 2003/06/04 20:16:43 tom Exp $
//
// Revision history
//
// $Log: MCTKShortTagger.cc,v $
// Revision 1.1  2003/06/04 20:16:43  tom
// Move higher-level Eta, Pi0, and Ks tagging code to Navigation to
//   live with the NavEta, NavPi0, and NavKs objects, which are built
//   from the more basic tracks and showers. Also eliminates circular
//   linking dependency that resulted from earlier location of these files
//   in the tagging libraries.
//
// Revision 1.4  2003/02/18 16:56:50  tom
// Modify algorithm used in matchNavKsToMCKs() from simply looking
//   through reconstructed Nav objects for a tag matching the MC Ks, to
//   actually looking for a Nav object constructed from the tracks
//   tagged to MC daughters of the Ks. This better reflects the
//   underlying assymetry in the MC <-> Nav map, using only the
//   "MC -> Nav" direction to answer a question about a MC -> Nav match.
//
// Revision 1.3  2003/02/17 12:56:06  tom
// Fixed typo in check of MCParticle type for Nav->MC tagging
//
// Revision 1.2  2003/02/03 19:55:14  bkh
// Restructure interface
//
// Revision 1.1  2002/03/14 23:41:10  lyon
// A KShort tagger
//

#include "Experiment/Experiment.h"

// system include files
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
//#include <algorithm>
//#include <utility>

// user include files
//#include "Experiment/report.h"
#include "Navigation/MCTKShortTagger.h"

#include "DataHandler/Record.h"
#include "DataHandler/Frame.h"
#include "FrameAccess/extract.h"
#include "FrameAccess/FAItem.h"
#include "FrameAccess/FATable.h"

#include "Navigation/NavTrack.h" // include NavTrack
#include "Navigation/NavKs.h"
#include "MCInfo/MCDecayTree/MCParticle.h" // include MCParticle
#include "MCInfo/MCDecayTree/MCVertex.h" // include MCVertex

#include "MCTrackTagger/MCTrackTagsByMCParticle.h"

//
// constants, enums and typedefs
//

static const char* const kFacilityString =
"Navigation.MCTKShortTagger" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  =
"$Id: MCTKShortTagger.cc,v 1.1 2003/06/04 20:16:43 tom Exp $";

static const char* const kTagString =
"$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
MCTKShortTagger::MCTKShortTagger(Frame& theFrame) :
   m_theFrame( theFrame )
{}

// MCTKShortTagger::MCTKShortTagger( const MCTKShortTagger& rhs )
// {
//    // do actual copying here; if you implemented
//    // operator= correctly, you may be able to use just say      
//    *this = rhs;
// }

MCTKShortTagger::~MCTKShortTagger()
{}

//
// assignment operators
//
// const MCTKShortTagger& MCTKShortTagger::operator=( const MCTKShortTagger& rhs )
// {
//   if( this != &rhs ) {
//      // do actual copying here, plus:
//      // "SuperClass"::operator=( rhs );
//   }
//
//   return *this;
// }

//
// member functions
//

//
// const member functions
//

const NavKs* 
MCTKShortTagger::matchNavKsToMCKs( const MCParticle& particle )
{
  // 2003.02.17 tom
  // New implementation that uses the MC -> Nav map instead of relying
  //   on the Nav -> MC map. (Tagging is only approximately symmetric.)

  // Check that this is really a Ks
  if (particle.properties().name() != "K0S") return 0;
  
  // Check for decay
  const MCVertex* deathVtx = particle.deathVertex();
  if (0 == deathVtx) return 0;
  
  // Check for decay to two children (PI+ PI-)
  if (deathVtx->nChildren() != 2) return 0;

  // Find the two (oppositely) charged pions
  const MCParticle* piP = 0;
  const MCParticle* piM = 0;
  for (MCVertex::const_iterator child = deathVtx->pBegin();
       child != deathVtx->pEnd(); ++child)
  {
    if (child->properties().name() == "PI+") piP = &(*child);
    if (child->properties().name() == "PI-") piM = &(*child);
  }

  // Better have found both charged pions
  if ((0 == piP) || (0 == piM)) return 0;
  
  // Extract list of tracks tagged to particles
  FATable< MCTrackTagsByMCParticle > tagsByParticle;
  extract(m_theFrame.record(Stream::kEvent), tagsByParticle);

  // Find tracks tagged to the two pions
  FATable< MCTrackTagsByMCParticle >::const_iterator piPTag =
    tagsByParticle.find(piP->identifier());

  FATable< MCTrackTagsByMCParticle >::const_iterator piMTag =
    tagsByParticle.find(piM->identifier());

  // Better have both tracks
  if ((tagsByParticle.end() == piPTag) ||
      (tagsByParticle.end() == piMTag)    ) return 0;

  // Find the NavKs, if any, built from these two tracks
  const Count piPTrkId = piPTag->bestTrackID();
  const Count piMTrkId = piMTag->bestTrackID();

  // Extract reconstructed Ks
  FATable< NavKs >                              navKses   ;
  extract( m_theFrame.record( Stream::kEvent ), navKses ) ;

  // Loop over them
  FATable<NavKs>::const_iterator navEnd = navKses.end();
  for ( FATable<NavKs>::const_iterator navIter = navKses.begin();
        navIter != navEnd; ++navIter )
  {
    if ( (navIter->kShort().piPlusId()  == piPTrkId) &&
         (navIter->kShort().piMinusId() == piMTrkId)    )
    {
      // Found it!
      return &( *navIter );
    }
  }

  return 0;
}

const MCParticle* 
MCTKShortTagger::matchMCKsToNavKs( const NavKs& navKs )
{
   enum { qqPiPlus = 21, qqPiMinus=22, qqK0s = 57 };

   // Get the MC particles that match the showers
   const Count recoPionIDs[2] = { navKs.piPlus().identifier(), 
				  navKs.piMinus().identifier()  } ;

   const NavTrack& plus  ( navKs.piPlus()  ) ;
   const NavTrack& minus ( navKs.piMinus() ) ;

   FAItem< MCParticle > mcPlus  ( plus.mcTag()  ) ;
   FAItem< MCParticle > mcMinus ( minus.mcTag() ) ;

   if( mcPlus.valid()                           &&
       mcMinus.valid()                          &&
       qqPiPlus  == mcPlus->properties().QQId() &&
       qqPiMinus == mcMinus->properties().QQId()     )
   {
      const MCVertex* vPlus  ( mcPlus ->productionVertex() ) ;
      const MCVertex* vMinus ( mcMinus->productionVertex() ) ;
      if( 0 != vPlus &&
	  0 != vMinus   )
      {
	 const MCParticle& parPlus  ( vPlus ->parent() ) ;
	 const MCParticle& parMinus ( vMinus->parent() ) ;
	 const UInt32 qqPlus  ( parPlus .properties().QQId() ) ;
	 const UInt32 qqMinus ( parMinus.properties().QQId() ) ;
	 const UInt32 idPlus  ( parPlus .identifier() ) ;
	 const UInt32 idMinus ( parMinus.identifier() ) ;
	 if( qqPlus  == qqK0s &&
	     qqMinus == qqK0s &&
	     idPlus  == idMinus  )
	 {
	    return &parPlus ;
	 }
      }
   }
   return 0 ;
}
  


//
// static member functions
//
