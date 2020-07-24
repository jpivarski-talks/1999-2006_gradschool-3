*
* $Id: cosmcom.inc,v 1.1.1.1 1998/02/06 19:11:50 dpp Exp $
*
* $Log: cosmcom.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:41  clib
* Developmental version of DUET.
*
* Revision 1.1.1.1  1994/10/08 01:01:12  zfiles
* first version of doit in CVS
*
*
*
* cosmcom.inc
*
      INTEGER T0PT,T0VD,T0DR,T0DC
      COMMON/SFCOSM/T0PT,T0VD(10),T0DR,T0DC
C
      INTEGER TCALLA,TCALWI,TCALNU,TCALT1,TCALT2,TCALTA
      COMMON/SFCOS2/TCALLA(800),TCALWI(800),TCALNU(800),
     1              TCALT1(800),TCALT2(800),TCALTA(800)

