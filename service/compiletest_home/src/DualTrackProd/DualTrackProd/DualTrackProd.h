// -*- C++ -*-
#if !defined(DUALTRACKPROD_DUALTRACKPROD_H)
#define DUALTRACKPROD_DUALTRACKPROD_H
//
// Package:     <DualTrackProd>
// Module:      DualTrackProd
//
/**\class DualTrackProd DualTrackProd.h DualTrackProd/DualTrackProd.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Wed Dec 10 11:04:40 EST 2003
// $Id: DualTrackProd.h,v 1.3 2003/12/16 22:16:27 mccann Exp $
//
// Revision history
//
// $Log: DualTrackProd.h,v $
// Revision 1.3  2003/12/16 22:16:27  mccann
// Turned on ZD lattices and fixed a Linux compilation bug.
//
// Revision 1.2  2003/12/15 17:51:43  mccann
// adding hit lattices for dual-constrained residuals
//
// Revision 1.1.1.1  2003/12/11 17:14:27  mccann
// imported DualTrackProd sources
//
//

// system include files

// user include files
#include "DualTrackProd/DualTrackProxy.h"
#include "DualTrackProd/DualTrackFitHelixProxy.h"
#include "DualTrackProd/DualTrackDRHitLatticeProxy.h"
#include "DualTrackProd/DualTrackZDHitLatticeProxy.h"
#include "Processor/Producer.h"
#include "HistogramInterface/HistogramPackage.h"
#include "CommandPattern/Parameter.h"
#include "CleoDB/DBCandidate.h"

// forward declarations

class DualTrackProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      DualTrackProd( void );                      // anal1 
      virtual ~DualTrackProd();                   // anal5 

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  
      
      // ------------ const member functions ----------------------

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      DualTrackProd( const DualTrackProd& );

      // ------------ assignment operator(s) ----------------------
      const DualTrackProd& operator=( const DualTrackProd& );

      // ------------ private member functions --------------------
      DBCandidate::Hypo selectedHypo();

      ProxyBase* makeDualTrackProxy();
      ProxyBase* makeDualTrackDRHitLatticeProxy();
      ProxyBase* makeDualTrackZDHitLatticeProxy();

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      Parameter<string> m_massHypothesis;
      Parameter<double> m_virtualPhotonPx;
      Parameter<double> m_virtualPhotonPy;
      Parameter<double> m_virtualPhotonPz;
      Parameter<DABoolean> m_pointConstraint;
      Parameter<DABoolean> m_ptConstraint;
      Parameter<DABoolean> m_pzConstraint;
      Parameter<DABoolean> m_printOutP;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* DUALTRACKPROD_DUALTRACKPROD_H */
