#if !defined(HISTOGRAMINTERFACE_HIARITHMETIC_H)
#define HISTOGRAMINTERFACE_HIARITHMETIC_H


#include "HistogramInterface/HIArithmetic/HIBinaryOp.h"
#include "HistogramInterface/HIArithmetic/HIHistHolder.h"

#include "HistogramInterface/HIHist1D.h"
#include "HistogramInterface/HIHist2D.h"

//
// algebraic operator overloading - HIHist1D
//

// Scalar Multiplication

template <class Op, class Op_T1, class Op_T2>
inline
HIBinaryOp<Op, Op_T1, Op_T2>
operator*(  double iScale,  const HIBinaryOp<Op, Op_T1, Op_T2>& rOperand )
{
   HIBinaryOp<Op,Op_T1, Op_T2> operand( rOperand );
   operand.setScalar( iScale*rOperand.scalar() );
   return operand;
}

template <class Op, class Op_T1, class Op_T2>
inline
HIBinaryOp<Op, Op_T1, Op_T2>
operator*(  float iScale,  const HIBinaryOp<Op, Op_T1, Op_T2>& rOperand )
{
   HIBinaryOp<Op,Op_T1, Op_T2> operand( rOperand );
   operand.setScalar( iScale*rOperand.scalar() );
   return operand;
}

template <class Op, class Op_T1, class Op_T2>
inline
HIBinaryOp<Op, Op_T1, Op_T2>
operator*( const HIBinaryOp<Op, Op_T1, Op_T2>& rOperand, double iScale )
{
   HIBinaryOp<Op,Op_T1, Op_T2> operand( rOperand );
   operand.setScalar( iScale*rOperand.scalar() );
   return operand;
}

template <class Op, class Op_T1, class Op_T2>
inline
HIBinaryOp<Op, Op_T1, Op_T2>
operator*( const HIBinaryOp<Op, Op_T1, Op_T2>& rOperand, float iScale )
{
   HIBinaryOp<Op,Op_T1, Op_T2> operand( rOperand );
   operand.setScalar( iScale*rOperand.scalar() );
   return operand;
}

inline
HIHistHolder<HIHist1D>
operator*( const HIHist1D& rHist,  double iScale )
{
   HIHistHolder<HIHist1D> histHolder( rHist );
   histHolder.setScalar( iScale );
   return histHolder;
}

inline
HIHistHolder<HIHist1D>
operator*( const HIHist1D& rHist,  float iScale )
{
   HIHistHolder<HIHist1D> histHolder( rHist );
   histHolder.setScalar( iScale );
   return histHolder;
}

inline
HIHistHolder<HIHist1D>
operator*( double iScale, HIHist1D& rHist )
{
   HIHistHolder<HIHist1D> histHolder( rHist );
   histHolder.setScalar( iScale );
   return histHolder;
}

inline
HIHistHolder<HIHist1D>
operator*( float iScale, const HIHist1D& rHist )
{
   HIHistHolder<HIHist1D> histHolder( rHist );
   histHolder.setScalar( iScale );
   return histHolder;
}


// Scalar Division

template <class Op, class Op_T1, class Op_T2>
inline
HIBinaryOp<Op, Op_T1, Op_T2>
operator/( const HIBinaryOp<Op, Op_T1, Op_T2>& rOperand, double iScale )
{
   HIBinaryOp<Op,Op_T1, Op_T2> operand( rOperand );
   operand.setScalar( (1.0/iScale)*rOperand.scalar() );
   return operand;
}

template <class Op, class Op_T1, class Op_T2>
inline
HIBinaryOp<Op, Op_T1, Op_T2>
operator/( const HIBinaryOp<Op, Op_T1, Op_T2>& rOperand, float iScale )
{
   HIBinaryOp<Op,Op_T1, Op_T2> operand( rOperand );
   operand.setScalar( (1.0/iScale)*rOperand.scalar() );
   return operand;
}

inline
HIHistHolder<HIHist1D>
operator/( const HIHist1D& rHist,  double iScale )
{
   HIHistHolder<HIHist1D> histHolder( rHist );
   histHolder.setScalar( (1/iScale) );
   return histHolder;
}

inline
HIHistHolder<HIHist1D>
operator/( const HIHist1D& rHist,  float iScale )
{
   HIHistHolder<HIHist1D> histHolder( rHist );
   histHolder.setScalar( (1/iScale) );
   return histHolder;
}


// Addition

template <class Op1, class Op1_T1, class Op1_T2, class Op2, class Op2_T1, class Op2_T2>
inline
HIBinaryOp<AddOp, HIBinaryOp<Op1, Op1_T1, Op1_T2> , 
                  HIBinaryOp<Op2, Op2_T1, Op2_T2> > 
