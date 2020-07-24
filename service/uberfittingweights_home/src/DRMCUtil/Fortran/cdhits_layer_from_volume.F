C 
C  $Id: cdhits_layer_from_volume.F,v 1.2 1999/11/22 01:14:17 jjo Exp $
C 
C  $Log: cdhits_layer_from_volume.F,v $
C  Revision 1.2  1999/11/22 01:14:17  jjo
C  Use geant 'det' number to give layer number as numbv isn't filled for DR
C
C  Revision 1.1  1999/10/20 21:50:32  jjo
C  Import routines for DR MC response simulation
C
C 
      INTEGER FUNCTION CDHITS_LAYER_FROM_VOLUME( GeantNUMBV,
     &                                           GeantISET,
     &                                           GeantIDET )

C 
C  begin_doc
C 
C   Documentation for function cdhits_layer_from_volume
C 
C   Purpose: Return DR layer number based on Geant info
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
C   Function returns DR layer number
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
C   DR Wire Layers.  The fact that each DRWireLayer is a seperate
C   Geant 'DET' the layer number corresponds to the DET number is
C   used to give the layer number.
C
C   Author:   John J O'Neill      Created:  Thu Sep 23 14:53:10 EDT 1999
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
      parameter(chrcss='$RCSfile: cdhits_layer_from_volume.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 1999/11/22 01:14:17 $')
      parameter(chrcsa='$Author: jjo $')
 
      character*(*) crname
      parameter(    crname = 'cdhits_layer_from_volume' )
 
C  end_var
C 
C   Data statements:
C   ----------------
      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine cdhits_layer_from_volume:
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
        chmess='Convert Geant volume to DR3 layer number'
        call report(MS_DEBUG, crname, chmess)
        lfirst=.false.
      endif

C     Return DET number
      CDHITS_LAYER_FROM_VOLUME = GeantIDET

      return
      end



