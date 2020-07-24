#!/usr/local/bin/perl

@tosubmit = ( "s5flat01.run",
	      "s5flat02.run",
	      "s5flat03.run", 
	      "s5flat04.run", 
	      "s5flat05.run", 
	      "s5flat06.run", 
	      "s5flat07.run", 
	      "s5flat08.run", 
	      "s5flat09.run", 
	      "s5flat10.run", 
	      "s5flat11.run", 
	      "s5flat12.run", 
	      "s5flat13.run", 
	      "s5flat14.run", 
	      "s5flat15.run", 
	      "s5flat16.run", 
	      "s5flat17.run", 
	      "s5flat18.run", 
	      "s5flat19.run", 
	      "s5flat20.run", 
	      "s5flat21.run", 
	      "s5flat22.run", 
	      "s5flat23.run", 
	      "s5flat24.run", 
	      "s5flat25.run", 
	      "s5flat26.run", 
	      "s5flat27.run", 
	      "s5flat28.run" );

#  while ( $#tosubmit > -1 ) {
#      print "qsub -v USER_TMP,HOME,OS_NAME -l calibration -cell rcf $tosubmit[0]\n";
#      @tosubmit = @tosubmit[1..$#tosubmit];
#  }

foreach $job ( @tosubmit ) {
    do {
	open( QSTAT, "qstat -u mccann -cell rcf |" );
	$queued = 0;
	while( <QSTAT> ) {
	    $queued++ if ( $_ =~ /QUEUED/ );
	}
	sleep (5*60) if ( $queued > 5 );

    } while ( $queued > 5 );

    system "qsub -v USER_TMP,HOME,OS_NAME -l calibration -cell rcf $job > /dev/null\n";
}

