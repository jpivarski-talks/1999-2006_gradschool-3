// -*- C++ -*-
#if !defined(HISTOGRAMVIEWERPROC_HISTOGRAMVIEWERPROC_H)
#define HISTOGRAMVIEWERPROC_HISTOGRAMVIEWERPROC_H
//
// Package:     <HistogramViewerProc>
// Module:      HistogramViewerProc
//
/**\class HistogramViewerProc HistogramViewerProc.h HistogramViewerProc/HistogramViewerProc.h
 
 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris D Jones
// Created:     Fri Jul 20 09:24:39 EDT 2001
// $Id: HistogramViewerProc.h,v 1.1.1.1 2001/07/30 14:11:45 cdj Exp $
//
// Revision history
//
// $Log: HistogramViewerProc.h,v $
// Revision 1.1.1.1  2001/07/30 14:11:45  cdj
// importing
//
//

// system include files

// user include files
#include "Processor/Processor.h"
#include "HistogramInterface/HistogramPackage.h"

// forward declarations
class HVPWindow;
class QApplication;

class HistogramViewerProc : public Processor
{
      // ------------ friend classses and functions --------------

   public:
      // ------------ constants, enums and typedefs --------------

      // ------------ Constructors and destructor ----------------
      HistogramViewerProc( void );                      // anal1 
      virtual ~HistogramViewerProc();                   // anal5 

      // ------------ member functions ---------------------------

      // methods for beginning/end "Interactive"
      virtual void init( void );             // anal1 "Interactive"
      virtual void terminate( void );        // anal5 "Interactive"

      // standard place for booking histograms
      virtual void hist_book( HIHistoManager& );                  

      // methods for binding to streams (anal2-4 etc.)
      virtual ActionBase::ActionResult action( Frame& iFrame );

      // ------------ const member functions ---------------------

      // ------------ static member functions --------------------

   protected:
      // ------------ protected member functions -----------------

      // ------------ protected const member functions -----------

   private:
      // ------------ Constructors and destructor ----------------
      HistogramViewerProc( const HistogramViewerProc& );

      // ------------ assignment operator(s) ---------------------
      const HistogramViewerProc& operator=( const HistogramViewerProc& );

      // ------------ private member functions -------------------
      void bind( 
         ActionBase::ActionResult (HistogramViewerProc::*iMethod)( Frame& ),
	      const Stream::Type& iStream );

      // ------------ private const member functions -------------

      // ------------ data members -------------------------------
      HVPWindow* m_window;
      QApplication* m_app;
      unsigned int m_numberStopsSinceLastUpdate;

      // ------------ static data members ------------------------

};

// inline function definitions

#endif /* HISTOGRAMVIEWERPROC_HISTOGRAMVIEWERPROC_H */
