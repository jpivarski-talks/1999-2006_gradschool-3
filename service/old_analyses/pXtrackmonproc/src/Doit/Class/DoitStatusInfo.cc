// -*- C++ -*-
//
// Package:     <Doit>
// Module:      DoitStatusInfo
// 
// Description: <one line class summary>
//
// Implementation:
//     <Notes on implementation>
//
// Author:      Adam Lyon
// Created:     Tue Jan  2 11:31:02 EST 2001
// $Id: DoitStatusInfo.cc,v 1.3 2001/12/05 16:47:29 lyon Exp $
//
// Revision history
//
// $Log: DoitStatusInfo.cc,v $
// Revision 1.3  2001/12/05 16:47:29  lyon
// Add more bits to the DoitStatusInfo object
//
// Revision 1.2  2001/01/05 06:30:22  lyon
// Added assert to check for bad SeedType; Actually register V3 storage helper this time
//
// Revision 1.1  2001/01/05 05:37:14  lyon
// Added Doit status words
//

#include "Experiment/Experiment.h"

#include <iostream.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

// system include files
#if defined(STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG)
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
#include <algorithm>
//#include <utility>
#endif /* STL_TEMPLATE_DEFAULT_PARAMS_FIRST_BUG */

// user include files
//#include "Experiment/report.h"
#include "Doit/DoitStatusInfo.h"

// STL classes
// You may have to uncomment some of these or other stl headers
// depending on what other header files you include (e.g. FrameAccess etc.)!
//#include <string>
//#include <vector>
//#include <set>
//#include <map>
#include <algorithm>
//#include <utility>

//
// constants, enums and typedefs
//

static const char* const kFacilityString = "Doit.DoitStatusInfo" ;

// ---- cvs-based strings (Id and Tag with which file was checked out)
static const char* const kIdString  = "$Id: DoitStatusInfo.cc,v 1.3 2001/12/05 16:47:29 lyon Exp $";
static const char* const kTagString = "$Name:  $";

//
// static data member definitions
//

//
// constructors and destructor
//
DoitStatusInfo::DoitStatusInfo(UInt32 word1, UInt32 word2, 
			       UInt32 word3, UInt32 word4)
{
  m_words[0] = word1;
  m_words[1] = word2;
  m_words[2] = word3;
  m_words[3] = word4;
};

DoitStatusInfo::DoitStatusInfo( const DoitStatusInfo& rhs )
{
   // do actual copying here; if you implemented
   // operator= correctly, you may be able to use just say      
   *this = rhs;
}

DoitStatusInfo::~DoitStatusInfo()
{}

//
// assignment operators
//
const DoitStatusInfo& DoitStatusInfo::operator=( const DoitStatusInfo& rhs )
{
  if ( this != &rhs ) 
  {
    for ( unsigned int i = 0; i < knWords; ++i )
    {
      m_words[i] = rhs.m_words[i];
    }
  }

  return *this;
}

//
// member functions
//

//
// const member functions
//


DoitStatusInfo::SeedType DoitStatusInfo::seedType() const
{
  SeedType seedType = kBad;
  
  if ( isTrue(1, 1) ) seedType = kSF;
  if ( isTrue(1, 2) ) seedType = kAlternate1;
  if ( isTrue(1, 3) ) seedType = kAlternate2;

  return seedType;
}

DABoolean DoitStatusInfo::isTrue(unsigned int word, unsigned int bit) const
{
  // Get the word
  UInt32 theWord = m_words[word-1];

  // Shift to get the bit at bit0
  UInt32 shiftedWord = theWord >> (bit-1);
  
  // Is the bit set?
  return ( (shiftedWord & 1) > 0 );
}



unsigned int DoitStatusInfo::value(unsigned int word, unsigned int startBit,
				 unsigned int length) const
{
  // Get the word
  UInt32 theWord = m_words[word-1];
  
  // Shift the word to get the least significant bit at bit 0
  UInt32 shiftedWord = theWord >> (startBit-1);

  // Now make the mask
  UInt32 mask = 1;
  
  for ( unsigned int i = 1; i < length; ++i)
  {
    // Shift and add one
    mask <<= 1;
    mask += 1;
  }

  // Mask and return
  return ( shiftedWord & mask );
}


