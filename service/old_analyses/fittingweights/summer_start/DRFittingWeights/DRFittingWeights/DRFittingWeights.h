// -*- C++ -*-
#if !defined(DRFITTINGWEIGHTS_DRFITTINGWEIGHTS_H)
#define DRFITTINGWEIGHTS_DRFITTINGWEIGHTS_H
//
// Package:     DRFittingWeights
// Module:      DRFittingWeights
//
/**\class DRFittingWeights DRFittingWeights.h DRFittingWeights/DRFittingWeights.h
 
 Description: Suez Module that allows you to control the "event" loop

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Mon Jul 30 11:00:22 EDT 2001
// $Id$
//
// Revision history
//
// $Log$
//

// system include files

// user include files
#include "CommandPattern/Module.h"
#include "FrameIterate/FrameIteratorModuleBase.h"
#include "FrameIterate/FIHolder.h"
//The data object you wish to change on each iteration

#include "CLEOConstantsModifiable/CLEOConstantsModifiable.h"
#include "BDLDRclient/DBDRWeightDrift.hh"
#include "BDLDRclient/DBDRWeightTheta.hh"
//  #include "BDLDRclient/DBDRWeightAlpha.hh"
//  #include "BDLDRclient/DBDRWeightPhi.hh"
//  #include "BDLDRclient/DBDRWeightLayer.hh"

#include "CommandPattern/Parameter.h"
#include "MinuitInterface/MinuitInterface.h"
#include "MinuitInterface/MIFcn.h"

#include "CLHEP/Vector/ThreeVector.h"

#include <fstream.h>
#include <iostream.h>

// forward declarations

class DRFittingWeights : public FrameIteratorModuleBase
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum {
	 kIterations = 6;
	 kBinFull = 10000;
      };

      // ---------- Constructors and destructor ----------------
      DRFittingWeights();
      virtual ~DRFittingWeights();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      ///override this function to do the actual iterations
      virtual void iterate( const FIFrameIterator& iBegin,
			    const FIFrameIterator& iEnd );

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DRFittingWeights( const DRFittingWeights& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DRFittingWeights& operator=( const DRFittingWeights& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      CLEOConstantsModifiable< DBDRWeightDrift > m_drift;
      CLEOConstantsModifiable< DBDRWeightTheta > m_theta;
//       CLEOConstantsModifiable< DBDRWeightAlpha > m_alpha;
//       CLEOConstantsModifiable< DBDRWeightPhi >   m_phi;
//       CLEOConstantsModifiable< DBDRWeightLayer > m_layer;

      FIHolder< CLEOConstantsModifiable< DBDRWeightDrift > > m_driftHolder;
      FIHolder< CLEOConstantsModifiable< DBDRWeightTheta > > m_thetaHolder;
//        FIHolder< CLEOConstantsModifiable< DBDRWeightAlpha > > m_alphaHolder;
//        FIHolder< CLEOConstantsModifiable< DBDRWeightPhi > >   m_phiHolder;
//        FIHolder< CLEOConstantsModifiable< DBDRWeightLayer > > m_layerHolder;

      // ---------- static data members ------------------------

      // ---------- nested classes -----------------------------

   public:
      
      class bin : public MIFcn
      {
	 public:
	    typedef enum {
	       kDrift = 1,
	       kTheta = 2,
	       kAlpha = 3,
	       kPhi   = 4,
	       kLayer = 5
	    } BinType;
	    
	    double iterate( double* values );
	    void take( double dca, double drift,
		       double e_dca, double e_drift );
	    void fill_after( double fw );

	 private:
	    void setup( HIHistoManager* hm,
			BinType myType,
			unsigned int binNum );

	    int index;
	    double l_residual[ DRFittingWeights::kBinFull ];
	    double l_residual2[ DRFittingWeights::kBinFull ];
	    double l_e_dca2[ DRFittingWeights::kBinFull ];
	    double l_e_drift2[ DRFittingWeights::kBinFull ];

	    HIHist1D* h_dca;
	    HIHist1D* h_drift;
	    HIHist1D* h_e_dca;
	    HIHist1D* h_e_drift;
	    HIHist1D* h_residual;
	    HIHist1D* h_e_residual;
	    HIHist1D* h_norm_resid;
	    HIHist1D* h_norm_resid_after;
      };

      class drift : public bin
      {
	 public:
	    drift( HIHistoManager* hm, unsigned int binNum,
		   unsigned int layer, double leftNdrift, double rightNdrift )
	    {
	       setup( hm, kDrift, binNum );
	       myLayer = layer;
	       myLeftNdrift = leftNdrift;
	       myRightNdrift = rightNdrift;
	    }

	    bool owns( unsigned int layer, double ndrift )
	    {
	       return ( layer == myLayer         &&
			myLeftNdrift <= ndrift   &&
			ndrift < myRightNdrift       );
	    }
	    
	 protected:
	    drift() { ; }

	 public:
	    unsigned int myLayer;
	    double myLeftNdrift;
	    double myRightNdrift;
      }
};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "DRFittingWeights/Template/DRFittingWeights.cc"
//#endif

#endif /* DRFITTINGWEIGHTS_DRFITTINGWEIGHTS_H */
