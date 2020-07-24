*
* $Id: doitloadsvhits.F,v 1.3 2000/06/27 21:15:16 pg Exp $
*
* $Log: doitloadsvhits.F,v $
* Revision 1.3  2000/06/27 21:15:16  pg
* Added protection against TOO MANY SI HITS case
*
* Revision 1.2  2000/05/15 18:31:05  pg
*    Changes to accomodate modifications to svscratch.inc .
*
* Revision 1.1  2000/03/28 15:59:30  pg
* Now filling Si common blocks
*
*
*
      SUBROUTINE DOITLOADSVHITS(hittype,index, parent, layer, hybrid,
     &     qualsv, xsv, ysv, zsv, phisv, pulsesv, o_idx)
*
* begin_doc
*
*  Documentation for subroutine doitloaddrhits
*
*  Purpose:
*  --------
*
*  Input Parameters:  (Name - Type - Meaning)
*  ----------------
*
*  Output Parameters:  (Name - Type - Meaning)
*  -----------------
*
*  Other routines:
*  ---------------
*
*  Notes:
*  ------
*
*  Author:   Peter Gaidarev      Created:  Mon Mar 20 14:08:16 EDT 2000
*  -------
*
*  Major revisions:
*  ----------------
*     
*
* end_doc
*
#if defined(CLEO_TYPECHEK)
      IMPLICIT NONE
#endif
*
* begin_inc
*  Include files :
*  ---------------------
#include "seq/clutil/mesrep.inc"
#include "doit/duseq/svscratch.inc"
* end_inc
*
* begin_var
*  input/output variables:
*  -----------------------
      INTEGER hittype, INDEX, parent, layer, hybrid, qualsv, o_idx
      REAL xsv,ysv,zsv,phisv,pulsesv
*
*  Local pre-defined variables:
*  ---------------------------
      LOGICAL RCSINI
      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
      PARAMETER(CHRCSS='$RCSfile: doitloadsvhits.F,v $')
      PARAMETER(CHRCSR='$Revision: 1.3 $')
      PARAMETER(CHRCST='$State: Exp $')
      PARAMETER(CHRCSD='$Date: 2000/06/27 21:15:16 $')
      PARAMETER(CHRCSA='$Author: pg $')
*
      CHARACTER*(*) CRNAME
      PARAMETER(    CRNAME = 'DOITLOADSVHITS' )
*
*  Local User defined variables:
*  -----------------------------
      INTEGER i
      INTEGER RPHIHIT
      PARAMETER( RPHIHIT = 0 )
*
*
* end_var
*
*  Data statements:
*  ----------------
*
      SAVE RCSINI
*
      DATA RCSINI/.TRUE./

*
*  executable code for routine doitloaddrhits:
#if WRITE_RCS_INFO
      IF(RCSINI)THEN
        RCSINI = .FALSE.
        WRITE(6,*)'-------- CVSinfo --------'
        WRITE(6,*)CHRCSS
        WRITE(6,*)CHRCSR//' '//CHRCST
        WRITE(6,*)CHRCSD//' '//CHRCSA
      ENDIF
#endif
*----6----------------------------------------------------------------72
*


        if ( SV_NHITS+1 .gt. MHITSV ) then
          CHMESS='Doit -- Number of Si hits exceeded MHITSV!'
          CALL MESLOG(CRNAME, 1, MSWARN)
          o_idx = 0 
          RETURN
        endif
        
        SV_NHITS = SV_NHITS + 1

        i = SV_NHITS
        
C*****Copy hit information into the common block
        SV_TYPE(I) = HITTYPE
        SV_INDEX(I) = INDEX
        SV_PARENT(I)  = PARENT
        SV_LAYER(I) = layer
        SV_HYBRID(I)  = hybrid
        SV_QUALITY(I)  = qualsv
        SV_X(I) = xsv
        SV_Y(I) = ysv
        SV_Z(I) = zsv
        SV_PHI(I) = phisv
        SV_PULSE(i)   = pulsesv
        
        o_idx = i
        
c        WRITE(6,*) 'DOITLOADSVHITS: RPHI ',I,LAYER,HYBRID
        

      RETURN
      END
