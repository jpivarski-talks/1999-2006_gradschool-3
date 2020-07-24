C 
C  $Id: scale_phi_fit_wt.F,v 1.3 1999/10/27 14:54:19 jjo Exp $
C 
C  $Log: scale_phi_fit_wt.F,v $
C  Revision 1.3  1999/10/27 14:54:19  jjo
C  Comment out for now.  The phi dependence
C  of the fitting weights are mainly due to
C  radiation damage.  Will need to look at this
C  in the future for DR3.
C
C  Revision 1.2  1999/10/26 18:56:34  jjo
C  Improve docs - still a dummy routine
C
C  Revision 1.1  1999/09/20 22:48:11  jjo
C  Import files into cvs
C
C 
      SUBROUTINE SCALE_PHI_FIT_WT(LAYER, PHI, BEFORE_SCALE, AFTER_SCALE)
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
C   LAYER        -- DR layer number
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
C   Called By: MCResponseProd/Fortran/mc_dr_responses.F
C   ----------
C
C   Notes: This is due mainly to radiation damage of the 
C   ------ chamber so we will not use this initially be will need to look
C          into this in the future.
C
C   Author:   John J O'Neill      Created:  Thu Sep  2 12:27:54 EDT 1999
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
      parameter(chrcss='$RCSfile: scale_phi_fit_wt.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/10/27 14:54:19 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'scale_phi_fit_wt' )
 
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
        chmess='No phi-dependence corrections for DR3 radiation damage'
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



