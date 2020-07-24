// -*- C++ -*-
#if !defined(ENDERSKIM_ENDERSKIM_H)
#define ENDERSKIM_ENDERSKIM_H
//
// Package:     <EnderSkim>
// Module:      EnderSkim
//
/**\class EnderSkim EnderSkim.h EnderSkim/EnderSkim.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Mon Sep 17 15:42:08 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"

#include "ASiStorePro/ASiStore.h"
#include "SiHits/CalibratedSVRphiHit.h"
#include "SiHits/CalibratedSVZHit.h"
#include "Navigation/NavTrack.h"
#include "HelixIntersection/HIHelix.h"

#include <stdio.h>

// forward declarations

class EnderSkim : public Processor
{
      // ------------ friend classses and functions --------------
      typedef enum {
	 kNothing,
	 kTrack,
	 kSVRphiHit,
	 kSVZHit
      } NextInFile;

      typedef unsigned int uint;

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      EnderSkim( void );                      // anal1 
      virtual ~EnderSkim();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      //virtual ActionBase::ActionResult beginRun( Frame& iFrame);
      //virtual ActionBase::ActionResult endRun( Frame& iFrame);
      //virtual ActionBase::ActionResult geometry( Frame& iFrame);
      //virtual ActionBase::ActionResult hardware( Frame& iFrame);
      //virtual ActionBase::ActionResult user( Frame& iFrame);

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      EnderSkim( const EnderSkim& );

      // ------------ assignment operator(s) ---------------------
      const EnderSkim& operator=( const EnderSkim& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (EnderSkim::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      DABoolean write_track( HIHelix* helixPtr, FILE* filePtr );
      DABoolean write_rphi_hit( CalibratedSVRphiHit* hitPtr, FILE* filePtr );
      DABoolean write_z_hit( CalibratedSVZHit* hitPtr, FILE* filePtr );

      // ------------ data members -------------------------------

      Parameter< string > m_skim_dir;

      FILE* m_ladder[61];
      FILE* m_ladder_noz[61];

      uint m_tracks;
      uint m_rphi_ladder[61];
      uint m_z_ladder[61];

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* ENDERSKIM_ENDERSKIM_H */
