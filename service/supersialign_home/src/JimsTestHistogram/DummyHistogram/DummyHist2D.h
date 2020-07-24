#if !defined(PACKAGE_DUMMYHIST2D_H)
#define PACKAGE_DUMMYHIST2D_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      DummyHist2D
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 22:29:45 EST 1999
// $Id: DummyHist2D.h,v 1.3 2000/06/19 18:47:03 bkh Exp $
//
// Revision history
//
// $Log: DummyHist2D.h,v $
// Revision 1.3  2000/06/19 18:47:03  bkh
// Implement new virtual fcns
//
// Revision 1.2  1999/12/23 23:30:12  cdj
// updated to new histogram interface
//
// Revision 1.1.1.1  1999/05/12 00:10:11  billm
// imported DummyHistogram sources
//

// system include files

// user include files
#include "HistogramInterface/HIHist2D.h"

// forward declarations

class DummyHist2D : public HIHist2D
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DummyHist2D(
		     unsigned int  iId,
		     const string& iTitle,
		     unsigned int  iNChannelsX,
		     float         iLowEdgeX,
		     float         iHighEdgeX,
		     unsigned int  iNChannelsY,
		     float         iLowEdgeY,
		     float         iHighEdgeY,
		     float         iBitsPerChannel = 0 );
	     
      virtual ~DummyHist2D();

      // ---------- member functions ---------------------------
      virtual void fill ( float iXEntry,
			  float iYEntry,
			  float iWeight = 1.0);


      virtual void scale( const float iScale = 1.0 );
   
      virtual void add  ( const HIHist2D&,
			  const HIHist2D&,
			  const float = 1.0,
			  const float = 1.0 );

      virtual void sub  ( const HIHist2D&,
			  const HIHist2D&,
			  const float = 1.0,
			  const float = 1.0 );

      virtual void mult ( const HIHist2D&,
			  const HIHist2D&,
			  const float = 1.0,
			  const float = 1.0 );

      virtual void div  ( const HIHist2D&,
			  const HIHist2D&,
			  const float = 1.0,
			  const float = 1.0 );

      virtual void copy( const HIHist2D& rOriginal );


      virtual void reset() ;

      // ---------- const member functions ---------------------
      virtual float channelValue( unsigned int iChannel ) const;

      virtual float sumWithinLimits() const ; 
      virtual float maxValue()        const ;
      virtual float minValue()        const ;

      virtual float underXoverY()    const ;
      virtual float insideXoverY()   const ;
      virtual float overXoverY()     const ;
      virtual float underXinsideY()  const ;
      virtual float overXinsideY()   const ;
      virtual float underXunderY()   const ;
      virtual float insideXunderY()  const ;
      virtual float overXunderY()    const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DummyHist2D( const DummyHist2D& ); // stop default
      DummyHist2D(); // default

      // ---------- assignment operator(s) ---------------------
      const DummyHist2D& operator=( const DummyHist2D& ); // stop default

      // ---------- private member functions -------------------
      virtual void fillChannelValue( unsigned int iChannel, float iWeight );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/DummyHist2D.cc"
//#endif

#endif /* PACKAGE_DUMMYHIST2D_H */
