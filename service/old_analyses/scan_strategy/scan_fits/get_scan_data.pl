#!/usr/local/bin/perl

die "Get on a SunOS!\n" if ( `uname` ne "SunOS\n" );

# for the Y(3s) first real scan

  @runs = ( 121887, 121889, 121890, 121891, 121892, 121895, 121897, 121898,

  	  121899, 121901, 121902, 121903, 121904, 121905, 121906, 121909,
  	  121911, 121912, 121914, 121915,

  	  121916, 121917, 121918, 121920, 121924, 121925, 121927, 121928,
  	  121929, 121930, 121932, 121933, 121935, 121936, 121938, 121939,
  	  121940, 121942, 121943, 121944 );

# for the second Y(3s) real scan

# @runs = ( 122069, 122070, 122071, 122072, 122073, 122074, 122076, 122077, 122079,
# 	  122064, 122066, 122080..122083, 122085, 122090..122094,
#           122097..122099, 122101, 122102, 122104, 122105, 122109 );

print "{\n";
foreach $run ( @runs )
{
    $energy = "n/a";
    $cchad = "n/a";
    $cchaderr = "n/a";

    open( CONST4, "/nfs/cleo3/Common/rel/current/bin/sparc-sun-solaris2.6/DBRunStatisticsRetrieve $run 4 |" );
    while( <CONST4> )
    {
	$energy = $1 if ( $_ =~ /^CESR\/energy:\s+([0-9\.\-]+)/ );
    }
    close( CONST4 );

    open( P1LUMI, "/home/dlk/Luminosities/lumi.105" );
    while( <P1LUMI> )
    {
	( $cchad, $cchaderr ) = ( $1, $2 )
	    if ( $_ =~ /^run$run\s+\*\s+CC Hadronic Cross Section \=\s+([0-9\.\-eE\+]+)\s+\+\-\s+([0-9\.\-eE\+]+)\s+nb/ );

    }
    close( P1LUMI );

    print "{ $run, $energy, $cchad, $cchaderr },\n"
	if ( $energy ne "n/a"  &&  $cchad ne "n/a"  &&  $cchaderr ne "n/a"  &&
	     $energy != 0      &&  $cchad != 0      &&  $cchaderr != 0 );
}
print " }\n";
