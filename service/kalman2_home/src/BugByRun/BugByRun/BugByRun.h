// -*- C++ -*-
#if !defined(BUGBYRUN_BUGBYRUN_H)
#define BUGBYRUN_BUGBYRUN_H
//
// Package:     <BugByRun>
// Module:      BugByRun
//
/**\class BugByRun BugByRun.h BugByRun/BugByRun.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Jan 27 11:50:30 EST 2003
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

class BugByRun : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      BugByRun( void );                      // anal1 
      virtual ~BugByRun();                   // anal5 

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
      BugByRun( const BugByRun& );

      // ------------ assignment operator(s) ---------------------
      const BugByRun& operator=( const BugByRun& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (BugByRun::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      int run, buggies, tracks;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* BUGBYRUN_BUGBYRUN_H */
