// -*- C++ -*-
#if !defined(ASKFORTRACKS_ASKFORTRACKS_H)
#define ASKFORTRACKS_ASKFORTRACKS_H
//
// Package:     <AskForTracks>
// Module:      AskForTracks
//
/**\class AskForTracks AskForTracks.h AskForTracks/AskForTracks.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Feb 19 15:08:43 EST 2003
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

// forward declarations

class AskForTracks : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      AskForTracks( void );                      // anal1 
      virtual ~AskForTracks();                   // anal5 

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
      AskForTracks( const AskForTracks& );

      // ------------ assignment operator(s) ---------------------
      const AskForTracks& operator=( const AskForTracks& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (AskForTracks::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* ASKFORTRACKS_ASKFORTRACKS_H */
