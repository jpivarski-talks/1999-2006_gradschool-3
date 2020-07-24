#if !defined(HISTOGRAMINTERFACE_HIHISTFITGAUSS_H)
#define HISTOGRAMINTERFACE_HIHISTFITGAUSS_H
// -*- C++ -*-
//
// Package:     <MuTracking>
// Module:      MomentumFitFunc
// 
/**\class MomentumFitFunc MomentumFitFunc.h MuTracking/MomentumFitFunc.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Sat Oct 20 22:08:25 EDT 2001
// $Id$
//
// Revision history
//
// $Log$

// system include files

// user include files
#include "HistogramInterface/HIHistFitFunc.h"

// forward declarations

class MomentumFitFunc : public HIHistFitFunc
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum {
	 kArea1,
	 kMean1,
	 kSigma1,
	 kArea2,
	 kMean2,
	 kSigma2,
	 kNumParams
      };

      // ---------- Constructors and destructor ----------------

      MomentumFitFunc( HIHist* histogram, HistogramType type,
		      double* paramsStart, double* paramsStep,
		      DABoolean hasLimits = false,
		      double* paramsLimitsLow = NULL,
		      double* paramsLimitsHigh = NULL,
		      DABoolean useMinos = false );

      // ---------- member functions ---------------------------

      double func( double x, double* params );
      void print( ostream& os );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      MomentumFitFunc();
      virtual ~MomentumFitFunc();

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      MomentumFitFunc( const MomentumFitFunc& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const MomentumFitFunc& operator=( const MomentumFitFunc& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "MuTracking/Template/MomentumFitFunc.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHISTFITGAUSS_H */
