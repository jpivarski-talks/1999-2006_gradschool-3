// -*- C++ -*-
#if !defined(ASKFORFITTINGWEIGHTS_ASKFORFITTINGWEIGHTS_H)
#define ASKFORFITTINGWEIGHTS_ASKFORFITTINGWEIGHTS_H
//
// Package:     <AskForFittingWeights>
// Module:      AskForFittingWeights
//
/**\class AskForFittingWeights AskForFittingWeights.h AskForFittingWeights/AskForFittingWeights.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Oct  1 14:36:31 EDT 2003
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
#include "HistogramInterface/HINtuple.h"

// forward declarations

class AskForFittingWeights : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kLayer,
	 kDrift,
	 kSinAlpha,
	 kCosTheta,
	 kFitWeight,

	 kNumEntries
      };

      // ------------ Constructors and destructor ----------------
      AskForFittingWeights( void );                      // anal1 
      virtual ~AskForFittingWeights();                   // anal5 

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

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      AskForFittingWeights( const AskForFittingWeights& );

      // ------------ assignment operator(s) ---------------------
      const AskForFittingWeights& operator=( const AskForFittingWeights& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (AskForFittingWeights::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* ASKFORFITTINGWEIGHTS_ASKFORFITTINGWEIGHTS_H */
