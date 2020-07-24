#if !defined(HISTOGRAMINTERFACE_HIHISTFITLINE_H)
#define HISTOGRAMINTERFACE_HIHISTFITLINE_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistFitLine
// 
/**\class HIHistFitLine HIHistFitLine.h HistogramInterface/HIHistFitLine.h

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

class HIHistFitLine : public HIHistFitFunc
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      enum {
	 kConstant,
	 kSlope,
	 kNumParams
      };

      // ---------- Constructors and destructor ----------------

      HIHistFitLine( HIHist* histogram, HistogramType,
		     double* paramsStart, double* paramsStep,
		     DABoolean hasLimits = false,
		     double* paramsLimitsLow = NULL,
		     double* paramsLimitsHigh = NULL,
		     DABoolean useMinos = false );
      
      // ---------- member functions ---------------------------

      double constant();
      double slope();

      double func( double x, double* params );

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      HIHistFitLine();
      virtual ~HIHistFitLine();

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIHistFitLine( const HIHistFitLine& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIHistFitLine& operator=( const HIHistFitLine& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HIHistFitLine.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHISTFITLINE_H */
