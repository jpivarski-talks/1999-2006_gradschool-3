*
* $Id: svaha100.inc,v 1.3 1998/09/29 18:35:18 pg Exp $
*
* $Log: svaha100.inc,v $
* Revision 1.3  1998/09/29 18:35:18  pg
*   Renamed common block to avoid conflicts.
*
* Revision 1.2  1998/07/29 20:57:16  pg
*  All elements numbered from 1 to N. Changed array's declaration to
* reflect this.
*
* Revision 1.1  1998/01/07 20:49:27  pg
*   Added files needed for SVX hit storage. Rearranged and modified
* svgeompa.inc and svlaseq.inc to make hit-related things work.
*
* Revision 1.1  1996/02/07 03:49:06  nk
* New common block to hold the first and last cluster # in svah
*
*
* Addition to svahc100.inc
* This is in a different file as it requires SVMWFR from svgeompa.inc
*
* The following defines the first and last cluster of a given wafer, side.
* You can loop over from minasv to maxasv but you should still check the
* wafer and side numbers of the cluster are right using WNOASV and ORIASV
* These variables are filled by svaaux
*
      INTEGER MINASV, MAXASV
      COMMON /SVAH03/ MINASV(NWAFER_SV3,0:1), 
     +     MAXASV(NWAFER_SV3,0:1)
      SAVE /SVAH03/







