      SUBROUTINE ZD_SCALE_PHI_FIT_WT(LAYER,PHI,BEFORE_SCALE,AFTER_SCALE)
C 
C  begin_doc
C 
C   Documentation for subroutine scale_phi_fit_wt
C 
C   Purpose: rescales the desired quantity according to the shape of
C   -------- the phi-dependence of the DUET fitting weights.
C            Based on CleoII.5 routine crawm/phisca.F
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   LAYER        -- ZD layer number
C   PHI          -- PHI under consideration
C   BEFORE_SCALE -- unscaled quantity
C
C   Output Parameters: (Name - Meaning)
C   -----------------
C   AFTER_SCALE  -- scaled quantity
C   
C   Other routines:
C   ---------------
C 
C   Called By: MCResponseProd/Fortran/mc_zd_responses.F
C   ----------
C
C   Notes: This is due mainly to radiation damage of the 
C   ------ chamber so we will not use this initially be will need to look
C          into this in the future.
C
C   Author:   Guangshun Huang      Created:  Tue Dec 17, 2002
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

      INTEGER LAYER
      REAL PHI
      REAL BEFORE_SCALE, AFTER_SCALE
C 
C   Local User defined variables:
C   -----------------------------

C      REAL FIT_WEIGHT, SCALE_FACTOR
C
CC Functions
C      REAL   CG_PHI_WEIGHT
 
 

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_scale_phi_fit_wt.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:45 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_scale_phi_fit_wt' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine scale_phi_fit_wt:
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
        chmess='No phi-dependence corrections for ZD radiation damage'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

C     Phi corrections mainly due to radiation damage so
C     will need to revisit this issue in the future.
C
C     Get the duet (1 / fitting weight) phi correction
c      FIT_WEIGHT = CG_PHI_WEIGHT( LAYER, PHI )


C     Calculate the scale factor to use
C     PHI_RESCALE(LAYER) updated in Cleo2 via cleog/runs/cgphbr.F
C      SCALE_FACTOR = SQRT(FIT_WEIGHT) * PHI_RESCALE(LAYER)


C     Rescale
C      AFTER_SCALE = BEFORE_SCALE * SCALE_FACTOR
      
      return
      end
