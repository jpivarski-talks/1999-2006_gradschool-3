#if !defined(PACKAGE_DRCAGURU_H)
#define PACKAGE_DRCAGURU_H
// -*- C++ -*-
//
// Package:     <package>
// Module:      DRCAGuru
// 
/**\class DRCAGuru DRCAGuru.h package/DRCAGuru.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Dan Cronin-Hennessy
// Created:     Wed Feb  2 14:58:38 EST 2000
// $Id: DRCAGuru.h,v 1.1 2000/03/24 01:30:01 hennessy Exp $
//
// Revision history
//
// $Log: DRCAGuru.h,v $
// Revision 1.1  2000/03/24 01:30:01  hennessy
// utility object for cathode simulation
//

// system include files

// user include files

// forward declarations

class DRCAGuru
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      // ---------- Constructors and destructor ----------------
      DRCAGuru();
      virtual ~DRCAGuru();
      
      // ---------- member functions ---------------------------
      //      float whatIsEffZPosition(float drZposition,float drDipAngle);
      int   whatIsClosestPadID(float effZposition, float drPhi);
      int   whatIsUpperLimitID(int closestPadID);
      int   whatIsLowerLimitID(int closestPadID);
      float whatIsWidthOfQDist(int closestPadID);
      float whatIsZPosition(int PadID);
      float whatIsQDeposited(int PadID, int iCenter, float width, float charge, float dZ);
 
      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------
      DRCAGuru( const DRCAGuru& ); // stop default

      // ---------- assignment operator(s) ---------------------
      const DRCAGuru& operator=( const DRCAGuru& ); // stop default

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------

      // ---------- data members -------------------------------

      // ---------- static data members ------------------------

};

// inline function definitions

// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "package/Template/DRCAGuru.cc"
//#endif

#endif /* PACKAGE_DRCAGURU_H */
