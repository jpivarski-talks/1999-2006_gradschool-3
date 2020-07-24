#!/usr/bin/perl

###########################################################################
# Important global variables
###########################################################################
$STATSDIR = "/home/pass2/Stats";
$PASS2DIR = "/home/pass2/Pass2Production/pass2/Pass2Scripts";

$n = $ARGV[0];
$varfile = "$STATSDIR/majorErrors.$n.pl";

###########################################################################
# Get the variables stored in "majorErrors.$n.pl"
# Syntax:
#   $log{$run}{$logversion} == [ $datetime ]
#   $logrecent{$run} == largest $logversion
#   @runs == sorted list of runs
#   $info{$run}{$varname} == value of variable
###########################################################################
$varcontents = "";
open( VARFILE, $varfile ) || die;
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

#open( BROWSER, "> $STATSDIR/majorErr_web/browser.html" )  ||  die;
#print BROWSER "<html><body>
print "<html><body>

<h3>Data accumulated on $datadate.</h3>

<table>
";

sub numly { $a <=> $b; }

$prevrun = 0;
%alllines = ();
foreach $error ( @majorerrors )
{
    ( $run, $log, $text, $linesref ) = @{ $error };
    if ( $run != $prevrun )
    {
#	print BROWSER "</table><br><table>
#<tr><td><b>Run</b></td><td><b>Log</b></td><td><b>Line</b></td><td><b>Text of Line from Logfile</b></td></tr>
#";
	print "</table><br><table>
<tr><td><b>Run</b></td><td><b>Log</b></td><td><b>Line</b></td><td><b>Text of Line from Logfile</b></td></tr>
";
	&makeup_file( $prevrun, $log ) if ( $prevrun != 0 );
	%alllines = ();
    }

    @lines = sort numly @{ $linesref };
    chop( $text );
    $text = substr( $text, 0, 100 );
    foreach $line ( @lines )
    {
#	print BROWSER "<tr><td>$run</td><td>$log</td><td>$line</td><td><a href=\"log_$run-$log.html\#$line\" target=\"logview\">$text</a></td></tr>\n";
	print "<tr><td>$run</td><td>$log</td><td>$line</td><td><a href=\"log_$run-$log.html\#$line\" target=\"logview\">$text</a></td></tr>\n";
	$alllines{$line} = 1;
    }

    $prevrun = $run;
}

#print BROWSER "</table></body></html>
#";
print "</table></body></html>
";

#close( BROWSER );

sub makeup_file {
    local( $run, $log ) = @_;
    local( @lines ) = keys %alllines;

    if ( open( FROM, "$PASS2DIR/Log$log/$run\_pass2.log" ) )
    {
	open( TO, "> $STATSDIR/majorErr_web/log_$run\-$log.html" );

	print TO "<html><body><pre>
";
	$line = 1;
	while( $_ = <FROM> )
	{
	    chop( $_ );
	    local( $printit ) = 0;
	    foreach $l ( @lines )
	    {
		$printit = 1 if ( abs( $line - $l ) < 1000 );
	    }
	    
	    if ( $printit == 1 )
	    {
		$b = $bb = "";
		if ( defined( $alllines{$line} ) )
		{
		    print TO "<a name=\"$line\">";
		    $b = "<b>";
		    $bb = "</b>";
		}
		printf( TO "%8d : $b%s$bb\n", $line, $_ );
	    }
	    
	    $line++;
	}
	print TO "</pre></body></html>
";
	close( TO );
    }

    close( FROM );
}
