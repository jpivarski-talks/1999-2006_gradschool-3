*
* $Id: zfgugp.inc,v 1.1 1998/05/07 20:39:42 pg Exp $
*
* $Log: zfgugp.inc,v $
* Revision 1.1  1998/05/07 20:39:42  pg
*   Added ZFILES subdirectory with copy of the files from CLEO II
* libraries. Increased size of the common in usbank.inc from 3500000 to
* 4200000 to accomodate large number of channels in Si3. Other files are
* unchanged.
*
* Revision 1.1.1.1  1994/10/04 20:58:43  zfiles
* first version of seq in CVS
*
*
*CMZ :  1.01/02 03/04/89  08.46.13  by  B. Heltsley
*-- Author : B. Heltsley
*
* zfgugp.inc
*
*************************** begin sequence ZFGUGP ************************
C.
C. ZFGUGP - This sequence gets and checks ZFILES constants pointers.
C.          Stringent error checking is done, the pointers are
C.          checked to see if new constants need to be loaded.
C.          This sequence is a shortcut for ZFGUID/ZFGPTR.
C.
C.          User must initialize 11 variables and include +SEQ,ZFGUGPSQ,
C.          +SEQ,MESREP, QUEST, as well as have the standard CRNAME
C.          initialized to the calling routine name. All statement numbers
C.          are in the 87700's and format statement numbers in the 97700's
C.          to avoid conflicts. All variables (declared in ZFGUGPSQ) end
C.          in the 2 letters ZF.
C.
C.......................................................................

* first call initialization
      IF( .NOT. CALDZF( INDFZF ) ) THEN

         CALDZF( INDFZF ) = .TRUE.

* get id lists from ZFILES
         CALL ZFGUID( TOPDZF( INDFZF ), NLSTZF( INDFZF ),
     *                IDLSZF( INDFZF ), PATHZF( INDFZF ),
     *                RWMOZF( INDFZF ), ACFRZF( INDFZF ),
     *                UKEYZF( INDFZF ), CMNTZF( INDFZF ), CRNAME )

* check for error in ZFILES id list
         IF( ICQUST( JZFERR ) .NE. 0 ) THEN

            WRITE( CHMESS, 97710, IOSTAT = IOERZF ) ICQUST( JZFERR )
97710       FORMAT(' ZFGUID returns error =', I10 )
            CALL MESLOG( CRNAME, 91, MSABOR )

         ENDIF

* zero pointers on first try
         CALL VZERO( IPLSZF( INDFZF ), NLSTZF( INDFZF ) )

      ENDIF

* check pointers to see if constants are invalid, if so get new ones
      NEWCZF = .FALSE.
      DO 87701 LOOPZF = INDFZF, INDFZF + NLSTZF( INDFZF ) - 1

* this will be true on first call
         IF( IPLSZF( LOOPZF ) .LE. 0 ) THEN
            NEWCZF = .TRUE.
         ELSE
* this will be true on new run with new constants
            IF( PZU( IPLSZF( LOOPZF ) ) .EQ. 0 ) NEWCZF = .TRUE.
         ENDIF
87701 CONTINUE

* get pointers to pointers for new constants
      IF( NEWCZF ) THEN

         CALL ZFGPTR( IDLSZF( INDFZF ), IPLSZF( INDFZF ),
     *                NWRDZF( INDFZF ), NLSTZF( INDFZF ) )

* check for error
         IF( ICQUST( JZFERR ) .NE. 0 ) THEN

            WRITE( CHMESS, 97720, IOSTAT = IOERZF ) ICQUST( JZFERR )
97720       FORMAT(' ZFGPTR returns error flag =', I10 )
            CALL MESLOG( CRNAME, 92, MSABOR )

         ENDIF

* check if number returned is number expected
         DO 87702 LOOPZF = INDFZF, INDFZF + NLSTZF( INDFZF ) - 1

            IF( MWRDZF( LOOPZF ) .NE. 0 .AND.
     *          NWRDZF( LOOPZF ) .NE. MWRDZF( LOOPZF ) ) THEN

               WRITE( CHMESS, 97730, IOSTAT = IOERZF ) PATHZF( LOOPZF )
97730          FORMAT(' Error for entity: ', A )
               CALL MESLOG( CRNAME, 93, MSERRO )

               WRITE( CHMESS, 97731, IOSTAT = IOERZF )
     *                  NWRDZF( LOOPZF ), MWRDZF( LOOPZF )
97731          FORMAT(' # words = ', I10, ' not equal to expected =',
     *                I10 )
               CALL MESLOG( CRNAME, 93, MSABOR )

            ENDIF
87702    CONTINUE
      ENDIF

* check that pointers are not zero
      DO 87703 LOOPZF = INDFZF, INDFZF + NLSTZF( INDFZF ) - 1

         IF( PZU( IPLSZF( LOOPZF ) ) .LE. 0 ) THEN

            WRITE( CHMESS, 97740, IOSTAT = IOERZF ) PATHZF( LOOPZF )
97740       FORMAT(' ZFILES ptr is <=0 for entity ', A )
            CALL MESLOG( CRNAME, 4, MSABOR )

         ENDIF
87703 CONTINUE
*************************** end sequence ZFGUGP ************************
