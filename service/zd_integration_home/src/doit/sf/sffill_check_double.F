*
* $Id: sffill_check_double.F,v 1.1 2003/03/05 18:28:06 dpp Exp $
*
* $Log: sffill_check_double.F,v $
* Revision 1.1  2003/03/05 18:28:06  dpp
*      -> NEW ROUTINE
*      -> split off from sffill, check fix poor assigment of primary in double
*
*
*
      SUBROUTINE SFFILL_CHECK_DOUBLE(HOWFIL,FIRSTH,ADHESF,LYRHE,WRHE)
C......................................................................
C.
C. SFFILL_CHECK_DOUBLE  - check for poor choice out of a double hit 
C.
C. COMMON    : /STEPCn/
C. CALLS     : 
C. CALLED    : SFFILL
C. AUTHOR    : D. Peterson
C. VERSION   : 1.00
C. CREATED   : split out of SFFILL 10-Feb-2003
C. LAST MOD  :
C.
C. Modification Log
C
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
C FIRSTH...LOGICAL FLAG OF FIRST HIT IN CHAIN
      LOGICAL FIRSTH
C ADHESF...ADDRESS OF HIGH END HIT,                  SF INDEXING
      INTEGER ADHESF

C.......output arguments

C LYRHE....LAYER OF HIGH END HIT
      INTEGER LYRHE
C WRHE.....WIRE OF HIGH END HIT
      INTEGER WRHE

C...... local variable documentaion

C LSFTMP...SF LAYER, temp
      INTEGER LSFTMP
C ADHECD...ADDRESS OF HIGH END HIT,                  CD INDEXING
      INTEGER ADHECD
C ADDOCD...ADDRESS OF HIGH END DOUBLE HIT,           CD INDEXING
      INTEGER ADDOCD
C TIMHIT...TDC TIME OF HIT
      INTEGER TIMHIT(2)
C IADRL....ADDRESS OF LAST ENCOUNTERED HIT IN CHAIN
      INTEGER IADRL
C IADRN....ADDRESS OF NEXT HIT IN CHAIN
      INTEGER IADRN
C IADRA....ADDRESS OF ADJACENT HIT
      INTEGER IADRA

C----------------------Executable code starts here---------------------

C this would be very dangerous if, in an earlier call, it had chosen
C to assign the same adjacent hit to two chains; do not flip for
C HOWFIL=1 AND do not assign the same adjacent hit in HOWFIL=1

      IF(HOWFIL.EQ.2)THEN

C this is done for the "HIGH END" before using for interpolation
C special routing here for the first hit
C this stuff is done for every hit; before filling in between hits

         IF(DOUBPT(ADHESF).GT.0)THEN

C select the low time for double hits
            LSFTMP=SFLSFA(ADHESF)
            ADHECD=ADHESF-INSFSF(LSFTMP)+INCDSF(LSFTMP)

            LSFTMP=SFLSFA(DOUBPT(ADHESF))
            ADDOCD=DOUBPT(ADHESF)-INSFSF(LSFTMP)+INCDSF(LSFTMP)

      sf_data_input_hit_adr=ADHECD
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      TIMHIT(1)=sf_data_output_tdc_raw

      sf_data_input_hit_adr=ADDOCD
#define SF_DATA_RTDC
#include "doit/sf_data_access/sf_data_access.inc"
      TIMHIT(2)=sf_data_output_tdc_raw

            IF(TIMHIT(1).GT.TIMHIT(2))THEN
               IADRL=ISTBAK(ADHESF)
               IADRN=ISTFOR(ADHESF)
               IADRA=DOUBPT(ADHESF)
               IF(IADRL.NE.0)ISTFOR(IADRL)=IADRA
               ISTBAK(IADRA)=IADRL
               ISTFOR(IADRA)=IADRN
               IF(IADRN.NE.0)ISTBAK(IADRN)=IADRA
               INUCHN(IADRA)=INUCHN(ADHESF)
               DOUBPT(IADRA)=ADHESF
C INUCHN(ADHESF) IS OK AS IS
               DOUBPT(ADHESF)=0
               ISTBAK(ADHESF)=0
               ISTFOR(ADHESF)=0
               ADHESF=IADRA
               IF(FIRSTH)LINCHN(ICHAIN)=IADRA
               IF(IADRN.EQ.0) MINCHN(ICHAIN)=IADRA
C end of requirement: TIMHIT(1).GT.TIMHIT(2)
            ENDIF

C end of requirement: existence of double hit
         ENDIF

#if defined(CLEO_SFDIAG)
C start filling the efficiency array
C do not fill the double hit
         INUEFF(ADHESF)=INUCHN(ADHESF)
#endif

C end of requirement: HOWFIL=2
      ENDIF

C load the layer and wire number
      LYRHE=SFLSFA(ADHESF)
      WRHE=SFWSFA(ADHESF)


      RETURN
      END