operator+( const HIBinaryOp<Op1, Op1_T1, Op1_T2>& rOperand1, 
	   const HIBinaryOp<Op2, Op2_T1, Op2_T2>& rOperand2 )
{
   return HIBinaryOp<AddOp, HIBinaryOp<Op1, Op1_T1, Op1_T2>, 
                            HIBinaryOp<Op2, Op2_T1, Op2_T2> >( rOperand1, 
							       rOperand2 );
}

template < class Op1 >
inline
HIBinaryOp<AddOp, HIHistHolder<HIHist1D>, Op1 >
operator+(const HIHist1D& rHist, const Op1& rOperand)
{
   return HIBinaryOp<AddOp, HIHistHolder<HIHist1D>, Op1 >( 
      HIHistHolder<HIHist1D>(rHist), rOperand );
}

template < class Op1 >
inline
HIBinaryOp<AddOp, Op1, HIHistHolder<HIHist1D> >
operator+( const Op1& rOperand, const HIHist1D& rHist )
{
   return HIBinaryOp<AddOp, Op1, HIHistHolder<HIHist1D> >( 
      rOperand, HIHistHolder<HIHist1D>(rHist) );
}

inline
HIBinaryOp< AddOp, HIHistHolder<HIHist1D>, HIHistHolder<HIHist1D> > 
operator+( const HIHist1D& rHist1, 
	   const HIHist1D& rHist2 )
{
   return HIBinaryOp<AddOp, HIHistHolder<HIHist1D>, HIHistHolder<HIHist1D> >( 
      HIHistHolder<HIHist1D>(rHist1), HIHistHolder<HIHist1D>(rHist2) );
}

// Multiplication

template <class Op1, class Op1_T1, class Op1_T2, class Op2, class Op2_T1, class Op2_T2>
inline
HIBinaryOp<MulOp, HIBinaryOp<Op1, Op1_T1, Op1_T2> , 
                  HIBinaryOp<Op2, Op2_T1, Op2_T2> > 
operator*( const HIBinaryOp<Op1, Op1_T1, Op1_T2>& rOperand1, 
	   const HIBinaryOp<Op2, Op2_T1, Op2_T2>& rOperand2 )
{
   return HIBinaryOp<MulOp, HIBinaryOp<Op1, Op1_T1, Op1_T2>, 
                            HIBinaryOp<Op2, Op2_T1, Op2_T2> >( rOperand1, 
							       rOperand2 );
}

template < class Op1 >
inline
HIBinaryOp<MulOp, HIHistHolder<HIHist1D>, Op1 >
operator*(const HIHist1D& rHist, const Op1& rOperand)
{
   return HIBinaryOp<MulOp, HIHistHolder<HIHist1D>, Op1 >( 
      HIHistHolder<HIHist1D>(rHist), rOperand );
}

template < class Op1 >
inline
HIBinaryOp<MulOp, Op1, HIHistHolder<HIHist1D> >
operator*( const Op1& rOperand, const HIHist1D& rHist )
{
   return HIBinaryOp<MulOp, Op1, HIHistHolder<HIHist1D> >( 
      rOperand, HIHistHolder<HIHist1D>(rHist) );
}

inline
HIBinaryOp< MulOp, HIHistHolder<HIHist1D>, HIHistHolder<HIHist1D> > 
operator*( const HIHist1D& rHist1, 
	   const HIHist1D& rHist2 )
{
   return HIBinaryOp<MulOp, HIHistHolder<HIHist1D>, HIHistHolder<HIHist1D> >( 
      HIHistHolder<HIHist1D>(rHist1), HIHistHolder<HIHist1D>(rHist2) );
}


// Subtraction

template <class Op1, class Op1_T1, class Op1_T2, class Op2, class Op2_T1, class Op2_T2>
inline
HIBinaryOp<SubOp, HIBinaryOp<Op1, Op1_T1, Op1_T2> , 
                  HIBinaryOp<Op2, Op2_T1, Op2_T2> > 
operator-( const HIBinaryOp<Op1, Op1_T1, Op1_T2>& rOperand1, 
	   const HIBinaryOp<Op2, Op2_T1, Op2_T2>& rOperand2 )
{
   return HIBinaryOp<SubOp, HIBinaryOp<Op1, Op1_T1, Op1_T2>, 
                            HIBinaryOp<Op2, Op2_T1, Op2_T2> >( rOperand1, 
							       rOperand2 );
}

template < class Op1 >
inline
HIBinaryOp<SubOp, HIHistHolder<HIHist1D>, Op1 >
operator-(const HIHist1D& rHist, const Op1& rOperand)
{
   return HIBinaryOp<SubOp, HIHistHolder<HIHist1D>, Op1 >( 
      HIHistHolder<HIHist1D>(rHist), rOperand );
}

