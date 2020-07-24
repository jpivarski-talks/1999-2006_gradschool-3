#if !defined(NAVIGATIONPROD_NAVIGATIONPROD_H)
#define NAVIGATIONPROD_NAVIGATIONPROD_H
// -*- C++ -*-
//
// Package:     NavigationProd
// Module:      NavigationProd
// 
// Description: Producer providing proxies for navigation objects (get it?).
//
// Usage:
//    <usage>
//
// Author:      Michael Marsh
// Created:     Mon Jul  6 14:20:22 EDT 1998
// $Id: NavigationProd.h,v 1.2 2000/09/26 21:28:45 lkg Exp $
//
// Revision history
//
// $Log: NavigationProd.h,v $
// Revision 1.2  2000/09/26 21:28:45  lkg
// An partial implementation of specification of seed track and fit track
// usage and production tags.  For track-only use, the implementation is
// complete.  No effort has been made to enforce the same track tags
// for clusters/rich/muons/etc (or to allow specification of
// prod/usage tag information for that information).
// * add parameters to NavigationProd for specification of the finder and
// fitter usage and production tags
// * add accessor functions to NavigationProd to access the tags
// * pass a pointer to (the calling) NavigationProd to the NavTrackProxy
// constructor, so it may access the tag information
// * create the NavTrack master block using the prod/usage tag information
//
// Revision 1.1.1.1  1999/07/19 16:47:45  bkh
// imported NavigationProd sources
//
// Revision 1.2  1998/12/19 21:22:38  marsh
// Removed override of registerProxies.
//
// Revision 1.1.1.1  1998/07/13 23:26:20  marsh
// New library for user-level analysis objects
//
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "TrackDelivery/TDTrackMaster.h"
#include "CommandPattern/Parameter.h"

//#include "ToolBox/HistogramPackage.h"

// forward declarations

class NavigationProd : public Producer
{
      // ------------ friend classses and functions ---------------

   public:
      // ------------ constants, enums and typedefs ---------------

      // ------------ Constructors and destructor -----------------
      NavigationProd( void );                      // anal1 
      virtual ~NavigationProd();                   // anal5 

      ProxyBase* makeNavTrackProxy();

      // ------------ member functions ----------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
//      virtual void hist_book( TBHistoManager& );                  

      // ------------ const member functions ----------------------
      TDTrackMaster::TagPair fitterTags() const;
      TDTrackMaster::TagPair finderTags() const;

      // ------------ static member functions ---------------------

   protected:
      // ------------ protected member functions ------------------

      // ------------ protected const member functions ------------

   private:
      // ------------ Constructors and destructor -----------------
      NavigationProd( const NavigationProd& );

      // ------------ assignment operator(s) ----------------------
      const NavigationProd& operator=( const NavigationProd& );

      // ------------ private member functions --------------------

      // ------------ private const member functions --------------

      // ------------ data members --------------------------------
      Parameter<string> m_fitterUsageTag;
      Parameter<string> m_fitterProductionTag;
      Parameter<string> m_finderUsageTag;
      Parameter<string> m_finderProductionTag;

      // ------------ static data members -------------------------

};

// inline function definitions

#endif /* NAVIGATIONPROD_NAVIGATIONPROD_H */
