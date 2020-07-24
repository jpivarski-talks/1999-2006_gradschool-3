#if !defined(HISTOGRAMINTERFACE_HIHISTHOLDER_H)
#define HISTOGRAMINTERFACE_HIHISTHOLDER_H
// -*- C++ -*-
//
// Package:     HistogramInterface
// Module:      HIHistHolder
// 
/**\class HIHistHolder HIHistHolder.h HistogramInterface/HIHistHolder.h

 Description: Inherits from HIOperand and holds a single HIHist ref and a 
              scale factor, allowing a HIOperand pointer to represent a scaled
	      histogram, without their being an associated algebraic operation
	      involved.

 Usage:
    <usage>

*/
//
// Author:      Aren Jansen
// Created:     Tue Oct 26 14:18:11 EDT 1999
// $Id: HIHistHolder.h,v 1.2 2000/02/15 16:16:45 cdj Exp $
//
// Revision history
//
// $Log: HIHistHolder.h,v $
// Revision 1.2  2000/02/15 16:16:45  cdj
// arithmetic operations now use expression templates
//
// Revision 1.1  1999/12/23 23:24:33  cdj
// first submission
//

// system include files

// user include files

// forward declarations
template <class T>
class HIHistHolder
{
      // ---------- friend classes and functions ---------------
   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HIHistHolder( const T& rHist );
      virtual ~HIHistHolder();

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------
      float execute( unsigned int channel ) const;
      float executeError( unsigned int channel ) const;
      const T& histogram() const;
      void setScalar( float iScale ) const { 
	 const_cast<HIHistHolder<T>*>(this)->m_Scalar = iScale; }
      float scalar() const { return m_Scalar; }

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      // HIHistHolder( const HIHistHolder& ); // stop default

      // ---------- assignment operator(s) ---------------------
      // const HIHistHolder& operator=( const HIHistHolder& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      const T& m_Hist;
      float m_Scalar;
      // ---------- static data members ------------------------

};

// template class implementation

template <class T>
inline
HIHistHolder<T>::HIHistHolder( const T& rHist )
   : m_Hist( rHist ),
     m_Scalar(1.0)
{
}


template <class T>
inline
HIHistHolder<T>::~HIHistHolder()
{
}

/*
template <class T>
inline
T& 
HIHistHolder<T>::execute( T& rHist ) const
{
   return histogram();
}
*/

template <class T>
inline
float
HIHistHolder<T>::execute( unsigned int channel ) const
{
   return histogram().channelValue( channel );
}

template <class T>
inline
float
HIHistHolder<T>::executeError( unsigned int channel ) const
{
   return histogram().channelError( channel );
}


template <class T>
inline
const T&
HIHistHolder<T>::histogram() const
{
   return m_Hist;
}

#endif /* HISTOGRAMINTERFACE_HIHISTHOLDER_H */



