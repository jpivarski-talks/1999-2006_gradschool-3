// -*- C++ -*-
#if !defined(ALIGNMENTPLOTS_ALIGNMENTPLOTS_H)
#define ALIGNMENTPLOTS_ALIGNMENTPLOTS_H
//
// Package:     <AlignmentPlots>
// Module:      AlignmentPlots
//
/**\class AlignmentPlots AlignmentPlots.h AlignmentPlots/AlignmentPlots.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri Mar  8 13:42:55 EST 2002
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
#include "CommandPattern/Parameter.h"

// forward declarations

class AlignmentPlots : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------
      enum {
	 kD0p, kZ0p, kCtp, kPhi0p, kMomp,
	 kD0n, kZ0n, kCtn, kPhi0n, kMomn,
	 kR1pexist, kR1plad, kR1pwaf, kR1px, kR1pfres, kR1pfrese, kR1phres, kR1pphi, kR1ptz,
	 kR2pexist, kR2plad, kR2pwaf, kR2px, kR2pfres, kR2pfrese, kR2phres, kR2pphi, kR2ptz,
	 kR3pexist, kR3plad, kR3pwaf, kR3px, kR3pfres, kR3pfrese, kR3phres, kR3pphi, kR3ptz,
	 kR4pexist, kR4plad, kR4pwaf, kR4px, kR4pfres, kR4pfrese, kR4phres, kR4pphi, kR4ptz,
	 kR1nexist, kR1nlad, kR1nwaf, kR1nx, kR1nfres, kR1nfrese, kR1nhres, kR1nphi, kR1ntz,
	 kR2nexist, kR2nlad, kR2nwaf, kR2nx, kR2nfres, kR2nfrese, kR2nhres, kR2nphi, kR2ntz,
	 kR3nexist, kR3nlad, kR3nwaf, kR3nx, kR3nfres, kR3nfrese, kR3nhres, kR3nphi, kR3ntz,
	 kR4nexist, kR4nlad, kR4nwaf, kR4nx, kR4nfres, kR4nfrese, kR4nhres, kR4nphi, kR4ntz,
	 kZ1pexist, kZ1plad, kZ1pwaf, kZ1pz, kZ1pfres, kZ1pfrese, kZ1phres, kZ1pphi, kZ1ptz,
	 kZ2pexist, kZ2plad, kZ2pwaf, kZ2pz, kZ2pfres, kZ2pfrese, kZ2phres, kZ2pphi, kZ2ptz,
	 kZ3pexist, kZ3plad, kZ3pwaf, kZ3pz, kZ3pfres, kZ3pfrese, kZ3phres, kZ3pphi, kZ3ptz,
	 kZ4pexist, kZ4plad, kZ4pwaf, kZ4pz, kZ4pfres, kZ4pfrese, kZ4phres, kZ4pphi, kZ4ptz,
	 kZ1nexist, kZ1nlad, kZ1nwaf, kZ1nz, kZ1nfres, kZ1nfrese, kZ1nhres, kZ1nphi, kZ1ntz,
	 kZ2nexist, kZ2nlad, kZ2nwaf, kZ2nz, kZ2nfres, kZ2nfrese, kZ2nhres, kZ2nphi, kZ2ntz,
	 kZ3nexist, kZ3nlad, kZ3nwaf, kZ3nz, kZ3nfres, kZ3nfrese, kZ3nhres, kZ3nphi, kZ3ntz,
	 kZ4nexist, kZ4nlad, kZ4nwaf, kZ4nz, kZ4nfres, kZ4nfrese, kZ4nhres, kZ4nphi, kZ4ntz,
      kSuperLength };      

      // ------------ Constructors and destructor ----------------
      AlignmentPlots( void );                      // anal1 
      virtual ~AlignmentPlots();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult event( Frame& iFrame );
      virtual ActionBase::ActionResult startRun( Frame& iFrame);
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
      AlignmentPlots( const AlignmentPlots& );

      // ------------ assignment operator(s) ---------------------
      const AlignmentPlots& operator=( const AlignmentPlots& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (AlignmentPlots::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------

      Parameter< DABoolean > m_useKalmanResids;
      Parameter< DABoolean > m_useMuPairs;
      Parameter< DABoolean > m_useDualTracks;
      Parameter< double > m_beamEnergy;
      Parameter< DABoolean > m_weightHits;
      Parameter< double > m_rphiCut;
      Parameter< double > m_zCut;
      Parameter< double > m_requiredRphiLayers;
      Parameter< double > m_requiredZLayers;
      Parameter< DABoolean > m_fillSuperNtuple;
      Parameter< DABoolean > m_fillRphiHitNtuple;
      Parameter< DABoolean > m_fillZHitNtuple;
      Parameter< DABoolean > m_fillMatchedNtuple;
      Parameter< DABoolean > m_fillRphiOverlapNtuple;
      Parameter< DABoolean > m_fillZOverlapNtuple;

      HINtuple* m_super_ntuple;

      HIHist1D* m_rphi_hits_vs_phi_l1;
      HIHist1D* m_rphi_hits_vs_phi_l2;
      HIHist1D* m_rphi_hits_vs_phi_l3;
      HIHist1D* m_rphi_hits_vs_phi_l4;
      HIHist1D* m_z_hits_vs_phi_l1;
      HIHist1D* m_z_hits_vs_phi_l2;
      HIHist1D* m_z_hits_vs_phi_l3;
      HIHist1D* m_z_hits_vs_phi_l4;

      HIHistProf* m_rphires_v_x_negz[61];
      HIHist2D* m_rphires_v_x_negz_scat[61];
      HIHistProf* m_rphires_v_x_posz[61];
      HIHist2D* m_rphires_v_x_posz_scat[61];
      HIHistProf* m_rphires_v_trackz[61];
      HIHist2D* m_rphires_v_trackz_scat[61];
      HIHistProf* m_zres_v_trackz[61];
      HIHist2D* m_zres_v_trackz_scat[61];
      HIHistProf* m_zres_v_hitz[61];
      HIHist2D* m_zres_v_hitz_scat[61];
      HIHistProf* m_zres_v_trackz_matched[61];
      HIHist2D* m_zres_v_trackz_matched_scat[61];
      HIHistProf* m_zres_v_hitz_matched[61];
      HIHist2D* m_zres_v_hitz_matched_scat[61];
      HIHist1D* m_rphi_overlap[61];
      HIHist1D* m_z_overlap[61];

      HINtuple* m_rphi_hit_ntuple;
      HINtuple* m_z_hit_ntuple;
      HINtuple* m_matched_ntuple;
      HINtuple* m_rphi_overlap_ntuple;
      HINtuple* m_z_overlap_ntuple;

      HIHistProf* m_layer1left1;
      HIHist2D* m_layer1left1_scat;
      HIHistProf* m_layer1middle;
      HIHist2D* m_layer1middle_scat;
      HIHistProf* m_layer1right1;
      HIHist2D* m_layer1right1_scat;

      HIHistProf* m_layer2left2;
      HIHist2D* m_layer2left2_scat;
      HIHistProf* m_layer2left1;
      HIHist2D* m_layer2left1_scat;
      HIHistProf* m_layer2right1;
      HIHist2D* m_layer2right1_scat;
      HIHistProf* m_layer2right2;
      HIHist2D* m_layer2right2_scat;

      HIHistProf* m_layer3left3;
      HIHist2D* m_layer3left3_scat;
      HIHistProf* m_layer3left2;
      HIHist2D* m_layer3left2_scat;
      HIHistProf* m_layer3left1;
      HIHist2D* m_layer3left1_scat;
      HIHistProf* m_layer3middle;
      HIHist2D* m_layer3middle_scat;
      HIHistProf* m_layer3right1;
      HIHist2D* m_layer3right1_scat;
      HIHistProf* m_layer3right2;
      HIHist2D* m_layer3right2_scat;
      HIHistProf* m_layer3right3;
      HIHist2D* m_layer3right3_scat;

      HIHistProf* m_layer4left5;
      HIHist2D* m_layer4left5_scat;
      HIHistProf* m_layer4left4;
      HIHist2D* m_layer4left4_scat;
      HIHistProf* m_layer4left3;
      HIHist2D* m_layer4left3_scat;
      HIHistProf* m_layer4left2;
      HIHist2D* m_layer4left2_scat;
      HIHistProf* m_layer4left1;
      HIHist2D* m_layer4left1_scat;
      HIHistProf* m_layer4right1;
      HIHist2D* m_layer4right1_scat;
      HIHistProf* m_layer4right2;
      HIHist2D* m_layer4right2_scat;
      HIHistProf* m_layer4right3;
      HIHist2D* m_layer4right3_scat;
      HIHistProf* m_layer4right4;
      HIHist2D* m_layer4right4_scat;
      HIHistProf* m_layer4right5;
      HIHist2D* m_layer4right5_scat;

      HIHistProf* m_layer1left1_matched;
      HIHist2D* m_layer1left1_matched_scat;
      HIHistProf* m_layer1middle_matched;
      HIHist2D* m_layer1middle_matched_scat;
      HIHistProf* m_layer1right1_matched;
      HIHist2D* m_layer1right1_matched_scat;

      HIHistProf* m_layer2left2_matched;
      HIHist2D* m_layer2left2_matched_scat;
      HIHistProf* m_layer2left1_matched;
      HIHist2D* m_layer2left1_matched_scat;
      HIHistProf* m_layer2right1_matched;
      HIHist2D* m_layer2right1_matched_scat;
      HIHistProf* m_layer2right2_matched;
      HIHist2D* m_layer2right2_matched_scat;

      HIHistProf* m_layer3left3_matched;
      HIHist2D* m_layer3left3_matched_scat;
      HIHistProf* m_layer3left2_matched;
      HIHist2D* m_layer3left2_matched_scat;
      HIHistProf* m_layer3left1_matched;
      HIHist2D* m_layer3left1_matched_scat;
      HIHistProf* m_layer3middle_matched;
      HIHist2D* m_layer3middle_matched_scat;
      HIHistProf* m_layer3right1_matched;
      HIHist2D* m_layer3right1_matched_scat;
      HIHistProf* m_layer3right2_matched;
      HIHist2D* m_layer3right2_matched_scat;
      HIHistProf* m_layer3right3_matched;
      HIHist2D* m_layer3right3_matched_scat;

      HIHistProf* m_layer4left5_matched;
      HIHist2D* m_layer4left5_matched_scat;
      HIHistProf* m_layer4left4_matched;
      HIHist2D* m_layer4left4_matched_scat;
      HIHistProf* m_layer4left3_matched;
      HIHist2D* m_layer4left3_matched_scat;
      HIHistProf* m_layer4left2_matched;
      HIHist2D* m_layer4left2_matched_scat;
      HIHistProf* m_layer4left1_matched;
      HIHist2D* m_layer4left1_matched_scat;
      HIHistProf* m_layer4right1_matched;
      HIHist2D* m_layer4right1_matched_scat;
      HIHistProf* m_layer4right2_matched;
      HIHist2D* m_layer4right2_matched_scat;
      HIHistProf* m_layer4right3_matched;
      HIHist2D* m_layer4right3_matched_scat;
      HIHistProf* m_layer4right4_matched;
      HIHist2D* m_layer4right4_matched_scat;
      HIHistProf* m_layer4right5_matched;
      HIHist2D* m_layer4right5_matched_scat;

      // ------------ static data members ------------------------

};

// inline function definitions

class AP_rphiHit {
   public:
      AP_rphiHit();
      
      double phi;
      double trackz;
      int sensor;
      int ladder;
      int layer;
      int layerOrClam;
      double rphires;
      double rphires_err;
      double weighted_rphires;
      double hitx;      
      double hitx_err;
      double charge;
      double charge_err;
      double width;
      double pitch;
};

class AP_zHit {
   public:
      AP_zHit();

      double phi;
      double trackz;
      int sensor;
      int ladder;
      int layer;
      int layerOrClam;
      double zres;
      double zres_err;
      double weighted_zres;
      double hitz;
      double hitz_err;
      double charge;
      double charge_err;
      double width;
      double pitch;
};

#endif /* ALIGNMENTPLOTS_ALIGNMENTPLOTS_H */
