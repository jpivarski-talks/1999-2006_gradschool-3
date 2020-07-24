C 
C  $Id: ph_correction.F,v 1.5 2001/01/26 17:38:05 dubrovin Exp $
C 
C  $Log: ph_correction.F,v $
C  Revision 1.5  2001/01/26 17:38:05  dubrovin
C  Improve defenition of index in calibration array
C
C  Revision 1.4  1999/12/13 01:49:07  jjo
C  Use report for debug messages
C
C  Revision 1.3  1999/11/22 23:02:47  jjo
C  Improve debugging printout
C
C  Revision 1.2  1999/11/22 01:17:24  jjo
C  Print debug info if CPP flag DEBUG_RESPONSE is set
C
C  Revision 1.1  1999/10/26 18:41:48  jjo
C  Renamed ph_fudge.F to ph_correction
C
C  Revision 1.2  1999/10/20 21:41:27  jjo
C  Get DR MC Response resolution constants installed
C
C  Revision 1.1  1999/09/20 22:48:10  jjo
C  Import files into cvs
C
C 
      SUBROUTINE PH_CORRECTION(CellID, ILayer, DriftDist, PulseHeight)
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
C.  CellID   - DR cell ID
C.  ILayer   - CD layer number
C.  DriftDist  = Unsmeared drift distance (signed)
C.  PulseHeight  = MC generated pulse height
C 
C   Output Parameters: 
C   -----------------
C   PulseHeight  = Pulse height tuned to that of DATA
C
C   Other routines: DRMCUtil/Fortran/drmc_beginrun.F - fills constants
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
C   Author:   John J O'Neill      Created:  Thu Aug 12 15:00:43 EDT 1999
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

C     arraySize - Array size returned by drmc_xxxx function
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

C     IDriftBin - The bin in DR cell size, 20 bins for Phlayr constants
C     IEntry - Entry in Phlayr constants based on 
      INTEGER IDriftBin, IEntry

C     MAXBIN - number of bins in DCA for Phlayr
      INTEGER      MAXBIN
      PARAMETER   (MAXBIN = 20)

C     cellSize - Size of the current DR cell
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
      parameter(chrcss='$RCSfile: ph_correction.F,v $')
      parameter(chrcsr='$Revision: 1.5 $')
      parameter(chrcst='$State: Exp $')
      parameter(chrcsd='$Date: 2001/01/26 17:38:05 $')
      parameter(chrcsa='$Author: dubrovin $')
 
      character*(*) crname
      parameter(    crname = 'ph_correction' )
 

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
      call get_phlayr( NoConstantsUpdate, IEntry, LayerCor)

C     Get wire pulse height constant
      call get_phwire( NoConstantsUpdate, CellID, Wircor)

C     Here are the corrections.   Also, record the peds in case we need them.
      PHCorrection = LayerCor * Wircor

C     I'd think we should print a warning message when the phcor is negative ?
      IF ( PHCorrection .LT. 0. ) PHCorrection = 1.

      PulseHeight = PulseHeight * PHCorrection

C     Check pulse height
      call check_ph( CellID, PulseHeight )


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



