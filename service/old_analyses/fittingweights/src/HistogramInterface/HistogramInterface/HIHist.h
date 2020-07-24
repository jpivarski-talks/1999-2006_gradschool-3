#if !defined(HISTOGRAMINTERFACE_HIHIST_H)
#define HISTOGRAMINTERFACE_HIHIST_H
// -*- C++ -*-
//
// Package:     <HistogramInterface>
// Module:      HIHist
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Marc Soussa
// Created:     Mon Nov  9 14:59:13 EST 1998
// $Id: HIHist.h,v 1.4 2000/06/19 18:17:22 bkh Exp $
//
// Revision history
//
// $Log: HIHist.h,v $
// Revision 1.4  2000/06/19 18:17:22  bkh
// Add some pure virtual functions for getting at sums, min, max, under, overflows
//
// Revision 1.3  1999/12/23 23:25:25  cdj
// added arithmetic operations on histograms
//
// Revision 1.2  1999/09/07 18:53:27  mkl
// new histogram interface
//
// Revision 1.1.1.1  1999/02/16 22:07:03  cdj
// first submission
//

// system include files
#include <string>

// user include files

// forward declarations
class HIHistoManager;
class HIHistFitFunc;

class HIHist 
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------
      // ---------- Constructors and destructor ----------------
      HIHist( Count  iId, 
	      const string& iTitle ) ;
      virtual ~HIHist();
      // ---------- member functions ---------------------------
      void setId() ;
      void setTitle ( void );
      virtual void reset () = 0; // Resets all values 
      // ---------- const member functions ---------------------
      // Accessors
      Count id ( void ) const;
      const string& title ( void ) const; 


      // contents sum not including over/underflows
      virtual float sumWithinLimits() const = 0 ; 

      // max of all bin contents not including over/underflows
      virtual float maxValue()        const = 0 ;

      // min of all bin contents not including over/underflows
      virtual float minValue()        const = 0 ;

      // associate a HIHistFitFunc to this histogram (user creates and
      // destroys it)
      void attachFitFunc( HIHistFitFunc* fitfunc );

      // get the HIHistFitFunc (or NULL if there is no such function
      // attached)
      HIHistFitFunc* fitFunc() const { return m_fitFunc; }
       
   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------
      
   private:
      // ---------- Constructors and destructor ----------------
      HIHist( const HIHist& ); // stop default
      HIHist();
      // ---------- assignment operator(s) ---------------------
      const HIHist& operator=( const HIHist& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      Count m_id ;
      string m_title ;
      HIHistFitFunc* m_fitFunc;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "HistogramInterface/Template/HIHist.cc"
//#endif

#endif /* HISTOGRAMINTERFACE_HIHist_H */









