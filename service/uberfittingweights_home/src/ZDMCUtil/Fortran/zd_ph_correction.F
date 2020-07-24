      SUBROUTINE ZD_PH_CORRECTION(CellID,ILayer,DriftDist,PulseHeight)
C 
C  begin_doc
C 
C   Documentation for subroutine ph_correction
C 
C   Purpose: Apply wire-by-wire mean pulseheight corrections,
C   -------- was cleog/crawm/phfudg.F in Cleo-II simulation.
C 
C   Input Parameters:  
C   ----------------
C.  CellID   - ZD cell ID
C.  ILayer   - CD layer number
C.  DriftDist  = Unsmeared drift distance (signed)
C.  PulseHeight  = MC generated pulse height
C 
C   Output Parameters: 
C   -----------------
C   PulseHeight  = Pulse height tuned to that of DATA
C
C   Other routines: ZDMCUtil/Fortran/zdmc_beginrun.F - fills constants
C   --------------- at each begin run.
C 
C   Called By: 
C   ----------
C
C   Notes: 
C   ------
C   Based on CLEO-II routine cleog/crawm/phfudg.F by Craig Prescott 
C
C
C   Changes from CLEO-II routine:
C   Pass in ILayer of the wire so this routine doesn't have to calculate it
C
C  This routine applies multiplicative scale factors
C  from constants database to the input pulse height.
C
C  Constants are produced by fitting plots of path length corrected 
C  pulseheight in 20 bins of signed DCA to the wire to a landau for
C  each layer; the peak positions are extracted from the data and 
C  compared to that of monte carlo.  The monte carlo gets scaled up or 
C  down as necessary.
C
C  On top of the layer correction as a function of DCA, there is a wire
C  correction; this is extracted by fitting path length corrected 
C  pulseheight for each wire (over all dca) to a landau for data and monte
C  carlo.  The correction is again data/mc of the landau peak position.
C
C  The layer corrections are indexed by layer in the device; the wire 
C  corrections by wire in the device.
C
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

C   cdgeom includes
#include "cl3seq/cdgm3/cdgeompa.inc"
#include "cl3seq/cdgm3/cdgeomcd.inc"
C  end_inc
 
C  begin_var
C   input/output variables:
C   -----------------------
      INTEGER  CellID, ILayer
      REAL     DriftDist, PulseHeight 
 
C 
C   Local User defined variables:
C   -----------------------------

C     arraySize - Array size returned by zdmc_xxxx function
C     kPhlayrSize - Array size expected by PHLAYR constants
C     kPHWIRE_SIZE - Array size expected by PHWIRE constants
      INTEGER kPhlayrSize, kPhwireSize, arraySize
      PARAMETER ( kPhlayrSize=1020, kPhwireSize=12240)

C     LayerCor - layer Pulse height conversion factor constants
      REAL LayerCor

C     Wircor - Wire to wire pulse height correction
      REAL Wircor

C     DriftFrac - Convert DCA to fraction of cell
      REAL DriftFrac

C     IDriftBin - The bin in ZD cell size, 20 bins for Phlayr constants
C     IEntry - Entry in Phlayr constants based on 
      INTEGER IDriftBin, IEntry

C     MAXBIN - number of bins in DCA for Phlayr
      INTEGER      MAXBIN
      PARAMETER   (MAXBIN = 20)

C     cellSize - Size of the current ZD cell
      real cellSize

C     NoConstantsUpdate - tell constants access subroutines to update constants
      logical NoConstantsUpdate
      parameter ( NoConstantsUpdate = .false. )

C     PHCorrection - pulse height correction from Phlayr & Phwire constants
      real PHCorrection

      real PI
      parameter (PI = 3.141592654 )

C   Local pre-defined variables:
C   ---------------------------
C   chmess - Text of current message
      character*132 chmess

      logical lfirst, rcsini
      character*(*) chrcss, chrcsr, chrcst, chrcsd, chrcsa
      parameter(chrcss='$RCSfile: zd_ph_correction.F,v $')
      parameter(chrcsr='$Revision: 1.2 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2003/04/24 14:40:45 $')
      parameter(chrcsa='$Author: huanggs $')
 
      character*(*) crname
      parameter(    crname = 'zd_ph_correction' )
 

C  EXTERNAL declarations
      INTEGER  KQOFPH
      EXTERNAL KQOFPH
      REAL     PHOFKQ
      EXTERNAL PHOFKQ

C  end_var
C 
C   Data statements:
C   ----------------

      save rcsini,lfirst
      data rcsini/.true./, lfirst/.true./
 
C   executable code for routine ph_correction:
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
        chmess='Correct mean pulseheight wire-by-wire'
        call report(MS_INFO, crname, chmess)
        lfirst=.false.
      endif


C.....Find out which bin 
C     Convert DCA to fraction of cell
C     cellSize (actually the half-cell size) = 1/2 (2piR/# wires in layer)  
*      cellSize = PI*RCD(ILAYER)/NWIRCD(ILAYER)
      DriftFrac = DriftDist * NWIRCD(ILAYER) / (PI*RCD(ILAYER))

C     Get the right DCA bin.  
C     1) get bin that corresponds to this drift distance
      IDriftBin = MIN(INT((DriftFrac + 1.)*MAXBIN*0.5+1), MAXBIN)
      IDriftBin = MAX(IDriftBin, 1)
C     2) get layer offset
      IEntry = IDriftBin + (ILAYER - 1)*MAXBIN

C     Get layer pulse height constant
      call zd_get_phlayr( NoConstantsUpdate, IEntry, LayerCor)

C     Get wire pulse height constant
      call zd_get_phwire( NoConstantsUpdate, CellID, Wircor)

C     Here are the corrections.   Also, record the peds in case we need them.
      PHCorrection = LayerCor * Wircor

C     I'd think we should print a warning message when the phcor is negative ?
      IF ( PHCorrection .LT. 0. ) PHCorrection = 1.

      PulseHeight = PulseHeight * PHCorrection

C     Check pulse height
      call zd_check_ph( CellID, PulseHeight )


#if defined(DEBUG_RESPONSE)
      chmess = ' ------- begin PH_CORRECTION debug info -------'
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'ILayer=',ILayer
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'DriftDist=',DriftDist
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'IEntry=',IEntry
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'LayerCor=', LayerCor
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'CellID=',CellID
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Wircor=',Wircor
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'PHCorrection=', PHCorrection
      call report(MS_INFO, crname, chmess)       
      write(chmess,*) 'Corrected PulseHeight=',PulseHeight 
      call report(MS_INFO, crname, chmess)       
      chmess = ' ------- end PH_CORRECTION debug info -------'
      call report(MS_INFO, crname, chmess)       
#endif
      
      return
      end
