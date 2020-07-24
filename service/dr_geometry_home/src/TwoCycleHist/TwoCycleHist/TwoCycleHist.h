// -*- C++ -*-
#if !defined(TWOCYCLEHIST_TWOCYCLEHIST_H)
#define TWOCYCLEHIST_TWOCYCLEHIST_H
//
// Package:     <TwoCycleHist>
// Module:      TwoCycleHist
//
/**\class TwoCycleHist TwoCycleHist.h TwoCycleHist/TwoCycleHist.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Fri May 30 14:58:21 EDT 2003
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

class TwoCycleHist : public Processor
{
      // ------------ friend classes and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      TwoCycleHist( void );                      // anal1 
      virtual ~TwoCycleHist();                   // anal5 

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
      TwoCycleHist( const TwoCycleHist& );

      // ------------ assignment operator(s) ---------------------
      const TwoCycleHist& operator=( const TwoCycleHist& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (TwoCycleHist::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      HIHist1D* m_cotTheta_m29;
      HIHist2D* m_d0miss_m29_2;
      HIHistProf* m_d0miss_m29;
      HIHist2D* m_z0miss_m29_2;
      HIHistProf* m_z0miss_m29;
      HIHist1D* m_cotTheta_m27;
      HIHist2D* m_d0miss_m27_2;
      HIHistProf* m_d0miss_m27;
      HIHist2D* m_z0miss_m27_2;
      HIHistProf* m_z0miss_m27;
      HIHist1D* m_cotTheta_m25;
      HIHist2D* m_d0miss_m25_2;
      HIHistProf* m_d0miss_m25;
      HIHist2D* m_z0miss_m25_2;
      HIHistProf* m_z0miss_m25;
      HIHist1D* m_cotTheta_m23;
      HIHist2D* m_d0miss_m23_2;
      HIHistProf* m_d0miss_m23;
      HIHist2D* m_z0miss_m23_2;
      HIHistProf* m_z0miss_m23;
      HIHist1D* m_cotTheta_m21;
      HIHist2D* m_d0miss_m21_2;
      HIHistProf* m_d0miss_m21;
      HIHist2D* m_z0miss_m21_2;
      HIHistProf* m_z0miss_m21;
      HIHist1D* m_cotTheta_m19;
      HIHist2D* m_d0miss_m19_2;
      HIHistProf* m_d0miss_m19;
      HIHist2D* m_z0miss_m19_2;
      HIHistProf* m_z0miss_m19;
      HIHist1D* m_cotTheta_m17;
      HIHist2D* m_d0miss_m17_2;
      HIHistProf* m_d0miss_m17;
      HIHist2D* m_z0miss_m17_2;
      HIHistProf* m_z0miss_m17;
      HIHist1D* m_cotTheta_m15;
      HIHist2D* m_d0miss_m15_2;
      HIHistProf* m_d0miss_m15;
      HIHist2D* m_z0miss_m15_2;
      HIHistProf* m_z0miss_m15;
      HIHist1D* m_cotTheta_m13;
      HIHist2D* m_d0miss_m13_2;
      HIHistProf* m_d0miss_m13;
      HIHist2D* m_z0miss_m13_2;
      HIHistProf* m_z0miss_m13;
      HIHist1D* m_cotTheta_m11;
      HIHist2D* m_d0miss_m11_2;
      HIHistProf* m_d0miss_m11;
      HIHist2D* m_z0miss_m11_2;
      HIHistProf* m_z0miss_m11;
      HIHist1D* m_cotTheta_m9;
      HIHist2D* m_d0miss_m9_2;
      HIHistProf* m_d0miss_m9;
      HIHist2D* m_z0miss_m9_2;
      HIHistProf* m_z0miss_m9;
      HIHist1D* m_cotTheta_m7;
      HIHist2D* m_d0miss_m7_2;
      HIHistProf* m_d0miss_m7;
      HIHist2D* m_z0miss_m7_2;
      HIHistProf* m_z0miss_m7;
      HIHist1D* m_cotTheta_m5;
      HIHist2D* m_d0miss_m5_2;
      HIHistProf* m_d0miss_m5;
      HIHist2D* m_z0miss_m5_2;
      HIHistProf* m_z0miss_m5;
      HIHist1D* m_cotTheta_m3;
      HIHist2D* m_d0miss_m3_2;
      HIHistProf* m_d0miss_m3;
      HIHist2D* m_z0miss_m3_2;
      HIHistProf* m_z0miss_m3;
      HIHist1D* m_cotTheta_m1;
      HIHist2D* m_d0miss_m1_2;
      HIHistProf* m_d0miss_m1;
      HIHist2D* m_z0miss_m1_2;
      HIHistProf* m_z0miss_m1;
      HIHist1D* m_cotTheta_p1;
      HIHist2D* m_d0miss_p1_2;
      HIHistProf* m_d0miss_p1;
      HIHist2D* m_z0miss_p1_2;
      HIHistProf* m_z0miss_p1;
      HIHist1D* m_cotTheta_p3;
      HIHist2D* m_d0miss_p3_2;
      HIHistProf* m_d0miss_p3;
      HIHist2D* m_z0miss_p3_2;
      HIHistProf* m_z0miss_p3;
      HIHist1D* m_cotTheta_p5;
      HIHist2D* m_d0miss_p5_2;
      HIHistProf* m_d0miss_p5;
      HIHist2D* m_z0miss_p5_2;
      HIHistProf* m_z0miss_p5;
      HIHist1D* m_cotTheta_p7;
      HIHist2D* m_d0miss_p7_2;
      HIHistProf* m_d0miss_p7;
      HIHist2D* m_z0miss_p7_2;
      HIHistProf* m_z0miss_p7;
      HIHist1D* m_cotTheta_p9;
      HIHist2D* m_d0miss_p9_2;
      HIHistProf* m_d0miss_p9;
      HIHist2D* m_z0miss_p9_2;
      HIHistProf* m_z0miss_p9;
      HIHist1D* m_cotTheta_p11;
      HIHist2D* m_d0miss_p11_2;
      HIHistProf* m_d0miss_p11;
      HIHist2D* m_z0miss_p11_2;
      HIHistProf* m_z0miss_p11;
      HIHist1D* m_cotTheta_p13;
      HIHist2D* m_d0miss_p13_2;
      HIHistProf* m_d0miss_p13;
      HIHist2D* m_z0miss_p13_2;
      HIHistProf* m_z0miss_p13;
      HIHist1D* m_cotTheta_p15;
      HIHist2D* m_d0miss_p15_2;
      HIHistProf* m_d0miss_p15;
      HIHist2D* m_z0miss_p15_2;
      HIHistProf* m_z0miss_p15;
      HIHist1D* m_cotTheta_p17;
      HIHist2D* m_d0miss_p17_2;
      HIHistProf* m_d0miss_p17;
      HIHist2D* m_z0miss_p17_2;
      HIHistProf* m_z0miss_p17;
      HIHist1D* m_cotTheta_p19;
      HIHist2D* m_d0miss_p19_2;
      HIHistProf* m_d0miss_p19;
      HIHist2D* m_z0miss_p19_2;
      HIHistProf* m_z0miss_p19;
      HIHist1D* m_cotTheta_p21;
      HIHist2D* m_d0miss_p21_2;
      HIHistProf* m_d0miss_p21;
      HIHist2D* m_z0miss_p21_2;
      HIHistProf* m_z0miss_p21;
      HIHist1D* m_cotTheta_p23;
      HIHist2D* m_d0miss_p23_2;
      HIHistProf* m_d0miss_p23;
      HIHist2D* m_z0miss_p23_2;
      HIHistProf* m_z0miss_p23;
      HIHist1D* m_cotTheta_p25;
      HIHist2D* m_d0miss_p25_2;
      HIHistProf* m_d0miss_p25;
      HIHist2D* m_z0miss_p25_2;
      HIHistProf* m_z0miss_p25;
      HIHist1D* m_cotTheta_p27;
      HIHist2D* m_d0miss_p27_2;
      HIHistProf* m_d0miss_p27;
      HIHist2D* m_z0miss_p27_2;
      HIHistProf* m_z0miss_p27;
      HIHist1D* m_cotTheta_p29;
      HIHist2D* m_d0miss_p29_2;
      HIHistProf* m_d0miss_p29;
      HIHist2D* m_z0miss_p29_2;
      HIHistProf* m_z0miss_p29;

      HIHistProf* m_z0miss_cotTheta;
      HIHistProf* m_posz0_cotTheta;
      HIHistProf* m_negz0_cotTheta;

      HIHist1D* m_chi2pos;
      HIHist1D* m_chi2neg;
      HIHistProf* m_chi2pos_vcotTh;
      HIHistProf* m_chi2neg_vcotTh;
      HIHistProf* m_chi2pos_vcotTh_spread;
      HIHistProf* m_chi2neg_vcotTh_spread;
      HIHist1D* m_probpos;
      HIHist1D* m_probneg;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* TWOCYCLEHIST_TWOCYCLEHIST_H */
