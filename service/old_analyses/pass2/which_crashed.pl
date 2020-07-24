#!/usr/bin/perl

###########################################################################
# Important global variables
###########################################################################

$STATSDIR = "/home/pass2/Stats";
$PASS2DIR = "/home/pass2/Pass2Production/pass2/Pass2Scripts";

$n = $ARGV[0];
$varfile = "$STATSDIR/status.$n.pl";

###########################################################################
# Get the variables stored in "status.$n.pl"
# Syntax:
#   $log{$run}{$logversion} == [ $datetime ]
#   $logrecent{$run} == largest $logversion
#   @runs == sorted list of runs
#   $info{$run}{$varname} == value of variable
###########################################################################
$varcontents = "";
open( VARFILE, $varfile ) || die "Couldn't open $varfile.\n";
$datadate = "";
while( $_ = <VARFILE> )
{
    $varcontents .= $_;
    if ( $datadate eq ""  &&  $_ =~ /^\# Data gathered (.+)$/ )
    {
	$datadate = $1;
    }
}
close( VARFILE );
eval( $varcontents );
##########################################################################

%errors = ();

open( MAJORERRORS, "/home/pass2/Stats/majorErrors.$n.txt" );
$keepgoing = 0;
while( <MAJORERRORS> )
{
    if ( $_ eq "RunNum | Log |   Line | Line from Logfile\n" )
    { ; }

    elsif ( $_ eq "-------+-----+--------+-------------------------------------------------------------------------------------------\n" )
    { ; }

    elsif ( $_ =~ /^([0-9]{6}) \|\s+([0-9]{1,3})/o )
    {
	if ( $2 > 80 )
	{
	    $keepgoing = 1;
	    $run = $1;
	    $errors{$run} .= $_;
	}
	else
	{
	    $keepgoing = 0;
	}
    }

    elsif ( $keepgoing == 1 )
    {
	$errors{$run} .= $_;
    }

}
close( MAJORERRORS );

foreach $run ( @runs )
{
    if ( ( $info{$run}{"stillrunning"}  eq "done"          ||
	   $info{$run}{"stillrunning"}  eq "unresponsive"     )   &&
	 ( $info{$run}{"processtime"}   eq "n/a"   &&
	   $info{$run}{"numevents"}     eq "n/a"   &&
	   $info{$run}{"l4rej"}         eq "n/a"   &&
	   $info{$run}{"bharej"}        eq "n/a"   &&
	   $info{$run}{"bbblumi"}       eq "n/a"   &&
	   $info{$run}{"bbblumierr"}    eq "n/a"   &&
	   $info{$run}{"secperevent"}   eq "n/a"   &&
	   $info{$run}{"bharatio"}      eq "n/a"   &&
	   $info{$run}{"l4ratio"}       eq "n/a"              ) )
    {
	print "Run $run may have crashed. It is listed as being $info{$run}{stillrunning} with $info{$run}{fatalerrors} fatal errors.\n";
	if ( defined( $errors{$run} ) )
	{
	    print "    These are:
RunNum | Log |   Line | Line from Logfile
-------+-----+--------+-------------------------------------------------------------------------------------------
$errors{$run}\n";
	}
    }
}
