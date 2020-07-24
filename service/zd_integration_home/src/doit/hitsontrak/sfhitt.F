*
* $Id: sfhitt.F,v 1.5 2003/03/05 17:59:05 dpp Exp $
*
* $Log: sfhitt.F,v $
* Revision 1.5  2003/03/05 17:59:05  dpp
*      -> changed all variable names in cdscrtcd to have common root
*      -> use sf_data_access instead of direct access of cdscrtcd
*         this allows diresct access to the selected hit on multi-hit pads
*      -> change the name of the hit level arrays again, to RoadWirLst_
*      -> remove lyrsf_prev from sfhitt argument list;
*         zero with initialization call
*      -> implement the intermediate chain buffer for z slices
*      -> change sfhitt output code to logical
*      -> use EvntHit_OpnHit to indicate usable hits
*
* Revision 1.4  2002/09/05 18:58:55  dpp
*      -> change names of wire-level arrays to avoid confusion
*      -> diagnostic format
*
* Revision 1.3  2000/02/12 20:30:49  dpp
*      -> extra drift distance from cut set in SFTCUT
*
* Revision 1.2  1999/11/22 21:02:52  dpp
*  apply multiplyer to the cut for drift distance
*
* Revision 1.1.1.1  1998/02/06 19:11:44  dpp
* DOIT first release.
*
*
*
#include "sys/CLEO_machine.h"
#include "pilot.h"
*-- Author :D. Peterson
      SUBROUTINE SFHITT(CD_cell_id,LYRSF,LYRTF,ISER,IWIRE,GOOD)

C.......................................................................
C.
C. SFHITT - test hits from SFHITS
C.
C. COMMON    : ?
C. CALLS     :
C. CALLED    : Only by SFHITS
C. AUTHOR    : D. Peterson
C.
C. VERSION   : 1.00
C. CREATED   : 11-Jan-96
C. LAST MOD  :
C.
C. Modification Log.
C.
C.......................................................................
#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C Argument declarations

* CD_cell_id.....input..cell id of hit to test
* LYRSF..........input..SF LAYER
* LYRTF..........input..TF LAYER
* ISER...........input..(display only) SEQUENCE IN SEARCH
* IWIRE..........input..(display only) TESTED WIRE NUMBER
* GOOD...........output.FLAG IF WIRE IS ACCEPTED

      INTEGER CD_cell_id,LYRSF,LYRTF,ISER,IWIRE
      LOGICAL GOOD

C Other Variables

* IHIT_SAV.......save IHIT value of a good hit
      INTEGER IHIT_SAV

C----------
C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom, and cdscrtcd (if needed)
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"
C----------


#include "doit/duseq/tfgeomcd.inc"
#include "doit/duseq/tfhitscd.inc"


C== Local variables
C USE_SFChanList.....use the SFChanList_ intermediate chain list for 
C                    multiple z slices 
                     LOGICAL USE_SFChanList
C IHIT...............hit address returned from SF_DATA_ACCESS
                     INTEGER IHIT
C OPEN_HIT...........indicates if hit is previously used
                     INTEGER OPEN_HIT
C DRIFT..............drift distance returned from SF_DATA_ACCESS
                     REAL DRIFT
C DLIMIT.............DRIFT DISTANCE LIMIT
                     REAL DLIMIT
C LYRSF_PREV.........PREVIOUS SF LAYER
                     INTEGER LYRSF_PREV

C---------------------------------------------------------------------
C  Executable code starts here
C---------------------------------------------------------------------

C This is initialization, set the previously saved SF layer to zero

      IF(CD_cell_id.EQ.0)THEN
        LYRSF_PREV=0

      ELSE

C---------------------------------------------------------------------
C start normal execution
C---------------------------------------------------------------------

c any good hit will set this .true.
        GOOD=.FALSE.
        IHIT_SAV=0

C---------------------------------------------------------------------
C loop over the hits on the cell id
C this allows access to hits prime information is overwritten in 
C loop over z slices
C---------------------------------------------------------------------

#define SF_DATA_USE_ADDRESSED_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
        
        sf_data_input_cd_adr=CD_cell_id
