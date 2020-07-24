*
* $Id: quest.inc,v 1.1 1998/05/07 20:39:38 pg Exp $
*
* $Log: quest.inc,v $
* Revision 1.1  1998/05/07 20:39:38  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.1.1.1  1994/10/04 20:58:43  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.01/31 11/03/90  10.33.28  by  Nobu Katayama
*CMZ :  1.01/05 22/06/89  13.30.35  by  B. Heltsley
* add JCLRUN to CQUEST
*CMZ :          12/04/89  11.32.06  by  R. Namjoshi
*-- Author :    R. Namjoshi   14/03/89
* ZEBRA error/info vector
*
      INTEGER    MQUEST
      PARAMETER( MQUEST = 100 )
      INTEGER        IQUEST
      COMMON /QUEST/ IQUEST(MQUEST)
      REAL           RQUEST(MQUEST)
      EQUIVALENCE(   RQUEST,IQUEST )
*
* CLEO error/info vector
*
      INTEGER    MCQUST
      PARAMETER( MCQUST = 100 )
      INTEGER         ICQUST
      COMMON /CQUEST/ ICQUST(MCQUST)
      REAL            RCQUST(MCQUST)
      EQUIVALENCE(    RCQUST,ICQUST )
* CLEVER
      INTEGER    NCLERR
      PARAMETER( NCLERR = 10                                        )
      INTEGER    JCLERR,      JCLRDT,      JCLRUN
      PARAMETER( JCLERR =  1, JCLRDT =  2, JCLRUN = 3               )
* ZFILES
      INTEGER    NZFERR
      PARAMETER( NZFERR = 20                                        )
      INTEGER    JZFERR,      JZFFND,      JZFNWD,      JZFIDT
      PARAMETER( JZFERR = 21, JZFFND = 22, JZFNWD = 23, JZFIDT = 24 )
*
* ZFILES constants information
*
      INTEGER    MZQUST
      PARAMETER (MZQUST = 100)
      INTEGER ZQPRID, ZQDATE, ZQTIME, ZQRNRG
* These variables are filled by ZFGPTR (XX(1) to XX(NLIST)), ZFGDAT (XX(1))
*     ZQPRID is the process id of the process which created the constants.
*     ZQDATE/ZFTIME is the creation date/time of the constants.
*     ZQRNRG is the run range of the constants.
*     for BR mode, ZQRNRG(1,1) is Begin run number,
*                  ZQRNRG(2,1) is End run number.
      COMMON /ZQQUEI/  ZQPRID(MZQUST), ZQDATE(MZQUST), ZQTIME(MZQUST),
     1                 ZQRNRG(2,2,MZQUST)
      SAVE   /ZQQUEI/
*
      CHARACTER*132 ZQCOMM
*     ZQCOMM is the comment associated with when the constants are created.
*     COMMNT is taken from the argument of ZFPDXX/ZFCDAT.
      CHARACTER*20  ZQAUTH, ZQPRCN, ZQMODU
*     ZQAUTH is the name of person who WROTE the constants.
*     To set the name, call ZFHTUA(NAME) before calling ZFPDXX/ZFCDAT.
*     ZQPRCN is the name of the process which wrote the constants.
*     To set the name, call ZFMNRP(NAME) before calling ZFPDXX/ZFCDAT.
*     ZQMODU is the name of the module which wrote the constants.
*     MODUNM is taken from the argument of ZFPDXX/ZFCDAT.
      COMMON /ZQQUEC/ ZQCOMM(MZQUST), ZQAUTH(MZQUST), ZQPRCN(MZQUST),
     1                ZQMODU(MZQUST)
      SAVE   /ZQQUEC/
