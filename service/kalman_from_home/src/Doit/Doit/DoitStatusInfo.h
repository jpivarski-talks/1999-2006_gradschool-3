#if !defined(DOIT_DOITSTATUSINFO_H)
#define DOIT_DOITSTATUSINFO_H
// -*- C++ -*-
//
// Package:     <Doit>
// Module:      DoitStatusInfo
// 
/**\class DoitStatusInfo DoitStatusInfo.h Doit/DoitStatusInfo.h

 Description: The Doit Finder Status -- A translation of the 
STATCD words from doit/dufind/cdtrakcd.inc

 Usage: This is mainly for Dan Peterson, but others may find some
        of the information here useful
    
*/
//
// Author:      Adam Lyon
// Created:     Tue Jan  2 10:11:54 EST 2001
// $Id: DoitStatusInfo.h,v 1.4 2002/05/23 14:38:31 dpp Exp $
//
// Revision history
//
// $Log: DoitStatusInfo.h,v $
// Revision 1.4  2002/05/23 14:38:31  dpp
//      -> add new status bits for limited radius solution
//
// Revision 1.3  2001/12/05 16:47:37  lyon
// Add more bits to the DoitStatusInfo object
//
// Revision 1.2  2001/01/05 06:30:31  lyon
// Added assert to check for bad SeedType; Actually register V3 storage helper this time
//
// Revision 1.1  2001/01/05 05:37:36  lyon
// Added Doit status words
//

// system include files
#include "C++Std/fwd_ostream.h"

// user include files

// forward declarations

class DoitStatusInfo
{
      // ---------- friend classes and functions ---------------

   public:
      // ---------- constants, enums and typedefs --------------

      //! Number of status words
      enum { knWords = 4 };

      //! Seed type
      enum SeedType { kSF, kAlternate1, kAlternate2, kBad=999 };

      // ---------- Constructors and destructor ----------------
      DoitStatusInfo(UInt32 word1, UInt32 word2, UInt32 word3, UInt32 word4);

      DoitStatusInfo( const DoitStatusInfo& rhs );
      DoitStatusInfo();
      virtual ~DoitStatusInfo();

      // ---------- member functions ---------------------------
      const DoitStatusInfo& operator=( const DoitStatusInfo& );

      // ---------- const member functions ---------------------
      
      // For the storage helper
      const UInt32* words() const { return m_words; };
  
      //! What is the seed type (word1, bits1-3)
      SeedType seedType() const;

      //! SF pass number (word1, bits4-6)
      unsigned int sfPassNumber() const { return value(1, 4, 3); };
  
      //! SF Merge type (word1, bits7-8)
      unsigned int sfMergeType() const { return value(1, 7, 2); };

      //! SF Merge indicator (word1, bit9)
      unsigned int sfMergeIndicator() const { return value(1, 9, 1); };

      //! SF Chain number (word1, bits10-17)
      unsigned int sfChainNumber() const { return value(1, 10, 8); };

      //! Number of hits in the first wire fit (word1, bits18-23)
      unsigned int nHitsFirstWireFit() const { return value(1, 18, 6); };

      //! Removed D0 constraint in wire fit (word1, bit24)
      DABoolean d0ConstraintRemovedInWireFit() const { 
	return isTrue(1, 24); };

      //! Was adding a kink tried (word1, bit25)
      DABoolean addingKinkTriedInWireFit() const { return isTrue(1, 25); };

      //! Did fitting continue with the kink (word1, bit26)
      DABoolean continuedWithKinkInWireFit() const { return isTrue(1, 26); };

      //! Was kink continued after adding close (word1, bit27)
      DABoolean continuedKinkAfterCloseInWireFit() const { 
	return isTrue(1, 27); };
  
      //! Was adding a kink tried in the hit fit (word1, bit28)
      DABoolean addingKinkTriedInHitFit() const { return isTrue(1, 28); };

      //! Did fitting continue with the kink in the hit fit (word1, bit29)
      DABoolean continuedWithKinkInHitFit() const { return isTrue(1, 29); };

      //! Was kink continued after deleting bit hits and adding hits 
      //! (word1, bit30)
      DABoolean continuedKinkAfterDelAndAddInHitFit() const { 
	return isTrue(1, 30); } 

      // -------- These are for first FAST_TRAK iteration -------
  
      //! D0 constraint [1st FastTrak Iteration] (word2, bit1)
      DABoolean d0Constraint1() const { return isTrue(2, 1); };

      //! Z0 constraint [1st FastTrak Iteration] (word2, bit2)
      DABoolean z0Constraint1() const { return isTrue(2, 2); };

      //! From alternate ambiguity [1st FastTrak Iteration] (word2, bit3)
      DABoolean fromAltAmbig1() const { return isTrue(2, 3); };

      //! Did alternate ambig use null? [1st FastTrak Iteration] (word2, bit4)
      DABoolean altAmbigUsedNull1() const { return isTrue(2, 4); };

      //! Number of hits from local ambiguity [1st FastTrak Iteration]
      //! (word2, bits5-10)
      unsigned int nHitsFromLocalAmbig1() const { return value(2, 5, 6); };
  
