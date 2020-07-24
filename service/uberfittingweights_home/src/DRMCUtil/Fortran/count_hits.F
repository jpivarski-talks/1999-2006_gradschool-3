C 
C  $Id: count_hits.F,v 1.3 2002/08/07 21:26:38 bkh Exp $
C 
C  $Log: count_hits.F,v $
C  Revision 1.3  2002/08/07 21:26:38  bkh
C  Move to using guser_suez and Cleog3_suez in place of guser and Cleog3
C
C  Revision 1.2  1999/11/22 01:15:49  jjo
C  Routine not used now so comment out
C
C  Revision 1.1  1999/10/20 21:50:29  jjo
C  Import routines for DR MC response simulation
C
C 
      SUBROUTINE COUNT_HITS( SetIndex, ValidHitsBank, NHits )
C 
C  begin_doc
C 
C   Documentation for subroutine count_hits
C 
C   Purpose: Count DR hits in a given SET
C   --------  ==> NEEDS TO BE UPDATED TO LOOP OVER DETS <==
C             ==> ROUTINE NOT USED NOW <==
C
C
C
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   SetIndex - Loop over the different sets in the CD
C              This isn't needed for Cleo3 as there is only 1 set.
C              Keep it in just to get things working.
C
C   Output Parameters: (Name - Meaning)
C   -----------------
C   NHITS - number of hits found in given set
C   ValidHitsBank - false -> problem with the Zebra JHITS data structure
C 
C
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:
C   ------
C 
C   Author:   John J O'Neill      Created:  Tue Sep 21 14:50:28 EDT 1999
C   -------
C 
C   Detailed description:
C   ---------------------
C 
C   Major revisions:
C   ----------------
C      
C 
C  end_doc
C 
      IMPLICIT NONE
 
C  begin_inc
C   Include files :
C   ---------------------
C   Cleo3 Message Reporter
#include "ToolBox/report.inc"

C#include "geant321/gcbank.inc"
C
C#include "Cleog3_suez/cg3cdes/cgspec.inc"
C#include "Cleog3_suez/cg3cdes/cddspa.inc"
C#include "Cleog3_suez/cg3cdes/cddsco.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
C     ValidHitsBank - Flag for valid hits in set
      logical ValidHitsBank

      integer SetIndex, NHits

C 
C   Local User defined variables:
C   -----------------------------

CC     iset - # of Set
CC     idet - # of Det
C      integer iset, idet
C 
CC     cuset - GFHITS: Name of Set
CC     cudet - GFHITS: Name of Det
C      character*4 cuset, cudet
C
CC     totalHitWords - total # of words used for storing hits in IDET
C      integer     totalHitWords
C
CC     nwdet() - # of hits in det = TOTALHITWORDS/NW (should = NHITS )
C      integer nwdet(MXDETS)
C
CC     jh - Pointer to hits for set number ISET, = LQ(JHITS-ISET)
CC     js - Pointer to detector set number ISET, = LQ(JSET-ISET)
CC     jd - Pointer to detector number IDET,     = LQ(JS  -IDET)
CC     wordsPerHit - # of words per hit
C      integer jh, js, jd, wordsPerHit


C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: count_hits.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2002/08/07 21:26:38 $')
      parameter(chrcsa='$Author: bkh $')
 
      character*(*) crname
      parameter(    crname = 'count_hits' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine count_hits:
#if WRITE_RCS_INFO
      if(rcsini)then
        rcsini = .false.
        write(6,*)'-------- CVSinfo --------'
        write(6,*)chrcss
        write(6,*)chrcsr//' '//chrcst
        write(6,*)chrcsd//' '//chrcsa
      endif
#endif
C ----6----------------------------------------------------------------72
 
      chmess='Dummy routine for now.  Shoult not be called'
      call report(MS_EMERGENCY, crname, chmess)

C      if (lfirst) then
C
C        chmess='Count DR hits in a given SET'
C        call report(MS_DEBUG, crname, chmess)
C        lfirst=.false.
C      endif
C
Cc     Count the number of hits in set.
C      NHits = 0
C
CC     Valid device ?
C      if ( iset.gt.0 .and. idet.gt.0 ) then
C
Cc       Require hits in SET
C        if ( jhits .ne. 0 ) then 
CC         set flag for valid hits in SET 
C          ValidHitsBank=.true.
C
CC         Load pointers to hits in set, set, and det. (JH, JS, JD)
CC         JH - pointer to hits structure for set number ISET 
CC         totalHitWords - number of words used for storing the hits of detector number IDET 
C          jh = lq(jhits-iset)
C          totalHitWords  = iq(jh+idet)
C
CC         Require valid pointer & non-zero number of words stored
C          if ( jh.gt.0 .and. totalHitWords.gt.0 ) then
CC           js - pointer to the structure containing the description of set number ISET 
C            js = lq(jset-iset)
C            if ( js .eq. 0 ) then
C              chmess = 'JS is null'
C              call report(MS_EMERGENCY, crname, chmess)
C            endif
C              
CC           jd - pointer to detector description bank for IDET
C            jd = lq(js-idet)
C            if (   jd .eq. 0 ) then
C              chmess = 'JD is null.'
C              call report(MS_EMERGENCY, crname, chmess)
C            endif
C              
CC           wordsPerHit - not sure why
CC           iq(jd+1) - ???
CC           iq(jd+3) - # words in which a hit is stored
C            wordsPerHit = iq(jd+1) + iq(jd+3) + 1
CC             
C            nwdet(icddof(SetIndex)+id) = totalHitWords / wordsPerHit
C            NHits = NHits + nwdet(icddof(SetIndex)+id)
C          endif
C                
C        else
CC         set flag for no valid hits in SET 
C          ValidHitsBank=.false.
C        endif
C        
C      endif
C
C
C
C      if (ValidHitsBank) then
C        chmess='Found valid DR hits'
C      else
C        chmess='No valid DR hits found '
C      endif
C      call report(MS_DEBUG, crname, chmess)

      return
      end



