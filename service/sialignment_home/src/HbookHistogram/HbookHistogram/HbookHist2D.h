#if !defined(HBOOKHISTOGRAM_HBOOKHIST2D_H)
#define HBOOKHISTOGRAM_HBOOKHIST2D_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookHist2D
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Thu Dec  3 15:49:46 EST 1998
// $Id: HbookHist2D.h,v 1.4 2000/06/19 18:18:20 bkh Exp $
//
// Revision history
//
// $Log: HbookHist2D.h,v $
// Revision 1.4  2000/06/19 18:18:20  bkh
// Implement new pure virtual functions from HistogramInterface
//
// Revision 1.3  1999/12/23 23:34:49  cdj
// updated to new histogram interface (for arithmetic operations)
//
// Revision 1.2  1999/09/29 17:55:53  cdj
// added histogram directories
//
// Revision 1.1.1.1  1999/09/06 22:02:30  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

// system include files

// user include files
#include "HistogramInterface/HIHist2D.h"
#include "HbookHistogram/HbookDirContents.h"

// forward declarations

class HbookHist2D : public HIHist2D, private HbookDirContents
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HbookHist2D( HbookHistDir* iParent,
		     unsigned int  iId,
		     const string& iTitle,
		     unsigned int  iNChannelsX,
		     float         iLowEdgeX,
		     float         iHighEdgeX,
		     unsigned int  iNChannelsY,
		     float         iLowEdgeY,
		     float         iHighEdgeY,
		     float         iBitsPerChannel = 0 );
      
      HbookHist2D();
      virtual ~HbookHist2D();

      // ---------- member functions ---------------------------
      // Modifiers
      virtual void fill ( float iXEntry,
			  float iYEntry,
			  float iWeight = 1.0);

      virtual void scale( const float iScale = 1.0 );

      virtual void add  ( const HIHist2D& rHist1,
			  const HIHist2D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );

      virtual void sub  ( const HIHist2D& rHist1,
			  const HIHist2D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );

      virtual void mult ( const HIHist2D& rHist1,
			  const HIHist2D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );
 
      virtual void div  ( const HIHist2D& rHist1,
			  const HIHist2D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );

      virtual void copy( const HIHist2D& rOriginal );

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


      virtual void reset() ;
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookHist2D( const HbookHist2D& ); // stop default
      //HbookHist2D(); // default
      // ---------- assignment operator(s) ---------------------
      const HbookHist2D& operator=( const HbookHist2D& ); // stop default

      // ---------- private member functions -------------------
      virtual void fillChannelValue( unsigned int iChannel, float iWeight );

      // ---------- private const member functions -------------
      virtual float channelValue( unsigned int iChannel ) const;

      float hBookChannelValue( unsigned int iX, unsigned int iY ) const ;

      // ---------- data members -------------------------------
      int m_id ;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookHist2D.cc"
//#endif

#endif /* HBOOKHISTOGRAM_HIBOOKHIST2D_H */




