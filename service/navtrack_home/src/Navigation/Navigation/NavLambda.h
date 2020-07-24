#if !defined(NAVIGATION_NAVLAMBDA_H)
#define NAVIGATION_NAVLAMBDA_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      NavLambda
// 
/**\class NavLambda NavLambda.h Navigation/NavLambda.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      David Urner
// Created:     Tue Mar 20 10:33:09 EST 2001
// $Id: NavLambda.h,v 1.3 2002/05/06 12:37:36 ajm36 Exp $
//
// Revision history
//
// $Log: NavLambda.h,v $
// Revision 1.3  2002/05/06 12:37:36  ajm36
// changes for new VXFitVeeCandidate library
//
// Revision 1.2  2001/09/19 14:19:11  ajm36
// get rid of ostream forward declaration
//
// Revision 1.1  2001/09/07 21:50:33  eckhart
// Created File
//
// Revision 1.1  2001/03/26 20:13:54  urner
// added Nav Lambda
//

// system include files

// user include files

#include "VXFitVeeCandidate/VXFitVeeLambda.h"

// forward declarations

class NavTrack ;

#include "C++Std/fwd_ostream.h"

class NavLambda ;

ostream& operator<<( ostream& os, const NavLambda& aLambda ) ;

class NavLambda
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef VXFitVeeLambda::Identifier Identifier ;

      // ---------- Constructors and destructor ----------------
      NavLambda(const VXFitVeeLambda* aLambda ,
	    const NavTrack* aProton ,
	    const NavTrack* aPion   ) ;
      virtual ~NavLambda();

      Identifier            identifier() const ;
      
      const VXFitVeeLambda& lambda()     const ;
      
      const NavTrack&       proton()     const ;
      const NavTrack&       pion()    const ;
      DABoolean isbar() const { return m_isbar; }

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      NavLambda( const NavLambda& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const NavLambda& operator=( const NavLambda& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      const VXFitVeeLambda* m_Lambda ;
      const NavTrack*       m_Proton ;
      const NavTrack*       m_Pion ;
      DABoolean m_isbar;

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "Navigation/Template/NavLambda.cc"
//#endif

#endif /* NAVIGATION_NAVLAMBDA_H */






