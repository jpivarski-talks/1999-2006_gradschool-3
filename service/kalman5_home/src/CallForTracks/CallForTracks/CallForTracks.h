// -*- C++ -*-
#if !defined(CALLFORTRACKS_CALLFORTRACKS_H)
#define CALLFORTRACKS_CALLFORTRACKS_H
//
// Package:     <CallForTracks>
// Module:      CallForTracks
//
/**\class CallForTracks CallForTracks.h CallForTracks/CallForTracks.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu May 22 11:23:59 EDT 2003
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

class CallForTracks : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      CallForTracks( void );                      // anal1 
      virtual ~CallForTracks();                   // anal5 

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
      CallForTracks( const CallForTracks& );

      // ------------ assignment operator(s) ---------------------
      const CallForTracks& operator=( const CallForTracks& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CallForTracks::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      int m_tracks;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CALLFORTRACKS_CALLFORTRACKS_H */
