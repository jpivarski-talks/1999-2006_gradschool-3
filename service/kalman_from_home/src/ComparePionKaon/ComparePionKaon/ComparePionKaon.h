// -*- C++ -*-
#if !defined(COMPAREPIONKAON_COMPAREPIONKAON_H)
#define COMPAREPIONKAON_COMPAREPIONKAON_H
//
// Package:     <ComparePionKaon>
// Module:      ComparePionKaon
//
/**\class ComparePionKaon ComparePionKaon.h ComparePionKaon/ComparePionKaon.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Dec 18 12:44:22 EST 2002
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

class ComparePionKaon : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      ComparePionKaon( void );                      // anal1 
      virtual ~ComparePionKaon();                   // anal5 

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
      ComparePionKaon( const ComparePionKaon& );

      // ------------ assignment operator(s) ---------------------
      const ComparePionKaon& operator=( const ComparePionKaon& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (ComparePionKaon::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D* m_pion_kaon;
      HIHist1D* m_kaon_pion;
      HIHist1D* m_exitPion_kaon;
      HIHist1D* m_exitPion_pion;
      HIHist1D* m_exitKaon_kaon;
      HIHist1D* m_exitKaon_pion;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* COMPAREPIONKAON_COMPAREPIONKAON_H */
