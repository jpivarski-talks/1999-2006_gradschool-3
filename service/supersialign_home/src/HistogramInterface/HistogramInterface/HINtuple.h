#if !defined(HISTOGRAMINTERFACE_HINTUPLE_H)
#define HISTOGRAMINTERFACE_HINTUPLE_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HINtuple
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Fri Oct 30 10:09:20 EST 1998
// $Id: HINtuple.h,v 1.4 2001/08/28 15:17:30 cdj Exp $
//
// Revision history
//
// $Log: HINtuple.h,v $
// Revision 1.4  2001/08/28 15:17:30  cdj
// now holds onto title and labels
//
// Revision 1.3  1999/09/07 18:53:31  mkl
// new histogram interface
//
// Revision 1.2  1999/02/23 20:27:08  cdj
// wrote the actual interface
//
// Revision 1.1.1.1  1999/02/16 22:07:03  cdj
// first submission
//

// system include files
#include <string>
// user include files

// forward declarations

class HINtuple
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      HINtuple( Count         iId,
		const string& iTitle,
		unsigned int  iNumberOfVariables,
		unsigned int  iChunkSize,
		const char* const* iLabels ) ;

      virtual ~HINtuple();

      // ---------- member functions ---------------------------
      virtual void fill( const float* iEntry ) =0 ;
      // ---------- const member functions ---------------------
      Count         id ( void ) const ;
      const string& title ( void ) const ;
      unsigned int  numberOfVariables ( void ) const ;
      unsigned int  chunkSize ( void ) const ;
      const char* const* labels ( void ) const ;
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------
      virtual void book ( const string& iTitle,
			  const string& iTopDir,
			  unsigned int  iNumberOfVariables,
			  unsigned int  iChunkSize,
			  const char* const* iLabels ) =0 ;

      virtual void close() =0 ;
      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      HINtuple( const HINtuple& ); // stop default
      HINtuple(); // default
      // ---------- assignment operator(s) ---------------------
      const HINtuple& operator=( const HINtuple& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Count         m_id ;
      string        m_title ;
      unsigned int  m_numberofvariables ;
      unsigned int  m_chunksize ;
      char* *  m_labels ;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HINtuple.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HINTUPLE_H */
