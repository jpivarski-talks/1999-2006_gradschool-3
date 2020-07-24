#if !defined(HISTOGRAMINTERFACE_HIBINARYOP_H)
#define HISTOGRAMINTERFACE_HIBINARYOP_H
// -*- C++ -*-
//
// Package:     HistogramInterface
// Module:      HIBinaryOp
// 
/**\class HIBinaryOp HIBinaryOp.h package/HIBinaryOp.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Aren Jansen
// Created:     Mon Jan 17 10:23:35 EST 2000
// $Id: HIBinaryOp.h,v 1.2 2001/09/07 16:56:20 cleo3 Exp $
//
// Revision history
//
// $Log: HIBinaryOp.h,v $
// Revision 1.2  2001/09/07 16:56:20  cleo3
// use cleo_math.h instead of math.h
//
// Revision 1.1  2000/02/15 16:16:43  cdj
// arithmetic operations now use expression templates
//

// system include files
// user include files
#include "Experiment/cleo_math.h"
#include "HistogramInterface/HIArithmetic/HIOperators.h"

#if defined(NO_TEMPLATED_MEMBER_FUNCTIONS_BUG)
#include "HistogramInterface/HIArithmetic/HIBinaryOpBase.h"
#endif

// forward declarations

template <class Operator, class Operand1, class Operand2>
#if !defined(NO_TEMPLATED_MEMBER_FUNCTIONS_BUG)
class HIBinaryOp
#else
class HIBinaryOp : public HIBinaryOpBase
#endif
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIBinaryOp( const Operand1 LHS, const Operand2 RHS );
      //virtual ~HIBinaryOp();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      float execute( unsigned int channel ) const;
      float executeError( unsigned int channel ) const;
      void setScalar( const float iScale ) const { 
	 const_cast<
	    HIBinaryOp<Operator, Operand1, Operand2>*
	           >(this)->m_Scalar = iScale; 
      }
      const float scalar() const { return m_Scalar; }
      const Operand1& operand1() const;
      const Operand2& operand2() const;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      // HIBinaryOp( const HIBinaryOp& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const HIBinaryOp& operator=( const HIBinaryOp& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Operand1 m_Operand1;
      Operand2 m_Operand2;
      float m_Scalar;
      // ---------- static data members ------------------------

};

// template class implementation
template <class Operator, class Operand1, class Operand2>
inline
HIBinaryOp<Operator, Operand1, Operand2>::HIBinaryOp( 
   const Operand1 LHS, const Operand2 RHS )
   : m_Operand1(LHS),
     m_Operand2(RHS),
     m_Scalar(1.0)
{
}

//template <class Operator, class Operand1, class Operand2>
//inline
//HIBinaryOp<Operator, Operand1, Operand2>::~HIBinaryOp()
//{
//}

template <class Operator, class Operand1, class Operand2>
inline
const Operand1&
HIBinaryOp<Operator, Operand1, Operand2>::operand1() const
{
   return m_Operand1;
}

template <class Operator, class Operand1, class Operand2>
inline
const Operand2&
HIBinaryOp<Operator, Operand1, Operand2>::operand2() const
{
   return m_Operand2;
}


template <class Operator, class Operand1, class Operand2>
inline
float
HIBinaryOp<Operator, Operand1, Operand2>::execute( unsigned int channel ) const
{
   return Operator::execute( scalar(),
      operand1().scalar() * operand1().execute( channel ),
      operand2().scalar() * operand2().execute( channel ) );
}

template <class Operator, class Operand1, class Operand2>
inline
float
HIBinaryOp<Operator, Operand1, Operand2>::executeError( 
   unsigned int channel ) const
{
   float sigma1 = operand1().executeError( channel );

   float sigma2 = operand2().executeError( channel );

   return  scalar() * sqrt( Operator::executeError(
      sigma1, sigma2, operand1().scalar(), operand2().scalar(),
      operand1().execute( channel ), operand2().execute( channel ) ));  
}

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/HIBinaryOp.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIBINARYOP_H */
