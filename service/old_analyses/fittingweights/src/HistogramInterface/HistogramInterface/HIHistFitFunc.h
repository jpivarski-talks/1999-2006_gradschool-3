#if !defined(HISTOGRAMINTERFACE_HIHISTFITFUNC_H)
#define HISTOGRAMINTERFACE_HIHISTFITFUNC_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistFitFunc
// 
/**\class HIHistFitFunc HIHistFitFunc.h HistogramInterface/HIHistFitFunc.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Jim McCann
// Created:     Fri Oct 19 15:55:30 EDT 2001
// $Id$
//
// Revision history
//
// $Log$

// system include files

// user include files

#include "HistogramInterface/HIHist.h"
#include "HistogramInterface/HIHist1D.h"
#include "HistogramInterface/HIHistProf.h"
#include "MinuitInterface/MinuitInterface.h"
#include "MinuitInterface/MIFcn.h"

#include <assert.h>

// forward declarations

class HIHistFitFunc : public MIFcn
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef enum {
	 kHist1D,
	 kHistProf,
	 kNumAllowedTypes
      } HistogramType;

      // ---------- Constructors and destructor ----------------

      // This copies all the relevant fields, so you don't need to
      // make the array arguments to this function persist in memory
      HIHistFitFunc( HIHist* histogram, HistogramType type,
		     unsigned int numParams,
		     double* paramsStart, double* paramsStep,
		     DABoolean hasLimits = false,
		     double* paramsLimitsLow = NULL,
		     double* paramsLimitsHigh = NULL,
		     DABoolean useMinos = false );
      virtual ~HIHistFitFunc();

      // ---------- member functions ---------------------------

      // This is the one you need to overwrite in the subclass
      virtual double func( double x, double* params ) = 0;
      
      // This will print out the parameters to an ostream
      virtual void print( ostream& os );

      // This one just calls the above with the final parameter values
      double func( double x );

      // returns true if the fit was successful (error matrix accurate)
      DABoolean fit();

      // returns the associated histogram
      HIHist* histogram();

      // returns its type, so that you know how to cast the pointer
      HistogramType histogramType();

      // you may want to know this. I want to know this
      DABoolean histogram_empty();

      // returns the number of parameters that this function accepts
      // (assertions happen if you don't respect this limit)
      unsigned int numParams();

      // returns a MinuitInterface parameter, see that documentation
      // for extracting fit values and uncertainties
      MIParameter parameter( unsigned int paramNumber );

      // you can change the starting values
      void setParamStart( double* start );
      void setParamStartToFitted();

      // you can change the step values
      void setParamStep( double* step );

      // returns true if you made this a function with limits on the
      // parameters
      DABoolean hasLimits();

      // allows you to reset one of the limits you placed on a
      // parameter (asserts if this is not a function that has limits
      // on its parameters)
      void setParamLimits( unsigned int paramNumber, double low, double high );

      // returns the leftmost histogram index used in the fit
      unsigned int domainIndexStart();

      // returns the rightmost histogram index used in the fit
      unsigned int domainIndexEnd();

      // returns the left border of the domain subinterval used in the fit
      double domainXStart();

      // returns the right border of the domain subinterval used in the fit
      double domainXEnd();

      // allows you to reset the subinterval used in the fit to the
      // full domain (this is the default)
      // (equivalent to setDomain( 1, -1 )
      void setDomainFull();

      // allows you to set the subinterval used in the fit to
      // [indexStart, indexEnd]. If either of these is negative, the
      // number of channels is counted from the right rather than from
      // the left. If, after all of this, indexEnd < indexStart, the
      // function will assert.
      void setDomain( int indexStart, int indexEnd );

      // allows you to set the subinterval used in the fit to
      // [xStart, xEnd] (snaps to the nearest histogram channel)
      void setDomain( double xStart, double xEnd );

      // returns true if the function is set to use Minos
      DABoolean useMinos();

      // allows you to set the function to use Minos or not
      void setUseMinos( DABoolean useMinos );

      // returns the covarianceMatrix of the fit (passed from
      // MinuitInterface)
      HepMatrix covarianceMatrix();

      // returns a MinuitInterface statistics object for the fit
      MIStats minuitStats();

      // returns the final chisq value
      double chisq();

      // returns the number of histogram channels used in the fit (can
      // be less than the number of channels if there are any unfilled
      // bins)
      unsigned int numPointsUsed();

      // returns the number of degrees of freedom = numPointsUsed -
      // numParams, forced to be non-negative
      unsigned int degreesOfFreedom();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      HIHistFitFunc();

      double iterate( double* values );

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIHistFitFunc( const HIHistFitFunc& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIHistFitFunc& operator=( const HIHistFitFunc& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      HIHist* m_histogram;
      unsigned int m_numberOfChannels;
      HistogramType m_type;
      DABoolean m_hasLimits;
      DABoolean m_useMinos;
      unsigned int m_numPointsUsed;
      unsigned int m_domainStart;
      unsigned int m_domainEnd;

      unsigned int m_numParams;
      double* m_paramsStart;
      double* m_paramsStep;
      double* m_paramsLimitsLow;
      double* m_paramsLimitsHigh;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HIHistFitFunc.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHISTFITFUNC_H */
