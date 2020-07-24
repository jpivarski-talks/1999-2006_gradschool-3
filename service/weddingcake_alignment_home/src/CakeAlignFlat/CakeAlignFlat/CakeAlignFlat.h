// -*- C++ -*-
#if !defined(CAKEALIGNFLAT_CAKEALIGNFLAT_H)
#define CAKEALIGNFLAT_CAKEALIGNFLAT_H
//
// Package:     <CakeAlignFlat>
// Module:      CakeAlignFlat
//
/**\class CakeAlignFlat CakeAlignFlat.h CakeAlignFlat/CakeAlignFlat.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Tue May 14 11:24:04 EDT 2002
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

class CakeAlignFlat : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------
      enum {
	 kFlatGranularity = 100
      };

      // ------------ Constructors and destructor ----------------
      CakeAlignFlat( void );                      // anal1 
      virtual ~CakeAlignFlat();                   // anal5 

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
      CakeAlignFlat( const CakeAlignFlat& );

      // ------------ assignment operator(s) ---------------------
      const CakeAlignFlat& operator=( const CakeAlignFlat& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (CakeAlignFlat::*iMethod)( Frame& ),
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

      HIHist1D* m_poscurv_poscotTheta;
      HIHist1D* m_poscurv_negcotTheta;
      HIHist1D* m_negcurv_poscotTheta;
      HIHist1D* m_negcurv_negcotTheta;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* CAKEALIGNFLAT_CAKEALIGNFLAT_H */
