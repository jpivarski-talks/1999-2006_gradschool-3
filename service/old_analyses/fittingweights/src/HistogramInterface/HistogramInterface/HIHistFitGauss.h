#if !defined(HISTOGRAMINTERFACE_HIHISTFITGAUSS_H)
#define HISTOGRAMINTERFACE_HIHISTFITGAUSS_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistFitGauss
// 
/**\class HIHistFitGauss HIHistFitGauss.h HistogramInterface/HIHistFitGauss.h

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

class HIHistFitGauss : public HIHistFitFunc
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum {
	 kArea,
	 kMean,
	 kSigma,
	 kNumParams
      };

      // ---------- Constructors and destructor ----------------

      HIHistFitGauss( HIHist* histogram, HistogramType type,
		      double* paramsStart, double* paramsStep,
		      DABoolean hasLimits = false,
		      double* paramsLimitsLow = NULL,
		      double* paramsLimitsHigh = NULL,
		      DABoolean useMinos = false );

      // ---------- member functions ---------------------------

      double area();
      double mean();
      double sigma();

      double func( double x, double* params );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      HIHistFitGauss();
      virtual ~HIHistFitGauss();

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIHistFitGauss( const HIHistFitGauss& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIHistFitGauss& operator=( const HIHistFitGauss& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HIHistFitGauss.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHISTFITGAUSS_H */
