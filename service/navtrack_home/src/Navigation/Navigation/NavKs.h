#if !defined(NAVIGATION_NAVKS_H)
#define NAVIGATION_NAVKS_H
// -*- C++ -*-
//
// Package:     <Navigation>
// Module:      NavKs
// 
/**\class NavKs NavKs.h Navigation/NavKs.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      David Urner
// Created:     Tue Mar 20 10:33:09 EST 2001
// $Id: NavKs.h,v 1.4 2002/11/07 17:42:22 bkh Exp $
//
// Revision history
//
// $Log: NavKs.h,v $
// Revision 1.4  2002/11/07 17:42:22  bkh
// Direct MC tagging now available from tracks, showers, pi0s, Kshorts
// Direct Ks and Lambda access now available from NavTrack
//
// Revision 1.3  2002/05/06 12:37:36  ajm36
// changes for new VXFitVeeCandidate library
//
// Revision 1.2  2001/09/07 18:04:02  cleo3
// removed forward declaration of ostream
//
// Revision 1.1  2001/03/26 20:13:54  urner
// added Nav Ks
//

// system include files

// user include files

#include "VXFitVeeCandidate/VXFitVeeKShort.h"

// forward declarations
#include "C++Std/fwd_ostream.h"
class NavTrack ;

class NavKs ;

class MCParticle ;

ostream& operator<<( ostream& os, const NavKs& aKs ) ;

template <class T> class FAItem;

class NavKs
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      typedef VXFitVeeKShort::Identifier Identifier ;

      // ---------- Constructors and destructor ----------------
      NavKs(const VXFitVeeKShort* aKs ,
	    const NavTrack* aPiPlus ,
	    const NavTrack* aPiMinus   ) ;
      virtual ~NavKs();

      Identifier            identifier() const ;
      
      const VXFitVeeKShort& kShort()     const ;
      
      const NavTrack&       piPlus()     const ;
      const NavTrack&       piMinus()    const ;

      FAItem< MCParticle >  mcTag()      const ;

      // ---------- member functions ---------------------------

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      NavKs( const NavKs& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const NavKs& operator=( const NavKs& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      const VXFitVeeKShort* m_Ks ;
      const NavTrack*       m_PiPlus ;
      const NavTrack*       m_PiMinus ;

      const MCParticle*     m_mcTag ;
      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "Navigation/Template/NavKs.cc"
//#endif

#endif /* NAVIGATION_NAVKS_H */
