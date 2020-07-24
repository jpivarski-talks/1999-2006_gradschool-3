// -*- C++ -*-
#if !defined(DUALTRACKTOUSAGETAGPROD_DUALTRACKTOUSAGETAGPROD_H)
#define DUALTRACKTOUSAGETAGPROD_DUALTRACKTOUSAGETAGPROD_H
//
// Package:     <DualTrackToUsageTagProd>
// Module:      DualTrackToUsageTagProd
//
/**\class DualTrackToUsageTagProd DualTrackToUsageTagProd.h DualTrackToUsageTagProd/DualTrackToUsageTagProd.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim Pivarski
// Created:     Mon Dec 15 14:41:35 EST 2003
// $Id: DualTrackToUsageTagProd.h,v 1.1.1.1 2003/12/17 21:00:53 mccann Exp $
//
// Revision history
//
// $Log: DualTrackToUsageTagProd.h,v $
// Revision 1.1.1.1  2003/12/17 21:00:53  mccann
// imported DualTrackToUsageTagProd sources
//
//

// system include files

// user include files
#include "Processor/Producer.h"
#include "HistogramInterface/HistogramPackage.h"

// forward declarations

class DualTrackToUsageTagProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      DualTrackToUsageTagProd( void );                      // anal1 
      virtual ~DualTrackToUsageTagProd();                   // anal5 

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
      DualTrackToUsageTagProd( const DualTrackToUsageTagProd& );

      // ------------ assignment operator(s) ----------------------
      const DualTrackToUsageTagProd& operator=( const DualTrackToUsageTagProd& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* DUALTRACKTOUSAGETAGPROD_DUALTRACKTOUSAGETAGPROD_H */
