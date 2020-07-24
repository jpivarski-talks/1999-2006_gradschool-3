*
* $Id: zferror.inc,v 1.1 1998/05/07 20:39:41 pg Exp $
*
* $Log: zferror.inc,v $
* Revision 1.1  1998/05/07 20:39:41  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.1.1.1  1994/10/04 20:58:43  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.01/02 25/04/89  14.28.32  by  B. Heltsley
*-- Author : B. Heltsley
***********************************************************************
* ZFERROR: used to detect generic ZFILES error
* needs CRNAME, IOERR declared; sequences QUEST, MESREP
*
         IF( ICQUST( JZFERR ) .NE. 0 ) THEN
            WRITE( CHMESS, '('' ZFILES error ='', I10 )',
     *            IOSTAT = IOERR ) ICQUST( JZFERR )
            CALL MESLOG( CRNAME, 999, MSABOR )
         ENDIF
***********************************************************************
