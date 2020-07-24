// -*- C++ -*-
#if !defined(FINDFITABORTS_FINDFITABORTS_H)
#define FINDFITABORTS_FINDFITABORTS_H
//
// Package:     <FindFitAborts>
// Module:      FindFitAborts
//
/**\class FindFitAborts FindFitAborts.h FindFitAborts/FindFitAborts.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Tue Oct 16 15:56:24 EDT 2001
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

class FindFitAborts : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      FindFitAborts( void );                      // anal1 
      virtual ~FindFitAborts();                   // anal5 

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
      FindFitAborts( const FindFitAborts& );

      // ------------ assignment operator(s) ---------------------
      const FindFitAborts& operator=( const FindFitAborts& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (FindFitAborts::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* FINDFITABORTS_FINDFITABORTS_H */
