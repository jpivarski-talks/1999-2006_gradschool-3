#if !defined(KALMANFILTER_KALMANUPDATEZPARAMSCONDITION_H)
#define KALMANFILTER_KALMANUPDATEZPARAMSCONDITION_H
// -*- C++ -*-
//
// Package:     <McCannFilter>
// Module:      McCannUpdateZParamsCondition
// 
// Description: <one line class summary>
//
// Usage:
//    <usage>
//
// Author:      Werner Sun
// Created:     Mon Apr  5 02:47:11 EDT 1999
// $Id: McCannUpdateZParamsCondition.h,v 1.2 1999/08/27 20:43:05 wsun Exp $
//
// Revision history
//
// $Log: McCannUpdateZParamsCondition.h,v $
// Revision 1.2  1999/08/27 20:43:05  wsun
// New constructor.
//
// Revision 1.1  1999/05/29 20:20:48  wsun
// First submission.
//
//

// system include files

// user include files
#include "HelixIntersection/HICondition.h"

// forward declarations

class McCannUpdateZParamsCondition : public HICondition
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      McCannUpdateZParamsCondition( double aErr2Z0 = 0.01,
				    double aErr2CotTheta = 1.,
				    double aDFracCotTheta = 2. ) ;
      virtual ~McCannUpdateZParamsCondition();

      // ---------- member functions ---------------------------
      void setErr2Z0( double aErr ) { m_err2Z0 = aErr ; }
      void setErr2CotTheta( double aErr ) { m_err2CotTheta = aErr ; }
      void setDFracCotTheta( double aFrac ) { m_dFracCotTheta = aFrac ; }

      // ---------- const member functions ---------------------
      virtual DABoolean satisfied( const HIHelix& aHelix,
				   HIIntersectionSurface& ) const ;

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      McCannUpdateZParamsCondition( const McCannUpdateZParamsCondition& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const McCannUpdateZParamsCondition& operator=( const McCannUpdateZParamsCondition& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------
      double m_err2Z0 ;
      double m_err2CotTheta ;
      double m_dFracCotTheta ;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "McCannFilter/Template/McCannUpdateZParamsCondition.cc"
//#endif

#endif /* KALMANFILTER_KALMANUPDATEZPARAMSCONDITION_H */
