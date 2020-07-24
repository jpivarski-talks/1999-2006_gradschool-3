#if !defined(PACKAGE_DUMMYHISTPROF_H)
#define PACKAGE_DUMMYHISTPROF_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      DummyHistProf
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Bill McCloskey
// Created:     Sat Mar  6 22:30:18 EST 1999
// $Id: DummyHistProf.h,v 1.3 2001/08/03 17:39:30 bkh Exp $
//
// Revision history
//
// $Log: DummyHistProf.h,v $
// Revision 1.3  2001/08/03 17:39:30  bkh
// Declaration of pure virtual base class function with dummy
//
// Revision 1.2  2000/06/19 18:47:03  bkh
// Implement new virtual fcns
//
// Revision 1.1.1.1  1999/05/12 00:10:11  billm
// imported DummyHistogram sources
//

// system include files

// user include files
#include "HistogramInterface/HIHistProf.h"

#include <vector>

// forward declarations

class DummyHistProf : public HIHistProf
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DummyHistProf(
     		     unsigned int   iId,
		     const string&  iTitle,
		     unsigned int   iNChannels,
		     float          iLowEdgeX,
		     float          iHighEdgeX,
		     float          iLowEdgeY,
		     float          iHighEdgeY,
		     HIHistProf::DisplayErrorsAs iDisplayErrorsAs );

      virtual ~DummyHistProf();

      // ---------- member functions ---------------------------
      virtual void fill ( float iXEntry,
			  float iYEntry,
			  float iWeight = 1. ) ;

      virtual void add  ( const HIHistProf&,
			  const HIHistProf&) ;

      virtual void sub  ( const HIHistProf&,
			  const HIHistProf&) ;

      virtual void mult ( const HIHistProf&,
			  const HIHistProf&) ;

      virtual void div  ( const HIHistProf&,
			  const HIHistProf&) ;
      
      virtual void reset() ;

      // ---------- const member functions ---------------------

      virtual float sumWithinLimits() const ; 
      virtual float maxValue()        const ;
      virtual float minValue()        const ;

      virtual float channelValue( unsigned int iChannel ) const;
      virtual float channelError( unsigned int iChannel ) const; 

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DummyHistProf( const DummyHistProf& ); // stop default
      DummyHistProf(); // default

      // ---------- assignment operator(s) ---------------------
      const DummyHistProf& operator=( const DummyHistProf& ); // stop default

      // ---------- private member functions -------------------
      virtual void fillChannelValue( unsigned int iChannel, float iYEntry, float iWeight );

      virtual DABoolean domainsMatch( const HIHistProf& first,
				      const HIHistProf& second );

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      vector< unsigned int > m_population;
      vector< double > m_numerator;
      vector< double > m_denominator;
      
      double m_underflow;
      double m_overflow;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/DummyHistProf.cc"
//#endif

#endif /* PACKAGE_DUMMYHISTPROF_H */
