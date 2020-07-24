// -*- C++ -*-
#if !defined(COMPAREINOUTCHI2_COMPAREINOUTCHI2_H)
#define COMPAREINOUTCHI2_COMPAREINOUTCHI2_H
//
// Package:     <CompareInOutChi2>
// Module:      CompareInOutChi2
//
/**\class CompareInOutChi2 CompareInOutChi2.h CompareInOutChi2/CompareInOutChi2.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Thu May 22 16:28:02 EDT 2003
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

class CompareInOutChi2 : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      enum {
	 kNTracks,
	 kPionUsed,
	 kPionIn,
	 kPionDrop,
	 kPionIter,
	 kPionFit,
	 kExitUsed,
	 kExitIn,
	 kExitDrop,
	 kExitIter,
	 kExitFit,

	 kNumEntries
      };

      // ------------ Constructors and destructor ----------------
      CompareInOutChi2( void );                      // anal1 
      virtual ~CompareInOutChi2();                   // anal5 

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
      CompareInOutChi2( const CompareInOutChi2& );

      // ------------ assignment operator(s) ---------------------
      const CompareInOutChi2& operator=( const CompareInOutChi2& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CompareInOutChi2::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_ntuple;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* COMPAREINOUTCHI2_COMPAREINOUTCHI2_H */
