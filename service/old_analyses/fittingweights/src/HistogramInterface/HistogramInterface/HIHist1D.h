#if !defined(HISTOGRAMINTERFACE_HIHIST1D_H)
#define HISTOGRAMINTERFACE_HIHIST1D_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHist1D
// 
// Description: This class inherits from the base class HIHist,
//              and serves as the base class for any 1D histogram.
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Fri Nov  6 16:35:07 EST 1998
// $Id: HIHist1D.h,v 1.6 2000/06/20 20:29:46 bkh Exp $
//
// Revision history
//
// $Log: HIHist1D.h,v $
// Revision 1.6  2000/06/20 20:29:46  bkh
// Add new virtual fcns
//
// Revision 1.5  2000/06/19 18:17:23  bkh
// Add some pure virtual functions for getting at sums, min, max, under, overflows
//
// Revision 1.4  2000/02/15 16:16:40  cdj
// arithmetic operations now use expression templates
//
// Revision 1.3  1999/12/23 23:25:26  cdj
// added arithmetic operations on histograms
//
// Revision 1.2  1999/09/07 18:53:28  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:03  cdj
// first submission
//

// system include files
#include <string>
// user include files
#include "HistogramInterface/HIHist.h"
#include "HistogramInterface/HIArithmetic/HIBinaryOp.h"
#include "HistogramInterface/HIArithmetic/HIHistHolder.h"

// forward declarations

class HIHist1D : public HIHist
{
      // ---------- friend classes and functions ---------------
      friend class HIHistHolder<HIHist1D>;
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIHist1D( Count  iId,
		const string& iTitle,
		unsigned int  iNChannels,
		float         iLowEdge,
		float         iHighEdge,
		float         iBitsPerChannel = 0 );

      virtual ~HIHist1D();

      // ---------- member functions ---------------------------
#if !defined(NO_TEMPLATED_MEMBER_FUNCTIONS_BUG)
      template <class Operator, class Operand1, class Operand2>
      const HIHist1D& operator=( 
	 const HIBinaryOp<Operator, Operand1, Operand2>& rOperand );
#else
      const HIHist1D& operator=( const HIBinaryOpBase& rOperand );
#endif

      const HIHist1D& operator=( const HIHistHolder<HIHist1D>& rHistHolder );


      // Modifiers
      virtual void fill ( float iEntry,
			  float iWeight=1.0 ) =0;

      virtual void scale( const float iScale = 1.0 ) =0;

      virtual void add  ( const HIHist1D& rHist1,
			  const HIHist1D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 )   =0;
      
      virtual void sub  ( const HIHist1D& rHist1,
			  const HIHist1D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 )   =0;
 
      virtual void mult ( const HIHist1D& rHist1,
			  const HIHist1D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 )   =0;

      virtual void div  ( const HIHist1D& rHist1,
			  const HIHist1D& rHist2,
			  const float iScale1 = 1.0,
			  const float iScale2 = 1.0 )   =0;

      virtual void copy( const HIHist1D& rOriginal )    =0;

      virtual void setErrors( const float* iErrorArray ) =0;

      // ---------- const member functions ---------------------

      // Accessors
      unsigned int nChannels ( void ) const ;
      float lowEdge   ( void ) const ;
      float highEdge  ( void ) const ;
      float bitsPerChannel ( void ) const ;
      virtual float channelValue( unsigned int iChannel ) const  =0;
      virtual float channelError( unsigned int iChannel ) const  =0;

      virtual float channelLowEdge( unsigned int iChannel ) const ;
      virtual float channelCenter(  unsigned int iChannel ) const ;

      virtual float underflow() const = 0 ;
      virtual float overflow()  const = 0 ;

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HIHist1D( const HIHist1D& ); // stop default
      HIHist1D(); // default

      // ---------- assignment operator(s) ---------------------
      const HIHist1D& operator=( const HIHist1D& ); // stop default

      // ---------- private member functions -------------------
      virtual void fillChannelValue( unsigned int iChannel, 
				     float iWeight ) =0; 

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      unsigned int m_nchannels ;
      float m_lowedge ;
      float m_highedge ;
      unsigned int m_bitsperchannel ;
      // ---------- static data members ------------------------

};

// inline function definitions

#if !defined(NO_TEMPLATED_MEMBER_FUNCTIONS_BUG)
template <class Operator, class Operand1, class Operand2>
inline
const HIHist1D&
HIHist1D::operator=( const HIBinaryOp<Operator, Operand1, Operand2>& rOperand )
{
   for ( int channel = 1; 
	channel <= nChannels(); 
	channel++ )
   { 
      fillChannelValue( channel, rOperand.execute( channel ) ); 
   }

   float* errorArray = new float[nChannels()];

   for ( int channelError = 1; 
	channelError <= nChannels(); 
	channelError++ )
   { 
      errorArray[channelError-1] = rOperand.executeError( channelError );
   }

   setErrors( errorArray );
 
   delete [] errorArray; 

   return *this;
}

#else

inline
const HIHist1D& 
HIHist1D::operator=( const HIBinaryOpBase& rOperand )
{
   for ( int channel = 1; 
	channel <= nChannels(); 
	channel++ )
   { 
      fillChannelValue( channel, rOperand.execute( channel ) ); 
   }

   float* errorArray = new float[nChannels()];

   for ( int channelError = 1; 
	channelError <= nChannels(); 
	channelError++ )
   { 
      errorArray[channelError-1] = rOperand.executeError( channelError );
   }

   setErrors( errorArray );
 
   delete [] errorArray; 

   return *this;
}

#endif

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HIHist1D.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHIST1D_H */