template < class Op1 >
inline
HIBinaryOp<SubOp, Op1, HIHistHolder<HIHist1D> >
operator-( const Op1& rOperand, const HIHist1D& rHist )
{
   return HIBinaryOp<SubOp, Op1, HIHistHolder<HIHist1D> >( 
      rOperand, HIHistHolder<HIHist1D>(rHist) );
}

inline
HIBinaryOp< SubOp, HIHistHolder<HIHist1D>, HIHistHolder<HIHist1D> > 
operator-( const HIHist1D& rHist1, 
	   const HIHist1D& rHist2 )
{
   return HIBinaryOp<SubOp, HIHistHolder<HIHist1D>, HIHistHolder<HIHist1D> >( 
      HIHistHolder<HIHist1D>(rHist1), HIHistHolder<HIHist1D>(rHist2) );
}

// Division

template <class Op1, class Op1_T1, class Op1_T2, class Op2, class Op2_T1, class Op2_T2>
inline
HIBinaryOp<DivOp, HIBinaryOp<Op1, Op1_T1, Op1_T2> , 
                  HIBinaryOp<Op2, Op2_T1, Op2_T2> > 
operator/( const HIBinaryOp<Op1, Op1_T1, Op1_T2>& rOperand1, 
	   const HIBinaryOp<Op2, Op2_T1, Op2_T2>& rOperand2 )
{
   return HIBinaryOp<DivOp, HIBinaryOp<Op1, Op1_T1, Op1_T2>, 
                            HIBinaryOp<Op2, Op2_T1, Op2_T2> >( rOperand1, 
							       rOperand2 );
}

template < class Op1 >
inline
HIBinaryOp<DivOp, HIHistHolder<HIHist1D>, Op1 >
operator/(const HIHist1D& rHist, const Op1& rOperand)
{
   return HIBinaryOp<DivOp, HIHistHolder<HIHist1D>, Op1 >( 
      HIHistHolder<HIHist1D>(rHist), rOperand );
}

template < class Op1 >
inline
HIBinaryOp<DivOp, Op1, HIHistHolder<HIHist1D> >
operator/( const Op1& rOperand, const HIHist1D& rHist )
{
   return HIBinaryOp<DivOp, Op1, HIHistHolder<HIHist1D> >( 
      rOperand, HIHistHolder<HIHist1D>(rHist) );
}

inline
HIBinaryOp< DivOp, HIHistHolder<HIHist1D>, HIHistHolder<HIHist1D> > 
operator/( const HIHist1D& rHist1, 
	   const HIHist1D& rHist2 )
{
   return HIBinaryOp<DivOp, HIHistHolder<HIHist1D>, HIHistHolder<HIHist1D> >( 
      HIHistHolder<HIHist1D>(rHist1), HIHistHolder<HIHist1D>(rHist2) );
}


//
// algebraic operator overloading - HIHist2D
//

// Scalar Multiplication


inline
HIHistHolder<HIHist2D>
operator*( HIHist2D& rHist, double iScale )
{
   HIHistHolder<HIHist2D> histHolder( rHist );
   histHolder.setScalar( iScale );
   return histHolder;
}

inline
HIHistHolder<HIHist2D>
operator*( HIHist2D& rHist, float iScale )
{
   HIHistHolder<HIHist2D> histHolder( rHist );
   histHolder.setScalar( iScale );
   return histHolder;
}

inline
HIHistHolder<HIHist2D>
operator*( double iScale, HIHist2D& rHist )
{
   HIHistHolder<HIHist2D> histHolder( rHist );
   histHolder.setScalar( iScale );
   return histHolder;
}

inline
HIHistHolder<HIHist2D>
operator*( float iScale, HIHist2D& rHist )
{
   HIHistHolder<HIHist2D> histHolder( rHist );
   histHolder.setScalar( iScale );
   return histHolder;
}

// Scalar Division

inline
HIHistHolder<HIHist2D>
operator/( const HIHist2D& rHist,  double iScale )
{
   HIHistHolder<HIHist2D> histHolder( rHist );
   histHolder.setScalar( (1/iScale) );
   return histHolder;
}

inline
HIHistHolder<HIHist2D>
operator/( const HIHist2D& rHist,  float iScale )
{
   HIHistHolder<HIHist2D> histHolder( rHist );
   histHolder.setScalar( (1/iScale) );
   return histHolder;
}


// Addition

template < class Op1 >
inline
HIBinaryOp<AddOp, HIHistHolder<HIHist2D>, Op1 >
operator+(HIHist2D& rHist, const Op1& rOperand)
{
   return HIBinaryOp<AddOp, HIHistHolder<HIHist2D>, Op1 >( 
      HIHistHolder<HIHist2D>(rHist), rOperand );
}