#define SF_DATA_MAPV_CD
#include "doit/sf_data_access/sf_data_access.inc"
        IHIT=sf_data_output_hit_adr
        
C---------------------------------------------------------------------
C test if there is any information of the cell with cell level flag
C---------------------------------------------------------------------

 201    CONTINUE
        IF(IHIT.GT.0 )THEN
          
C---------------------------------------------------------------------
C test if hit is still open with hit level flag
C---------------------------------------------------------------------
          
          sf_data_input_hit_adr=IHIT
#define SF_DATA_OPEN_HIT
#include "doit/sf_data_access/sf_data_access.inc"
          OPEN_HIT=sf_data_output_open_hit
          
          IF(OPEN_HIT.GE.0)THEN
            
C---------------------------------------------------------------------
C tests of hit quality
C---------------------------------------------------------------------
            
            DLIMIT=.55*CELLTF(LYRTF)*RTF(LYRTF)+DRIFT_EXTRA_HI
            
            sf_data_input_hit_adr=IHIT
#define SF_DATA_DRIFT_DISTANCE
#include "doit/sf_data_access/sf_data_access.inc"
            DRIFT=sf_data_output_drift
            
            
#if defined(CLEO_SFHITDIAG)
            PRINT 5004,ISER,IWIRE,CD_cell_id,IHIT,
     2           OPEN_HIT,DRIFT,DLIMIT
 5004       FORMAT(' SFHITT:---------- SER',I3,' WIR#',I4,
     2           ' Cell ID=',I7,'  Hit Address=',I7,
     3           ' _OpnHit=',I3,'  _Drift=',F9.6,' <?',F9.6)
#endif
            IF(DRIFT.LE.DLIMIT)THEN
              GOOD=.TRUE.
              IHIT_SAV=IHIT
            ENDIF
            
          ELSE  
#if defined(CLEO_SFHITDIAG)
            PRINT 5005,ISER,IWIRE,CD_cell_id,IHIT,OPEN_HIT
 5005       FORMAT(' SFHITT:---------- SER',I3,' WIR#',I4,
     2           ' Cell ID=',I7,'  Hit Address=',I7,
     3           '  _OpnHIt=',I3)
#endif
          ENDIF

C---------------------------------------------------------------------
C loop back for another hit
C---------------------------------------------------------------------

          IF(MultiHitLayer(LYRTF))THEN

            sf_data_input_hit_adr=IHIT
#define SF_DATA_NEXTHIT_MULTIHIT
#include "doit/sf_data_access/sf_data_access.inc"
            IHIT=sf_data_output_next_mult

            GO TO 201
          ENDIF
        ELSE
          
#if defined(CLEO_SFHITDIAG)
          PRINT 5006,ISER,IWIRE,CD_cell_id,IHIT,GOOD,IHIT_SAV
 5006     FORMAT(' SFHITT:---------- SER',I3,' WIR#',I4,
     2         ' Cell ID=',I7,'  Hit Address=',I7,
     3         30X,' GOOD=',L1,' saved Hit Address=',I7)
#endif
          
        ENDIF
              
#define SF_DATA_USE_MULTIHIT_RESTORE
#include "doit/sf_data_access/sf_data_access.inc"
          
C---------------------------------------------------------------------
C tests of storage space; store the hit
C---------------------------------------------------------------------

        IF(GOOD)THEN  
          IF(RoadWirLst_Nhit.LT.NHITDM )THEN
            
C increment number of layers with hits for new layers 
            IF(LYRSF.NE.LYRSF_PREV)THEN
              LYRSF_PREV=LYRSF
              RoadWirLst_Nlyr=RoadWirLst_Nlyr+1
            ENDIF
            
C store the hit
            RoadWirLst_Nhit=RoadWirLst_Nhit+1
            RoadWirLst_adrs(RoadWirLst_Nhit)=IHIT_SAV
          ELSE
            GOOD=.FALSE.
          ENDIF
        ENDIF
          
C---------------------------------------------------------------------
C end of initialize/normal mode condition
C---------------------------------------------------------------------
      ENDIF
      END







