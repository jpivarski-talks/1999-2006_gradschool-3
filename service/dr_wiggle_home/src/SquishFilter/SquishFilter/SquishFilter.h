// -*- C++ -*-
#if !defined(SQUISHFILTER_SQUISHFILTER_H)
#define SQUISHFILTER_SQUISHFILTER_H
//
// Package:     <SquishFilter>
// Module:      SquishFilter
//
/**\class SquishFilter SquishFilter.h SquishFilter/SquishFilter.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Tue Feb  4 10:29:04 EST 2003
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

class SquishFilter : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      SquishFilter( void );                      // anal1 
      virtual ~SquishFilter();                   // anal5 

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
      SquishFilter( const SquishFilter& );

      // ------------ assignment operator(s) ---------------------
      const SquishFilter& operator=( const SquishFilter& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (SquishFilter::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D* m_chiSquare_first;
      HIHist1D* m_chiSquare_second;
      HIHist1D* m_d0_first;
      HIHist1D* m_d0_second;
      HIHist1D* m_z0Diff;
      HIHist1D* m_cotThetaSum;
      HIHist1D* m_bunchTimeBefore;
      HIHist1D* m_bunchTimeAfter;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* SQUISHFILTER_SQUISHFILTER_H */
