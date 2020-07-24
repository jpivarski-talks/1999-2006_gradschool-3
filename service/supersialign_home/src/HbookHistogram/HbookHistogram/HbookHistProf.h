#if !defined(HBOOKHISTOGRAM_HBOOKHISTPROF_H)
#define HBOOKHISTOGRAM_HBOOKHISTPROF_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookHistProf
// 
// Description: HbookHistProf inherits from HIHistProf and 
//              contains the methods for a Hbook profile plot.  
//              
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Thu Dec  3 15:47:32 EST 1998
// $Id: HbookHistProf.h,v 1.6 2002/05/06 17:47:42 mccann Exp $
//
// Revision history
//
// $Log: HbookHistProf.h,v $
// Revision 1.6  2002/05/06 17:47:42  mccann
// added weighted profiles and the HIHistProf option kWeightedError
//
// Revision 1.5  2001/08/03 17:29:28  bkh
// Implement value and error fuctionality
//
// Revision 1.4  2000/06/19 18:18:20  bkh
// Implement new pure virtual functions from HistogramInterface
//
// Revision 1.3  1999/12/23 23:34:50  cdj
// updated to new histogram interface (for arithmetic operations)
//
// Revision 1.2  1999/09/29 17:55:55  cdj
// added histogram directories
//
// Revision 1.1.1.1  1999/09/06 22:02:31  mkl
// imported HbookHistogram source
//
// Revision 1.1.1.1  1999/02/23 22:00:59  cdj
// imported HbookHistogram source
//

// system include files

// user include files
#include "HistogramInterface/HIHistProf.h"
#include "HbookHistogram/HbookDirContents.h"

// forward declarations

class HbookHistProf : public HIHistProf, private HbookDirContents

{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // See HistogramInterface/HIHistProf.h for an explaination of
      // kWeightedError. It's implemented here with a 1D histogram, so
      // don't be suprised if you find a 1D histo in your .rzn file
      // that you thought you declared to be a profile.

      enum DisplayErrorsAs { kSpread, kErrorOnMean, kWeightedError };

      // ---------- Constructors and destructor ----------------
      HbookHistProf( HbookHistDir* iParent,
		     unsigned int   iId,
		     const string&  iTitle,
		     unsigned int   iNChannels,
		     float iLowEdgeX,
		     float iHighEdgeX,
		     float iLowEdgeY,
		     float iHighEdgeY,
		     HIHistProf::DisplayErrorsAs iDisplayErrorsAs );

      HbookHistProf();
      virtual ~HbookHistProf();

      // ---------- member functions ---------------------------
      // Modifiers
      virtual void fill ( float iXEntry,
			  float iYEntry,
			  float iWeight = 1. ) ;

      virtual float sumWithinLimits() const ; 
      virtual float maxValue()        const ;
      virtual float minValue()        const ;

      virtual float channelValue( unsigned int iChannel ) const;
      virtual float channelError( unsigned int iChannel ) const; 

/*    LEAVE THIS STUFF OUT UNLESS NEEDED
      virtual void scale( const float iScale = 1.0 );

      virtual void add  ( const HIHistProf& rProf1,
			  const HIHistProf& rProf2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 ) ;

      virtual void sub  ( const HIHistProf& rProf1,
			  const HIHistProf& rProf2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 ) ;

      virtual void mult ( const HIHistProf& rProf1,
			  const HIHistProf& rProf2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 ) ;

      virtual void div  ( const HIHistProf& rProf1,
			  const HIHistProf& rProf2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 ) ;

      virtual void copy( const HIHistProf& rOriginal );
*/      
      virtual void reset() ;

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------
   protected:
      // ---------- protected member functions -----------------

      const void updateAssociatedHistogram();

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookHistProf( const HbookHistProf& ); // stop default
      //HbookHistProf(); // default
      // ---------- assignment operator(s) ---------------------
      const HbookHistProf& operator=( const HbookHistProf& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      int m_id ;
      double* m_numerator;
      double* m_denominator;
      DABoolean m_hbookUpdated;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookHistProf.cc"
//#endif

#endif /* HBOOKHISTOGRAM_HIBOOKHISTPROF_H */
