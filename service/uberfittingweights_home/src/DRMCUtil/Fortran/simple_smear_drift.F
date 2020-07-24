C 
C  $Id: simple_smear_drift.F,v 1.2 2001/06/12 15:00:39 dubrovin Exp $
C 
C  $Log: simple_smear_drift.F,v $
C  Revision 1.2  2001/06/12 15:00:39  dubrovin
C  New generators of random numbers
C
C  Revision 1.1  2000/02/15 00:26:24  jjo
C  Import new routines into cvs
C
C 
      SUBROUTINE SIMPLE_SMEAR_DRIFT( Resolution,
     &                               DriftDist,
     &                               SmearedDriftDist )
C 
C  begin_doc
C 
C   Documentation for subroutine simple_smear_drift
C 
C   Purpose: Do a 'simple' simulation of drift distance smearing
C   --------
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   DriftDist - input drift distance
C   Resolution - sigma of Gaussian used to smear the drift distance
C
C   Output Parameters: (Name - Meaning)
C   -----------------
C   SmearedDriftDist - smeared drift distance
C
C   Other routines:
C   ---------------
C 
C   Called By: MCResponseProd/Fortran/mc_dr_responses.F
C   ----------
C
C   Notes:
C   ------
C 
C   Author:   John J O'Neill      Created:  Wed Feb  9 12:06:02 EST 2000
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
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      real Resolution, DriftDist, SmearedDriftDist
C 
C   Local User defined variables:
C   -----------------------------
C     Random1, Random2 - Normal random #'s from GRANOR
      real*8 Random1, Random2
      real*8 RANDOM_MODULE_GAUSS

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: simple_smear_drift.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/06/12 15:00:39 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'simple_smear_drift' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine simple_smear_drift:
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
 
      if (lfirst) then
        chmess='simple simulation of drift distance smearing'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

C     Get 2 normal random #'s - mean=0, sigma=1
C      CALL GRANOR( Random1, Random2 )
      Random1 =  RANDOM_MODULE_GAUSS()

C     Smear drift distance by Gaussian of mean=0, sigma=Resolution
      SmearedDriftDist = DriftDist + Resolution*Random1

      return
      end



