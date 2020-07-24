#if !defined(HISTOGRAMINTERFACE_HIHISTPROF_H)
#define HISTOGRAMINTERFACE_HIHISTPROF_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHistProf
// 
// Description: This class inherits from the base class HIHist,
//              and serves as the base class for any profile plot.
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Fri Oct 30 10:09:05 EST 1998
// $Id: HIHistProf.h,v 1.5 2002/05/06 17:46:50 mccann Exp $
//
// Revision history
//
// $Log: HIHistProf.h,v $
// Revision 1.5  2002/05/06 17:46:50  mccann
// added weighted profiles and the HIHistProf option kWeightedError
//
// Revision 1.4  2001/08/03 17:26:54  bkh
// Put in virtual channelLoEdge, channelCenter; *AND*
// Pure virtual channelValue and channelError
//
// Revision 1.3  1999/12/23 23:25:27  cdj
// added arithmetic operations on histograms
//
// Revision 1.2  1999/09/07 18:53:30  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:04  cdj
// first submission
//

// system include files
#include <string>
// user include files
#include "HIHist.h"
// forward declarations

class HIHistProf : public HIHist
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // kWeightedError means that not only will the central value of
      // the bin come from a *weighted* mean of the YEntries, but the
      // uncertainty in the bin value comes entirely from the weights:
      //
      //              sum( y_i * w_i )               /      1       \
      // bin value = --------------------  +/-  sqrt( -------------- )
      //                 sum( w_i )                  \  sum( w_i )  /
      //
      // it is assumed that w_i == 1 / sigma_i^2
      //
      // kSpread and kErrorOnMean take advantage of RMS information
      enum DisplayErrorsAs {kSpread, kErrorOnMean, kWeightedError } ;

      // ---------- Constructors and destructor ----------------
      HIHistProf( 
		  Count  iId,
		  const         string& iTitle,
		  unsigned int  iNChannels,
		  float         iLowEdgeX,
		  float         iHighEdgeX,
		  float         iLowEdgeY,
		  float         iHighEdgeY,
		  DisplayErrorsAs   iDisplayErrorsAs ) ;

      virtual ~HIHistProf();
      // ---------- member functions ---------------------------
      // Modifiers
      virtual void fill ( float iXEntry,
			  float iYEntry,
			  float iWeight = 1. ) =0;

/*
      virtual void scale( const float iScale = 1.0 ) =0;

      virtual void add  ( const HIHistProf&,
			  const HIHistProf&,
			  const float = 1.0,
			  const float = 1.0 ) =0;

      virtual void sub  ( const HIHistProf&,
			  const HIHistProf&,
			  const float = 1.0,
			  const float = 1.0 ) =0;

      virtual void mult ( const HIHistProf&,
			  const HIHistProf&,
			  const float = 1.0,
			  const float = 1.0 ) =0;
      
      virtual void div  ( const HIHistProf&,
			  const HIHistProf&,
			  const float = 1.0,
			  const float = 1.0 ) =0;

      virtual void copy( const HIHistProf& rOriginal )    =0;
*/

      // Accessors
//      virtual float getValue( unsigned int channel ) const =0;
      unsigned int nChannels    ( void ) const ;
      float lowEdgeX     ( void ) const ;
      float highEdgeX    ( void ) const ;
      float lowEdgeY     ( void ) const ;
      float highEdgeY    ( void ) const ;

      virtual float channelValue( unsigned int iChannel ) const  =0;
      virtual float channelError( unsigned int iChannel ) const  =0;

      virtual float channelLowEdge( unsigned int iChannel ) const ;
      virtual float channelCenter(  unsigned int iChannel ) const ;

      DisplayErrorsAs errorDisplay ( void ) const ;
      // ---------- const member functions ---------------------
      const HIHistProf& operator=( const HIHistProf& ); // stop default

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIHistProf(); // default
      HIHistProf( const HIHistProf& ); // stop default
      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      unsigned int m_nchannels ;
      float m_lowedgeX ;
      float m_highedgeX ;
      float m_lowedgeY ;
      float m_highedgeY ;
      DisplayErrorsAs m_errordisplay ;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HIHistProf.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHistPROF_H */



