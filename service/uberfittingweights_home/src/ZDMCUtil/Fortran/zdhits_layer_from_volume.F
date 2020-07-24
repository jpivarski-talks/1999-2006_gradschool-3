      INTEGER FUNCTION ZDHITS_LAYER_FROM_VOLUME( GeantNUMBV,
     &                                           GeantISET,
     &                                           GeantIDET )

C 
C  begin_doc
C 
C   Documentation for function zdhits_layer_from_volume
C 
C   Purpose: Return ZD layer number based on Geant info
C   -------- See Notes: below.
C 
C   Input Parameters:  (Name - Meaning)
C   ----------------
C   GeantNUMBV - Geant volume number
C   GeantISET  - Geant set number
C   GeantIDET  - Geant det number
C   
C   Output Parameters: (Name - Meaning)
C   -----------------
C   Function returns ZD layer number
C
C   Other routines:
C   ---------------
C 
C   Called By: 
C   ----------
C
C   Notes:
C   ------
C   It seems that the Geant array NUMBV isn't filled for the 
C   ZD Wire Layers.  The fact that each ZDWireLayer is a seperate
C   Geant 'DET' the layer number corresponds to the DET number is
C   used to give the layer number.
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
      integer GeantNUMBV, GeantISET, GeantIDET
C 
C   Local User defined variables:
C   -----------------------------

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zdhits_layer_from_volume.F,v $')
      parameter(chrcsr='$Revision: 1.3 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/02/10 20:03:26 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zdhits_layer_from_volume' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine zdhits_layer_from_volume:
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
        chmess='Convert Geant volume to ZD layer number'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

C     Return DET number
      ZDHITS_LAYER_FROM_VOLUME = GeantIDET

      return
      end
