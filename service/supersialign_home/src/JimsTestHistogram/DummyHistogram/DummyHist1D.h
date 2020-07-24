#if !defined(PACKAGE_DUMMYHIST1D_H)
#define PACKAGE_DUMMYHIST1D_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      DummyHist1D
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 22:29:31 EST 1999
// $Id: DummyHist1D.h,v 1.3 2000/06/19 18:47:02 bkh Exp $
//
// Revision history
//
// $Log: DummyHist1D.h,v $
// Revision 1.3  2000/06/19 18:47:02  bkh
// Implement new virtual fcns
//
// Revision 1.2  1999/12/23 23:30:11  cdj
// updated to new histogram interface
//
// Revision 1.1.1.1  1999/05/12 00:10:11  billm
// imported DummyHistogram sources
//

// system include files

// user include files
#include "HistogramInterface/HIHist1D.h"

#include <vector>

// forward declarations

class DummyHist1D : public HIHist1D
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DummyHist1D(
                     unsigned int  iId,
		     const string& iTitle,
		     unsigned int  iNChannels,
		     float         iLowEdge,
		     float         iHighEdge,
		     float         iBitsPerChannel = 0 );
      virtual ~DummyHist1D();

      // ---------- member functions ---------------------------
      virtual void fill ( float iEntry,
			  float iWeight = 1.0 );

      virtual void scale( const float iScale = 1.0 );

      virtual void add  ( const HIHist1D& iHist1,
			  const HIHist1D& iHist2, 
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );

      virtual void sub  ( const HIHist1D& iHist1,
			  const HIHist1D& iHist2, 
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );
 
      virtual void mult ( const HIHist1D& iHist1,
			  const HIHist1D& iHist2, 
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );

      virtual void div  ( const HIHist1D& iHist1,
			  const HIHist1D& iHist2, 
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 );
      
      virtual void copy( const HIHist1D& rOriginal );

      virtual void reset() ;

      virtual void setErrors( const float* iErrorArray ) ;

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
      DummyHist1D( const DummyHist1D& ); // stop default
      DummyHist1D(); // default

      // ---------- assignment operator(s) ---------------------
      const DummyHist1D& operator=( const DummyHist1D& ); // stop default

      // ---------- private member functions -------------------
      virtual void fillChannelValue( unsigned int iChannel, float iWeight );

      virtual DABoolean domainsMatch( const HIHist1D& first,
				      const HIHist1D& second );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      vector< double > m_data;
      vector< double > m_errors;

      double m_underflow;
      double m_overflow;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/DummyHist1D.cc"
//#endif

#endif /* PACKAGE_DUMMYHIST1D_H */
