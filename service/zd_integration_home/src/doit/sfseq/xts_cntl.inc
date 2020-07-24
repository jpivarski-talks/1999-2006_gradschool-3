*
* $Id: xts_cntl.inc,v 1.1.1.1 1998/02/06 19:11:50 dpp Exp $
*
* $Log: xts_cntl.inc,v $
* Revision 1.1.1.1  1998/02/06 19:11:50  dpp
* DOIT first release.
*
* Revision 1.1.1.1  1997/04/30 12:31:42  clib
* Developmental version of DUET.
*
* Revision 1.2  1995/09/25 19:48:29  zfiles
* add variable to control TFXF display
* incorporated documentation
*
* Revision 1.1.1.1  1994/10/08  01:01:12  zfiles
* first version of doit in CVS
*
*
*CMZ :  6.00/02 28/07/93  09.53.14  by  Dan Peterson
*CMZ :  3.01/01 23/03/90  12.38.36  by  Dan Peterson
*-- Author :
* MORE COMMON BLOCKS FOR SF XTS ROUTINES
*
* xts_cntl.inc
*

#if !defined(CLEO_NOCOMDOC)

C XTS_ANSWER_IY...INTEGER VARIABLE RETURNED FROM XTS_RFV
C XTS_ABORT.......FLAG TO ABORT EVENT IN SF ROUTINES, GO TO NEXT EVENT
C XTS_NEW_TRACK...FLAG THAT THE SEED TRACK NUMBER HAS CHANGED
C XTS_TFXFIT_ON...FLAG THAT THE TFXFIT VIEW OPTION IS NEEDED

#endif

      INTEGER         XTS_ANSWER_IY,XTS_ABORT,XTS_NEW_TRACK,
     2                XTS_TFXFIT_ON
      COMMON/XTS_CNTL/XTS_ANSWER_IY,XTS_ABORT,XTS_NEW_TRACK,
     2                XTS_TFXFIT_ON




















