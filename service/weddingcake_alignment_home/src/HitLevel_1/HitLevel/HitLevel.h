// -*- C++ -*-
#if !defined(HITLEVEL_HITLEVEL_H)
#define HITLEVEL_HITLEVEL_H
//
// Package:     <HitLevel>
// Module:      HitLevel
//
/**\class HitLevel HitLevel.h HitLevel/HitLevel.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri Jun 15 16:05:17 EDT 2001
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

class HitLevel : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      HitLevel( void );                      // anal1 
      virtual ~HitLevel();                   // anal5 

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
      HitLevel( const HitLevel& );

      // ------------ assignment operator(s) ---------------------
      const HitLevel& operator=( const HitLevel& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (HitLevel::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HINtuple* m_hitvars;
      unsigned int m_num_hits;
      DABoolean m_done;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* HITLEVEL_HITLEVEL_H */