void DoitStatusInfo::reportFlag(ostream& s, const char* text, 
				DABoolean flag) const
{

  s << " " << text << "=";

  if ( flag ) 
  {
    s << "TRUE";
  }
  else
  {
    s << "false";
  }
}

void DoitStatusInfo::printout(ostream& s) const
{
  // Make a nice output report
  s << "DoitStatusInfo: \n";
  s << "   [UPPERCASE FLAGS ARE SET, lowercase flags are cleared]\n";

  // Determine the seed track type
  const char* seedTypeStrings[] = {
    "SF", "Alternate1", "Alternate2" };

  // Get the enum
  SeedType st = seedType();

  assert(st>=0 && st<=2);

  // Report it
  s << "   Seed Type: " << seedTypeStrings[(int)st] << '\n';
  
  // Report other SF stuff
  s << "   SF: Pass # = " << sfPassNumber() 
    << ", Merge Type = " << sfMergeType() 
    << ", Merge Indicator = " << sfMergeIndicator()
    << ", Chain # = " << sfChainNumber() << "\n";

  // Report information about the first wire hit
  s << "   1st wire Fit: # hits  = " << nHitsFirstWireFit();
  reportFlag( s, "D0_Constraint_Removed", d0ConstraintRemovedInWireFit() );
  reportFlag( s, "Tried_Adding_Kink", addingKinkTriedInWireFit() );
  reportFlag( s, "Continued_With_Kink", continuedWithKinkInWireFit() );
  s << "\n       ";
  reportFlag( s, "Continued_With_Kink_After_Close", 
	      continuedKinkAfterCloseInWireFit() );
  s << "\n";

  s << "   1st Hit Fit: ";  
  reportFlag( s, "tried_adding_kink", addingKinkTriedInHitFit() );
  reportFlag( s, "continued_with_kink", continuedWithKinkInHitFit() );
  reportFlag( s, "continued_kink_after_del&add", 
	      continuedKinkAfterDelAndAddInHitFit() );
  s << "\n";

  s << "   1st FAST_TRAK iteration: # hits from loc ambig: " <<
    nHitsFromLocalAmbig1() << "\n          ";
  reportFlag( s, "d0_constraint", d0Constraint1() );
  reportFlag( s, "z0_constraint", z0Constraint1() );
  reportFlag( s, "from_alt_ambig", fromAltAmbig1() );
  reportFlag( s, "alt_ambig_used_null", altAmbigUsedNull1() );
  s << "\n";

  s << "   2nd FAST_TRAK iteration: # hits from loc ambig: " <<
    nHitsFromLocalAmbig2() << "\n          ";
  reportFlag( s, "d0_constraint", d0Constraint2() );
  reportFlag( s, "z0_constraint", z0Constraint2() );
  reportFlag( s, "from_alt_ambig", fromAltAmbig2() );
  reportFlag( s, "alt_ambig_used_null", altAmbigUsedNull2() );
  s << "\n";

  // Get the rest
  s << "   ";
  reportFlag( s, "first_call_to_tfxfitcf", firstCallTFXFITCF() );
  reportFlag( s, "first_call_replaced_z_hits", replacedZHitsAfterFirstCall() );
  
  s << "\n   ";
  reportFlag( s, "Recalled_fast_trak", recalledFastTrak() );
  reportFlag( s, "first_fast_trak_recall", firstFastTrakRecall() );
  reportFlag( s, "second_fast_trak_recall", secondFastTrakRecall() );
  reportFlag( s, "recall_gave_new_repr", newRepresentationFromRecall() );

  s << "\n   ";
  reportFlag( s, "second_call_to_tfxfitcf", secondCallTFXFITCF() );
  reportFlag( s, "replaced_z_hits_after_2nd_call", 
                 replacedZHitsAfterSecondCall() );
  reportFlag( s, "tested_tfxfitlf_with_z0_constraint", 
	      testedTFXFITLFWithZ0Constraint() );

  s << "\n   ";

  reportFlag( s, "remove_z0_bias_after_cf", removeZ0BiasAfterCF() );
  reportFlag( s, "tested_tfxfitlf_with_normal_z0",
	      testedTFXFITLFWithZ0Normal() );

  s << "\n   ";

  reportFlag( s, "used_tfxfitlf_with_z0_constraint",
	      usedTFXFITLFWithZ0Constraint() );
  
  reportFlag( s, "boot_si_z", bootSiliconZ() );
  
  s << "\n";
}

//
// static member functions
//
