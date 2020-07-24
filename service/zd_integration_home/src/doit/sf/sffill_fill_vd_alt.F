*
* $Id: sffill_fill_vd_alt.F,v 1.1 2003/03/05 18:28:37 dpp Exp $
*
* $Log: sffill_fill_vd_alt.F,v $
* Revision 1.1  2003/03/05 18:28:37  dpp
*      -> NEW ROUTINE
*      -> split off from sffill, fill VD EAST for efficiency studies
*
*
*
      SUBROUTINE SFFILL_FILL_VD_ALT(HOWFIL,ILYRT,IADRL,IADRN)
C......................................................................
C.
C. SFFILL_FILL_VD_ALT - procedure to test east vd
C.
C. COMMON    : /STEPCn/
C. CALLS     : 
C. CALLED    : SFFILL
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : slit off from SFFILL 10-Feb-2003
C. LAST MOD  :
C......................................................................

#if defined(CLEO_TYPCHK)
      IMPLICIT NONE
#endif
      SAVE

C this will include tfindpar, cdgeompa, cdgeomcd, cdraw1in,
C                   tfctlcde, usbank,
C                   sfpar, sfcom, and sfsvcom
#define SF_DATA_INCLUDES
#include "doit/sf_data_access/sf_data_access.inc"
C----------
#define SF_DATA_DECLARE
#include "doit/sf_data_access/sf_data_access.inc"

C.......input arguments

C HOWFIL...1:FILL IN CHAIN, ONLY FOR FOUND HITS, PUT IN STANDARD CHAIN, INUCHN,
C          2:FILL IN EVEN MISSING HITS, AND VDEAST, PUT IN INUEFF
      INTEGER HOWFIL
C ILYRT....TEST LAYER
      INTEGER ILYRT
C IADRL....ADDRESS OF LAST ENCOUNTERED HIT IN CHAIN
      INTEGER IADRL
C IADRN....ADDRESS OF NEXT HIT IN CHAIN
      INTEGER IADRN

C...... local variable documentaion

C IADRT....ADDRESS OF TEST HIT,                      SF INDEXING
      INTEGER IADRT
C IGOT.....FLAG
      INTEGER IGOT
C ADRHCD...HIT ADDRESS OF THE TEST HIT,              CD INDEXING
      INTEGER ADRHCD
C OPEN_HIT.........FLAG THAT HIT IS USABLE
      INTEGER OPEN_HIT

C----------------------Executable code starts here---------------------


C WILL COME HERE FOR EVERY HIT, STORED IN "LE"
C ON THE LAST HIT, "HE" WILL BE SET TO =0 AND "LE" WILL
C STORE THE LAST HIT
C THIS IS NOT NEEDED EXECPT IN SFDIAG, FILL INUEFF

#if defined(CLEO_SFDIAG)
      IF(HOWFIL.EQ.2)THEN
        IF(
     1       (ILYRT.GT.MLSFPT).AND.
     2       (ILYRT.LE.MLSFVD))THEN
          IADRT=IADRL+NWRSF(ILYRT)
          IF(INUEFF(IADRT).EQ.0)THEN
            IGOT=-1
            
            sf_data_input_sf_adr=IADRT
#define SF_DATA_MAPV
#include "doit/sf_data_access/sf_data_access.inc"
            ADRHCD=sf_data_output_hit_adr
            
            IF(ADRHCD.GT.0)THEN
              
              sf_data_input_hit_adr=ADRHCD
#define SF_DATA_OPEN_HIT
#include "doit/sf_data_access/sf_data_access.inc"
              OPEN_HIT=sf_data_output_open_hit
              
              IF(OPEN_HIT.GE.0)IGOT=+1
            ENDIF

            ISTFOR(IADRL)=IADRT
            ISTFOR(IADRT)=IADRN
            IF(IADRN.NE.0)THEN
              ISTBAK(IADRN)=IADRT
            ELSE
              MINCHN(ICHAIN)=IADRT
            ENDIF
            ISTBAK(IADRT)=IADRL
            INUEFF(IADRT)=IGOT*ICHAIN
            
            IADRL=IADRT
            
            PRINT 1005,ILYRT,IADRT,ISTBAK(IADRT),ISTFOR(IADRT)
 1005       FORMAT(' SFFILL: DOING VDEAST ON LAYER',I7,
     1           ' ADDED CELL-ID',I6,' BETWEEN',I7,' AND',I7)
            
            
          ENDIF
        ENDIF
      ENDIF
#endif

      RETURN
      END






