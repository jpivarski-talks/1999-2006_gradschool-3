// -*- C++ -*-
#if !defined(CONFLEV_CONFLEV_H)
#define CONFLEV_CONFLEV_H
//
// Package:     <ConfLev>
// Module:      ConfLev
//
/**\class ConfLev ConfLev.h ConfLev/ConfLev.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Wed Feb 20 19:00:24 EST 2002
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

class ConfLev : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      ConfLev( void );                      // anal1 
      virtual ~ConfLev();                   // anal5 

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
      ConfLev( const ConfLev& );

      // ------------ assignment operator(s) ---------------------
      const ConfLev& operator=( const ConfLev& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (ConfLev::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D* h_track_chi2;
      HIHist1D* h_track_chi2dof;
      HIHist1D* h_track_confLevel;
      HIHist1D* h_ks_chi2;
      HIHist1D* h_ks_chi2dof;
      HIHist1D* h_ks_confLevel;
      HIHist1D* h_ks_mass;
      HIHist2D* h_ks_vertex_xy;
      HIHist1D* h_ks_vertex_z;
      HIHist1D* h_ks_confLevel_non_ks;
      HIHist1D* h_ks_confLevel_ks;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CONFLEV_CONFLEV_H */
