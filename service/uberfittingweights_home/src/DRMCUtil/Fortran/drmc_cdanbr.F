CJJOC 
CJJOC  $Id: drmc_cdanbr.F,v 1.3 1999/10/26 18:33:55 jjo Exp $
CJJOC 
CJJOC  $Log: drmc_cdanbr.F,v $
CJJOC  Revision 1.3  1999/10/26 18:33:55  jjo
CJJOC  Comment out code as this routine isn't used anymore.
CJJOC  Will be retired soon.
CJJOC
CJJOC  Revision 1.2  1999/10/20 21:41:22  jjo
CJJOC  Get DR MC Response resolution constants installed
CJJOC
CJJOC  Revision 1.1  1999/09/20 22:48:07  jjo
CJJOC  Import files into cvs
CJJOC
C 
      SUBROUTINE DRMC_CDANBR
C 
C  begin_doc
C 
C   Documentation for subroutine drmc_cdanbr
C 
C   Purpose: 10/19/99 JJO - THIS IS AN OLD ROUTINE THAT HAS BEEN REPLACED.
C   -------- KEEP THIS AROUND TO MAKE SURE WE NOW CALC THE 
C            EFF_LAYER CONSTANTS CORRECTLY.   THIS IS DONE IN DRMCUtil/Fortran/get_layer_eff.F
C 
CJJOC   Input Parameters:  None
CJJOC   ----------------
CJJOC 
CJJOC   Output Parameters: None
CJJOC   -----------------
CJJOC 
CJJOC   Other routines:
CJJOC   ---------------
CJJOC 
CJJOC   Called By: 
CJJOC   ----------
CJJOC
CJJOC   Notes: Initialize DR3 Anode wire efficiency from constants database
CJJOC   ------
CJJOC 
CJJOC   Author:   John J O'Neill      Created:  Wed Aug 11 11:43:55 EDT 1999
CJJOC   -------
CJJOC 
CJJOC   Detailed description:
CJJOC   ---------------------
CJJOC 
CJJOC   Major revisions:
CJJOC   ----------------
CJJOC      
CJJOC 
CJJOC  end_doc
CJJOC 
CJJO      IMPLICIT NONE
CJJO 
CJJOC  begin_inc
CJJOC   Include files :
CJJOC   ---------------------
CJJOC   Cleo3 Message Reporter
CJJO#include "ToolBox/report.inc"
CJJO
CJJOC   8/12/99 jjo - Use CLEO2 CD GEOM to get system up and running
CJJOC                 Then convert to CLEO-III.
CJJOC                 But we are using Adam Lyon's routines to fill
CJJOC                 cdgeom for CLEO-III so this could cause me problems.
CJJOC   cdgeom & parameters  
CJJO#include "seq/cdgeom/cdgeompa.inc"
CJJO#include "seq/cdgeom/cdgeomcd.inc"
CJJO
CJJOC cleo3 Geometry - will need to switch to this eventually
CJJOC#include "cl3seq/cdgm3/cdgeompa.inc"
CJJOC cleo3 Geometry - will need to switch to this eventually
CJJOC#include "cl3seq/cdgm3/cdgeomcd.inc"
CJJO
CJJO
CJJOC   DR Anode average layer efficiencies
CJJO#include "DRMCUtil/Fortran/drmcAnodeLayerEff.inc"
CJJOC  end_inc
CJJO 
CJJOC  begin_var
CJJOC   input/output variables:
CJJOC   -----------------------
CJJO 
CJJOC 
CJJOC   Local User defined variables:
CJJOC   -----------------------------
CJJOC   NLayerDR - number of layers in drift chamber
CJJO      INTEGER NLayerDR
CJJO
CJJOC   arraySize - Array size returned by drmc_xxxx function
CJJOC   kWireffSize - Array size expected by WIREFF constants
CJJOC   kSPEFUD_SIZE - Array size expected by SPEFUD constants
CJJO      INTEGER kWireffSize, kSPEFUD_SIZE, arraySize
CJJO      PARAMETER ( kWireffSize=12240, kSPEFUD_SIZE=51)
CJJO
CJJOC...WIREFF - anode wire efficiency for each sense wire
CJJO      REAL WireEff(kWireffSize)
CJJO
CJJOC...SPEFUD - particle species dependent efficiency fudges
CJJO      REAL ElecFudge(kSPEFUD_SIZE),
CJJO     &     MuonFudge(kSPEFUD_SIZE),
CJJO     &     PionFudge(kSPEFUD_SIZE),
CJJO     &     KaonFudge(kSPEFUD_SIZE),
CJJO     &     ProtonFudge(kSPEFUD_SIZE) 
CJJO
CJJOC   Ilayer - loop over DR layers
CJJO      INTEGER Ilayer
CJJO
CJJOC   nWireLiveLayer - number of live wires in this layer
CJJO      INTEGER nWireLiveLayer
CJJO
CJJOC   SumEffLayer - sum of wire eff for all live wires in this layer
CJJO      REAL SumEffLayer
CJJO
CJJOC   IwireInLayer - loop over wires in this layer
CJJO      INTEGER IwireInLayer
CJJO
CJJOC   IwireCellID - cell Id counting all wires from 1st layer
CJJOC                
CJJO      INTEGER IwireCellID
CJJO
CJJOC   LdeadWire - TRUE if given wire is dead
CJJO      LOGICAL LdeadWire
CJJO
CJJOC   Local pre-defined variables:
CJJOC   ---------------------------
CJJOC   chmess - Text of current message
CJJO      CHARACTER*132 chmess
CJJO
CJJO      LOGICAL LFIRST, RCSINI
CJJO      CHARACTER*(*) CHRCSS, CHRCSR, CHRCST, CHRCSD, CHRCSA
CJJO      PARAMETER(CHRCSS='$RCSfile: drmc_cdanbr.F,v $')
CJJO      PARAMETER(CHRCSR='$Revision: 1.3 $')
CJJO      PARAMETER(CHRCST='$State: Exp $')
CJJO      PARAMETER(CHRCSD='$Date: 1999/10/26 18:33:55 $')
CJJO      PARAMETER(CHRCSA='$Author: jjo $')
CJJO 
CJJO      CHARACTER*(*) CRNAME
CJJO      PARAMETER(    CRNAME = 'DRMC_CDANBR' )
CJJO 
CJJOC  end_var
CJJOC 
CJJOC   Data statements:
CJJOC   ----------------
CJJO      SAVE RCSINI,LFIRST
CJJO      DATA RCSINI/.TRUE./, LFIRST/.TRUE./
CJJO 
CJJOC   executable code for routine drmc_cdanbr:
CJJO#if WRITE_RCS_INFO
CJJO      IF(RCSINI)THEN
CJJO        RCSINI = .FALSE.
CJJO        WRITE(6,*)'-------- CVSinfo --------'
CJJO        WRITE(6,*)CHRCSS
CJJO        WRITE(6,*)CHRCSR//' '//CHRCST
CJJO        WRITE(6,*)CHRCSD//' '//CHRCSA
CJJO      ENDIF
CJJO#endif
CJJOC ----6----------------------------------------------------------------72
CJJO 
CJJO      if (LFIRST) then
CJJO        chmess='Initialize Anode wire efficiency via constants database'
CJJO        call report(MS_INFO, CRNAME, chmess)
CJJO        LFIRST=.false.
CJJO      endif
CJJO
CJJOC.....Rename CLEO-II cdgeom variables that make no sense
CJJOC     NCANCD - Number of CD anode layers
CJJO      NLayerDR = NCANCD
CJJO
CJJO
CJJOC.....Get WireEff Constants - the anode wire efficiency constants.
CJJOC     Then find the average efficiency of each layer from 
CJJOC     the live wires.  This will be stored in effanode.inc 
CJJOC     and used in CDREFF.
CJJOC     The average layer eff. should be stored in the database 
CJJOC     instead of doing this calculation.  Need to implement this.
CJJO
CJJOC     WireEff Constants - Get anode wire efficiency
CJJOC     via drmc_wireff in DRMCUtil/Class/DRMCWireffFB.cc
CJJOC     drmc_wireff is C++ interface routine that gets the 
CJJOC     constants from the "Frame"
CJJOC     Request kWireffSize elements to be filled      
CJJO      arraySize = kWireffSize
CJJO      call drmc_wireff( WireEff, arraySize )
CJJOC     If arraySize changes somethings wrong - could cause memory overwrite
CJJO      if ( arraySize .NE. kWireffSize ) then
CJJO        write(chmess,*) 'arraySize changed, ', arraySize
CJJO        call report(MS_EMERGENCY, CRNAME, chmess)
CJJO      endif
CJJO
CJJO
CJJOC     Loop over anode wires
CJJO      do Ilayer = 1, NLayerDR
CJJOC       Reset number of live wires in this layer 
CJJO        nWireLiveLayer = 0
CJJOC       Reset total eff. for this layer which is a sum of all live wires
CJJO        SumEffLayer = 0.0
CJJO
CJJOC       Loop over wires in this layer, calc ave. eff. for layer
CJJO        do IwireInLayer = 0, NWIRCD(Ilayer)-1
CJJO
CJJOC         Get wire CellID - counting all wires from 1st layer
CJJOC         INDXCD - IADRHW address of wire # 0 of CD layer #
CJJO          IWireCellID = INDXCD(Ilayer) + IwireInLayer 
CJJO
CJJOC         Is given wire dead or alive ?
CJJOC         DEAD_WIRE Needs to be implemented for CLEO-III
CJJOC         Should we just fill an array instead of a function call ?
CJJOc          CALL DR_DEAD_WIRE( , LdeadWire)
CJJOC         Hard wire dead wire for now
CJJO          LdeadWire = .FALSE.
CJJO          if ( .not. LdeadWire ) then
CJJO            nWireLiveLayer = nWireLiveLayer + 1
CJJO            SumEffLayer = SumEffLayer + WireEff(IWireCellID)
CJJO          endif
CJJO        enddo
CJJO
CJJOC       Calculate average LIVE wire efficiency for this layer 
CJJOC       Save in EFF_LAYER in EFFANODE common block
CJJO        if ( nWireLiveLayer .gt. 0 ) then
CJJO          EFF_LAYER(Ilayer) = SumEffLayer/FLOAT(nWireLiveLayer)
CJJO        else
CJJO          EFF_LAYER(Ilayer) = 0.0
CJJO        endif
CJJO
CJJO      enddo
CJJO
CJJO
CJJOC.....Get SPEFUD Constants - particle species dependent efficiency fudges
CJJOC     via drmc_spefud in DRMCUtil/Class/DRMCSpefudFB.cc
CJJOC     Request kSPEFUD_SIZE elements to be filled      
CJJO      arraySize = kSPEFUD_SIZE
CJJO      call drmc_spefud( ElecFudge,
CJJO     &                  MuonFudge,
CJJO     &                  PionFudge,
CJJO     &                  KaonFudge,
CJJO     &                  ProtonFudge,
CJJO     &                  arraySize )
CJJOC     If arraySize changes somethings wrong - could cause memory overwrite
CJJO      if ( arraySize .ne. kSPEFUD_SIZE ) then
CJJO        write(chmess,*) 'arraySize changed, ', arraySize
CJJO        call report(MS_EMERGENCY, CRNAME, chmess)
CJJO      endif
CJJO
CJJO
CJJOC     Fill SPEFUD common block variables in effanode.inc
CJJO      do Ilayer = 1, NLayerDR
CJJO        SPEFDG(Ilayer,1) = ElecFudge(Ilayer)
CJJO        SPEFDG(Ilayer,2) = MuonFudge(Ilayer)
CJJO        SPEFDG(Ilayer,3) = PionFudge(Ilayer)
CJJO        SPEFDG(Ilayer,4) = KaonFudge(Ilayer)
CJJO        SPEFDG(Ilayer,5) = ProtonFudge(Ilayer)
CJJO      enddo
      
      return
      end



