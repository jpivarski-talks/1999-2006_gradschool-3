// -*- C++ -*-
#if !defined(CAKEALIGN_CAKEALIGN_H)
#define CAKEALIGN_CAKEALIGN_H
//
// Package:     <CakeAlign>
// Module:      CakeAlign
//
/**\class CakeAlign CakeAlign.h CakeAlign/CakeAlign.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Tue Apr  9 16:45:03 EDT 2002
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

class CakeAlign : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      CakeAlign( void );                      // anal1 
      virtual ~CakeAlign();                   // anal5 

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
      CakeAlign( const CakeAlign& );

      // ------------ assignment operator(s) ---------------------
      const CakeAlign& operator=( const CakeAlign& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CakeAlign::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D* m_west_all_1;
      HIHist1D* m_west_cosphi_1;
      HIHist1D* m_west_sinphi_1;
      HIHist1D* m_east_all_1;
      HIHist1D* m_east_cosphi_1;
      HIHist1D* m_east_sinphi_1;

      HIHist1D* m_west_all_2;
      HIHist1D* m_west_cosphi_2;
      HIHist1D* m_west_sinphi_2;
      HIHist1D* m_east_all_2;
      HIHist1D* m_east_cosphi_2;
      HIHist1D* m_east_sinphi_2;

      HIHist1D* m_west_all_3;
      HIHist1D* m_west_cosphi_3;
      HIHist1D* m_west_sinphi_3;
      HIHist1D* m_east_all_3;
      HIHist1D* m_east_cosphi_3;
      HIHist1D* m_east_sinphi_3;

      HIHist1D* m_west_all_4;
      HIHist1D* m_west_cosphi_4;
      HIHist1D* m_west_sinphi_4;
      HIHist1D* m_east_all_4;
      HIHist1D* m_east_cosphi_4;
      HIHist1D* m_east_sinphi_4;

      HIHist1D* m_west_all_5;
      HIHist1D* m_west_cosphi_5;
      HIHist1D* m_west_sinphi_5;
      HIHist1D* m_east_all_5;
      HIHist1D* m_east_cosphi_5;
      HIHist1D* m_east_sinphi_5;

      HIHist1D* m_west_all_6;
      HIHist1D* m_west_cosphi_6;
      HIHist1D* m_west_sinphi_6;
      HIHist1D* m_east_all_6;
      HIHist1D* m_east_cosphi_6;
      HIHist1D* m_east_sinphi_6;

      HIHist1D* m_west_all_7;
      HIHist1D* m_west_cosphi_7;
      HIHist1D* m_west_sinphi_7;
      HIHist1D* m_east_all_7;
      HIHist1D* m_east_cosphi_7;
      HIHist1D* m_east_sinphi_7;

      HIHist1D* m_west_all_8;
      HIHist1D* m_west_cosphi_8;
      HIHist1D* m_west_sinphi_8;
      HIHist1D* m_east_all_8;
      HIHist1D* m_east_cosphi_8;
      HIHist1D* m_east_sinphi_8;

      HIHistProf* m_momentum_vs_phi0_pos;
      HIHistProf* m_momentum_vs_phi0_pos_subtracted;

      HIHistProf* m_momentum_vs_phi0_neg;
      HIHistProf* m_momentum_vs_phi0_neg_subtracted;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CAKEALIGN_CAKEALIGN_H */