template < class Op1 >
inline
HIBinaryOp<AddOp, Op1, HIHistHolder<HIHist2D> >
operator+( const Op1& rOperand, HIHist2D& rHist )
{
   return HIBinaryOp<AddOp, Op1, HIHistHolder<HIHist2D> >( 
      rOperand, HIHistHolder<HIHist2D>(rHist) );
}

inline
HIBinaryOp< AddOp, HIHistHolder<HIHist2D>, HIHistHolder<HIHist2D> > 
operator+( HIHist2D& rHist1, 
	   HIHist2D& rHist2 )
{
   return HIBinaryOp<AddOp, HIHistHolder<HIHist2D>, HIHistHolder<HIHist2D> >( 
      HIHistHolder<HIHist2D>(rHist1), HIHistHolder<HIHist2D>(rHist2) );
}

// Multiplication


template < class Op1 >
inline
HIBinaryOp<MulOp, HIHistHolder<HIHist2D>, Op1 >
operator*(HIHist2D& rHist, const Op1& rOperand)
{
   return HIBinaryOp<MulOp, HIHistHolder<HIHist2D>, Op1 >( 
      HIHistHolder<HIHist2D>(rHist), rOperand );
}

template < class Op1 >
inline
HIBinaryOp<MulOp, Op1, HIHistHolder<HIHist2D> >
operator*( const Op1& rOperand, HIHist2D& rHist )
{
   return HIBinaryOp<MulOp, Op1, HIHistHolder<HIHist2D> >( 
      rOperand, HIHistHolder<HIHist2D>(rHist) );
}

inline
HIBinaryOp< MulOp, HIHistHolder<HIHist2D>, HIHistHolder<HIHist2D> > 
operator*( HIHist2D& rHist1, 
	   HIHist2D& rHist2 )
{
   return HIBinaryOp<MulOp, HIHistHolder<HIHist2D>, HIHistHolder<HIHist2D> >( 
      HIHistHolder<HIHist2D>(rHist1), HIHistHolder<HIHist2D>(rHist2) );
}


// Subtraction

template < class Op1 >
inline
HIBinaryOp<SubOp, HIHistHolder<HIHist2D>, Op1 >
operator-(HIHist2D& rHist, const Op1& rOperand)
{
   return HIBinaryOp<SubOp, HIHistHolder<HIHist2D>, Op1 >( 
      HIHistHolder<HIHist2D>(rHist), rOperand );
}

template < class Op1 >
inline
HIBinaryOp<SubOp, Op1, HIHistHolder<HIHist2D> >
operator-( const Op1& rOperand, HIHist2D& rHist )
{
   return HIBinaryOp<SubOp, Op1, HIHistHolder<HIHist2D> >( 
      rOperand, HIHistHolder<HIHist2D>(rHist) );
}

inline
HIBinaryOp< SubOp, HIHistHolder<HIHist2D>, HIHistHolder<HIHist2D> > 
operator-( HIHist2D& rHist1, 
	   HIHist2D& rHist2 )
{
   return HIBinaryOp<SubOp, HIHistHolder<HIHist2D>, HIHistHolder<HIHist2D> >( 
      HIHistHolder<HIHist2D>(rHist1), HIHistHolder<HIHist2D>(rHist2) );
}

// Division

template < class Op1 >
inline
HIBinaryOp<DivOp, HIHistHolder<HIHist2D>, Op1 >
operator/(const HIHist2D& rHist, const Op1& rOperand)
{
   return HIBinaryOp<DivOp, HIHistHolder<HIHist2D>, Op1 >( 
      HIHistHolder<HIHist2D>(rHist), rOperand );
}

template < class Op1 >
inline
HIBinaryOp<DivOp, Op1, HIHistHolder<HIHist2D> >
operator/( const Op1& rOperand, HIHist2D& rHist )
{
   return HIBinaryOp<DivOp, Op1, HIHistHolder<HIHist2D> >( 
      rOperand, HIHistHolder<HIHist2D>(rHist) );
}

inline
HIBinaryOp< DivOp, HIHistHolder<HIHist2D>, HIHistHolder<HIHist2D> > 
operator/( HIHist2D& rHist1, 
	   HIHist2D& rHist2 )
{
   return HIBinaryOp<DivOp, HIHistHolder<HIHist2D>, HIHistHolder<HIHist2D> >( 
      HIHistHolder<HIHist2D>(rHist1), HIHistHolder<HIHist2D>(rHist2) );
}

#endif /* HISTOGRAMINTERFACE_HIARITHMETIC_H */