      // -------- These are for second FAST_TRAK iteration -------
  
      //! D0 constraint [2nd FastTrak Iteration] (word2, bit11)
      DABoolean d0Constraint2() const { return isTrue(2, 11); };

      //! Z0 constraint [2nd FastTrak Iteration] (word2, bit12)
      DABoolean z0Constraint2() const { return isTrue(2, 12); };

      //! From alternate ambiguity [2nd FastTrak Iteration] (word2, bit13)
      DABoolean fromAltAmbig2() const { return isTrue(2, 13); };

      //! Did alternate ambig use null? [2nd FastTrak Iteration] (word2, bit14)
      DABoolean altAmbigUsedNull2() const { return isTrue(2, 14); };

      //! Number of hits from local ambiguity [2nd FastTrak Iteration]
      //! (word2, bits15-20)
      unsigned int nHitsFromLocalAmbig2() const { return value(2, 15, 6); };

      //! Did a first call to TFXFITCF (word2, bit21)
      DABoolean firstCallTFXFITCF() const { return isTrue(2, 21); };

      //! Z hits replaced after first call (word2, bit22)
      DABoolean replacedZHitsAfterFirstCall() const { return isTrue(2, 22); };

      //! Did we recall FAST_TRAK (word2, bit23)
      DABoolean recalledFastTrak() const { return isTrue(2, 23); };
  
      //! Did we pass a first recall of FAST_TRAK (word2, bit24)
      DABoolean firstFastTrakRecall() const { return isTrue(2, 24); };
  
      //! Did we pass a second recall of FAST_TRAK (word2, bit25)
      DABoolean secondFastTrakRecall() const { return isTrue(2, 25); };
  
      //! Did we get a new representation from the recall of FAST_TRAK
      //! (word2, bit26)
      DABoolean newRepresentationFromRecall() const { return isTrue(2, 26); };
  
      //! Did a 2nd call to TFXFITCF (word2, bit26)
      DABoolean secondCallTFXFITCF() const { return isTrue(2, 27); };

      //! Were z hits replaced after second call (word2, bit28)
      DABoolean replacedZHitsAfterSecondCall() const { return isTrue(2, 28); };

      //! Did we remove Z0 bias after call to CF (word 2, 29)
      DABoolean removeZ0BiasAfterCF() const { return isTrue(2, 29); };

      //! Did we test TFXFIT-LF with normal z0 (word3, bit1)
      DABoolean testedTFXFITLFWithZ0Normal() const {
	return isTrue(3, 1); };

      //! Did we test TFXFIT-LF with z0 constraint (word3, bit2)
      DABoolean testedTFXFITLFWithZ0Constraint() const { 
	return isTrue(3, 2); };

      //! Did we use TFXFIT-LF with z0 constraint (word3, bit3)
      DABoolean usedTFXFITLFWithZ0Constraint() const { return isTrue(3, 3); };

      //! Did we boot silicon z hits for curlers (word3, bit4)
      DABoolean bootSiliconZ() const { return isTrue(3, 4); };

      //! tested for a limited radius solution 
      //! (bit will be set for any representation of the track;
      //! not just the limited radius solution itself)
      DABoolean limitRadiusSolutionTest() const { return isTrue(3, 5); };

      //! limited radius solution is found 
      //! (bit will be set for any representation of the track;
      //! not just the limited radius solution itself)
      DABoolean limitRadiusSolutionSol() const { return isTrue(3, 6); };

      //! limited radius solution has been selected
      //! (bit will be set only for the limited radius solution) 
      DABoolean limitRadiusSolutionSel() const { return isTrue(3, 7); };



      //! Print out a report in a nice way (used by operator<<)
      void printout(ostream& os) const;
  
      // ---------- static member functions --------------------

   protected:
      // ---------- protected member functions -----------------

      // ---------- protected const member functions -----------

   private:
      // ---------- Constructors and destructor ----------------

      // ---------- assignment operator(s) ---------------------

      // ---------- private member functions -------------------

      // ---------- private const member functions -------------
  
      // Return true if a bit is set (word and bit start at 1)
      DABoolean isTrue(unsigned int word, unsigned int bit) const;

      // Return the value of part of a word (word and bit start at 1)
      unsigned int value(unsigned int word, unsigned int startBit, 
			 unsigned int nBits) const;

      // Print out a status flag
      void reportFlag(ostream& s, const char* text, DABoolean flag) const;

      // ---------- data members -------------------------------
      UInt32 m_words[knWords];

      // ---------- static data members ------------------------
  
};

// inline function definitions

// Declare operator<< for this class
inline ostream& operator<<(ostream& stream, const DoitStatusInfo& doitStatusInfo)
{
  doitStatusInfo.printout(stream);
  return stream;
};


// Uncomment the following lines, if your class is templated 
// and has an implementation file (in the Template directory)
//#if defined(INCLUDE_TEMPLATE_DEFINITIONS)
//# include "Doit/Template/DoitStatusInfo.cc"
//#endif

#endif /* DOIT_DOITSTATUSINFO_H */
