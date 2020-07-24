#if !defined(HISTOGRAMINTERFACE_HIHIST2D_H)
#define HISTOGRAMINTERFACE_HIHIST2D_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHist2D
// 
// Description: This class inherits from the base class HIHist,
//              and serves as the base class for any 2D histogram.
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Fri Oct 30 10:08:43 EST 1998
// $Id: HIHist2D.h,v 1.6 2000/06/20 20:29:47 bkh Exp $
//
// Revision history
//
// $Log: HIHist2D.h,v $
// Revision 1.6  2000/06/20 20:29:47  bkh
// Add new virtual fcns
//
// Revision 1.5  2000/06/19 18:17:23  bkh
// Add some pure virtual functions for getting at sums, min, max, under, overflows
//
// Revision 1.4  2000/02/15 16:16:40  cdj
// arithmetic operations now use expression templates
//
// Revision 1.3  1999/12/23 23:25:27  cdj
// added arithmetic operations on histograms
//
// Revision 1.2  1999/09/07 18:53:29  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:04  cdj
// first submission
//

// system include files

// user include files
#include <utility>
#include "HIHist.h"
#include "HistogramInterface/HIArithmetic/HIBinaryOp.h"
#include "HistogramInterface/HIArithmetic/HIHistHolder.h"

// forward declarations

class HIHist2D : public HIHist
{
      // ---------- friend classes and functions ---------------
      friend class HIHistHolder<HIHist2D>;

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------

      HIHist2D( Count   iId,
                const string&  iTitle,
                unsigned int   iNChannelsX,
                float          iLowEdgeX,
                float           iHighEdgeX,
                unsigned int   iNChannelsY,
                float          iLowEdgeY,
                float          iHighEdgeY,
                float          iBitsPerChannel = 0 );

      virtual ~HIHist2D();

#if !defined(NO_TEMPLATED_MEMBER_FUNCTIONS_BUG)      
      template <class Operator, class Operand1, class Operand2>
      const HIHist2D& operator=( 
	 const HIBinaryOp<Operator, Operand1, Operand2>& rOperand );

#else
      const HIHist2D& operator=( const HIBinaryOpBase& rOperand );

#endif

      const HIHist2D& operator=( const HIHistHolder<HIHist2D>& rHistHolder );

      const HIHist2D& operator=( const HIHist2D& ); // stop default

      // ---------- member functions ---------------------------
      HIHist2D ( const HIHist& ); // stop default 

      // Modifiers
      virtual void fill ( float iXentry,
			  float iYentry,
			  float iWeight=1.0 ) =0;

      virtual void scale( const float iScale = 1.0 ) =0;
   
      virtual void add  ( const HIHist2D&,
			  const HIHist2D&,
			  const float = 1.0,
			  const float = 1.0 ) =0;

      virtual void sub  ( const HIHist2D&,
			  const HIHist2D&,
			  const float = 1.0,
			  const float = 1.0 ) =0;

      virtual void mult ( const HIHist2D&,
			  const HIHist2D&,
			  const float = 1.0,
			  const float = 1.0 ) =0;

      virtual void div  ( const HIHist2D&,
			  const HIHist2D&,
			  const float = 1.0,
			  const float = 1.0 ) = 0;

      virtual void copy( const HIHist2D& rOriginal )    =0;

      // ---------- const member functions ---------------------
      // Accessors 
      unsigned int  nChannelsX ( void ) const ;
      float lowEdgeX   ( void ) const ;
      float highEdgeX  ( void ) const ;
      unsigned int  nChannelsY ( void ) const ;
      float lowEdgeY   ( void ) const ;
      float highEdgeY  ( void ) const ;
      float bitsPerChannel ( void ) const ;

      pair<float,float> channelLoEdge( unsigned int iXChannel ,
				       unsigned int iYChannel  ) const ;

      pair<float,float> channelCenter( unsigned int iXChannel ,
				       unsigned int iYChannel  ) const ;

      float channelValue( unsigned int iXChannel,
			  unsigned int iYChannel ) const {
	 return channelValue( (iYChannel-1)*nChannelsX() + iXChannel );
      }

      // over- and under-flows by region

      virtual float underXoverY()    const = 0 ;
      virtual float insideXoverY()   const = 0 ;
      virtual float overXoverY()     const = 0 ;
      virtual float underXinsideY()  const = 0 ;
      virtual float insideXinsideY() const { return sumWithinLimits() ; } ;
      virtual float overXinsideY()   const = 0 ;
      virtual float underXunderY()   const = 0 ;
      virtual float insideXunderY()  const = 0 ;
      virtual float overXunderY()    const = 0 ;

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIHist2D(); // default
      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------
      virtual void fillChannelValue( unsigned int iChannel, 
				     float iWeight ) =0; 

      // ---------- private const member functions -------------
      virtual float channelValue( unsigned int iBin ) const  =0;
      virtual float channelError( unsigned int iBin ) const {return 0.;}
      // ---------- data members -------------------------------
      unsigned int m_nchannelsX ;
      float m_lowedgeX ;
      float m_highedgeX ;
      unsigned int m_nchannelsY ;
      float m_lowedgeY ;
      float m_highedgeY ;
      unsigned int m_bitsperchannel ;
      // ---------- static data members ------------------------

};

// inline function definitions

#if !defined(NO_TEMPLATED_MEMBER_FUNCTIONS_BUG)
template <class Operator, class Operand1, class Operand2>
inline
const HIHist2D&
HIHist2D::operator=( const HIBinaryOp<Operator, Operand1, Operand2>& rOperand )
{
   for ( int channel = 1; 
	 channel <= (nChannelsX() * nChannelsY()); 
	 channel++ )
   { 
      fillChannelValue( channel, rOperand.execute( channel ) );
   }

   return *this;
}

#else
inline
const HIHist2D&
HIHist2D::operator=( const HIBinaryOpBase& rOperand )
{
   for ( int channel = 1; 
	 channel <= (nChannelsX() * nChannelsY()); 
	 channel++ )
   { 
      fillChannelValue( channel, rOperand.execute( channel ) );
   }

   return *this;
}

#endif
// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HIHist2D.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHist2D_H */
