#if !defined(HBOOKHISTOGRAM_HBOOKHIST1D_H)
#define HBOOKHISTOGRAM_HBOOKHIST1D_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      HbookHist1D
// 
// Description: This class inherits from HIHist1D and contains
//              the methods for a 1D Hbook histogram.
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Thu Dec  3 15:49:40 EST 1998
// $Id: HbookHist1D.h,v 1.5 2000/06/19 18:18:19 bkh Exp $
//
// Revision history
//
// $Log: HbookHist1D.h,v $
// Revision 1.5  2000/06/19 18:18:19  bkh
// Implement new pure virtual functions from HistogramInterface
//
// Revision 1.4  2000/03/11 17:49:53  cdj
// HbookHist1D.h
//
// Revision 1.3  1999/12/23 23:34:49  cdj
// updated to new histogram interface (for arithmetic operations)
//
// Revision 1.2  1999/09/29 17:55:52  cdj
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
#include "Experiment/fortran_types.h"
#include "HistogramInterface/HIHist1D.h"
#include "HbookHistogram/HbookDirContents.h"

// forward declarations

class HbookHist1D : public HIHist1D, private HbookDirContents
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
  
      // ---------- Constructors and destructor ----------------

      HbookHist1D( HbookHistDir* iParent,
		   unsigned  int iId,
		   const string& iTitle,
		   unsigned int  iNChannels,
		   float         iLowEdge,
		   float         iHighEdge,
		   float         iBitsPerChannel = 0 );
      
      HbookHist1D();
      virtual ~HbookHist1D();

      // ---------- member functions ---------------------------
      // Modifiers

      virtual void fill ( float iEntry,
			  float iWeight = 1.0 );

      virtual void scale( const float iScale = 1.0 );

      virtual void add  ( const HIHist1D& rHist1,
			  const HIHist1D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );

      virtual void sub  ( const HIHist1D& rHist1,
			  const HIHist1D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );
 
      virtual void mult ( const HIHist1D& rHist1,
			  const HIHist1D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );

      virtual void div  ( const HIHist1D& rHist1,
			  const HIHist1D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );      

      virtual void copy( const HIHist1D& rOriginal );

      virtual void setErrors( const float* iErrorArray );
      
      virtual void reset() ;
      // ---------- const member functions ---------------------
      virtual float channelValue( unsigned int iChannel ) const;
      virtual float channelError( unsigned int iChannel ) const; 

      virtual float sumWithinLimits() const ; 
      virtual float maxValue()        const ;
      virtual float minValue()        const ;
 
      virtual float underflow() const ;
      virtual float overflow()  const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HbookHist1D( const HbookHist1D& ); // stop default
      //HbookHist1D(); // default 
      // ---------- assignment operator(s) ---------------------
      const HbookHist1D& operator=( const HbookHist1D& ); // stop default

      // ---------- private member functions -------------------
      virtual void fillChannelValue( unsigned int iChannel, float iWeight );
      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      FInteger m_histAddress;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HbookHistogram/Template/HbookHist1D.cc"
//#endif

#endif /* HBOOKHISTOGRAM_HIBOOKHIST1D_H */



