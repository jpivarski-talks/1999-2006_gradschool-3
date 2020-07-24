#!/usr/bin/perl

###########################################################################
# Important global variables
###########################################################################

$STATSDIR = "/home/pass2/Stats";
$PASS2DIR = "/home/pass2/Pass2Production/pass2/Pass2Scripts";

$n = $ARGV[0];
$varfile = "$STATSDIR/status.$n.pl";
$webpage = "display.$n";

###########################################################################
# Get the variables stored in "status.$n.pl"
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

###########################################################################
# Make some statistical plots with paw
###########################################################################

open( RUNLIST, "> pawtmp_runlist.txt" );
open( LOADTIME, "> pawtmp_loadtime.txt" );
open( STARTUPTIME, "> pawtmp_startuptime.txt" );
open( PROCESSTIME, "> pawtmp_processtime.txt" );
open( SECPEREVENT, "> pawtmp_secperevent.txt" );
open( LUMINOSITY, "> pawtmp_luminosity.txt" );
open( BHARATIO, "> pawtmp_bharatio.txt" );
open( LFOURRATIO, "> pawtmp_l4ratio.txt" );

$numruns = 0;
foreach $run ( @runs )
{
    if ( $info{$run}{"loadtime"} ne "n/a"     &&
	 $info{$run}{"startuptime"} ne "n/a"  &&
	 $info{$run}{"processtime"} ne "n/a"  &&
	 $info{$run}{"secperevent"} ne "n/a"  &&
	 $info{$run}{"bbblumi"} ne "n/a"      &&
	 $info{$run}{"bharatio"} ne "n/a"     &&
	 $info{$run}{"l4ratio"} ne "n/a"      &&
         $info{$run}{"loadtime"} ne ""        &&
	 $info{$run}{"startuptime"} ne ""     &&
	 $info{$run}{"processtime"} ne ""     &&
	 $info{$run}{"secperevent"} ne ""     &&
	 $info{$run}{"bbblumi"} ne ""         &&
	 $info{$run}{"bharatio"} ne ""        &&
	 $info{$run}{"l4ratio"} ne ""            )
    {
	print RUNLIST $run . "\n";
	print LOADTIME $info{$run}{"loadtime"} . "\n";
	print STARTUPTIME $info{$run}{"startuptime"} . "\n";
	print PROCESSTIME $info{$run}{"processtime"} . "\n";
	print SECPEREVENT $info{$run}{"secperevent"} . "\n";
	print LUMINOSITY $info{$run}{"bbblumi"} . "\n";
	print BHARATIO $info{$run}{"bharatio"} . "\n";
	print LFOURRATIO $info{$run}{"l4ratio"} . "\n";
	$numruns++;
    }
}
close( RUNLIST );
close( LOADTIME );
close( STARTUPTIME );
close( PROCESSTIME );
close( SECPEREVENT );
close( LUMINOSITY );
close( BHARATIO );
close( LFOURRATIO );

open( PAW, "| paw > /dev/null" );
#open( PAW, "| paw" );
print PAW "
opt nbox
opt zfl1
opt sta

ve/read runlist 'pawtmp_runlist.txt'
ve/read loadtime 'pawtmp_loadtime.txt'
ve/read startuptime 'pawtmp_startuptime.txt'
ve/read processtime 'pawtmp_processtime.txt'
ve/read secperevent 'pawtmp_secperevent.txt'
ve/read luminosity 'pawtmp_luminosity.txt'
ve/read bharatio 'pawtmp_bharatio.txt'
ve/read l4ratio 'pawtmp_l4ratio.txt'

zone 1 2
";

foreach $thing ( "loadtime", "startuptime", "processtime", "secperevent",
		 "luminosity", "bharatio", "l4ratio" )
{
    print PAW "
h/cr/title '$thing VS. run'
graph $numruns runlist $thing
graph $numruns runlist $thing '*'
ve/plot $thing
pict/print '$webpage\_$thing.gif'
";
}

print PAW "exit\n";
close( PAW );

###########################################################################

###########################################################################
# Actually make the webpage
# tables are paged to make them load into browsers more quickly
###########################################################################
$page = 20;
open( WEBPAGE, "> $webpage.html" );

$tablestart = "<table><tr>
               <td><b>Run</b></td>
               <td><b>Log</b></td>
               <td><b>Logfile Date</b></td>
               <td><b>Latest Stop Reported</b></td>
               <td><b>Load time</b></td>
               <td><b>Startup time</b></td>
               <td><b>Process time</b></td>
               <td><b>Events</b></td>
               <td><b>Sec/Ev</b></td>
               <td><b>Disk Path</b></td>
               <td><b>Luminosity (BB)</b></td>
               <td><b>Bhabha Rej, Ratio</b></td>
               <td><b>L4 Rej, Ratio</b></td></tr>";

print WEBPAGE "<html><title>Log files parsed on $datadate</title><body>

<h3>Log files parsed on $datadate</h3>
<ul>

  <li><i>Italics</i> indicate that the job is still running.

  <li>Some of the \"Process Time\"s may be zero because the
  TimerModule is called a different number of times in some log files
  than in others.

</ul>

<h3>PAW statistics available for
  <a href=\"$webpage\_loadtime.gif\">load time</a>,
  <a href=\"$webpage\_startuptime.gif\">startup time</a>,
  <a href=\"$webpage\_processtime.gif\">process time</a>,
  <a href=\"$webpage\_secperevent.gif\">seconds per event</a>,
  <a href=\"$webpage\_luminosity.gif\">luminosity</a>,
  <a href=\"$webpage\_bharatio.gif\">bhabha rej/numevents ratio</a> and
  <a href=\"$webpage\_l4ratio.gif\">L4 rej/numevents ratio</a>.
";

sub fulldate {
    local( $totalsecs ) = @_[0];
    return "n/a" if ( $totalsecs eq "n/a" );

    local( $sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst )
	= localtime( $totalsecs );
    
    $year = $year % 100;
    $mday = sprintf( "%0d", $mday );
    $hour = sprintf( "%0d", $hour );
    $min = sprintf( "%0d", $min );
    $sec = sprintf( "%0d", $sec );

    return "$mday/$mon/$year $hour:$min:$sec";
}

sub atime {
    local( $totalsecs ) = @_[0];
    local( $d, $h, $m, $s );
    local( $out ) = "";

    $d = int( $totalsecs / 86400 );
    $totalsecs = $totalsecs % 86400;
    $out .= "$d day " if ( $d == 1 );
    $out .= "$d days " if ( $d > 1 );

    $h = int( $totalsecs / 3600 );
    $totalsecs = $totalsecs % 3600;
    $out .= "$h:" if ( $h != 0 );

    $m = int( $totalsecs / 60 );
    $totalsecs = $totalsecs % 60;
    $out .= "$m:";

    $s = $totalsecs;
    $out .= "$s";

    return $out;
}

sub littlefloat {
    local( $num ) = @_[0];
    return "n/a" if ( $num eq "n/a" );
    return sprintf( "%4.2f", $num );
}

sub bigfloat {
    local( $num ) = @_[0];
    return "n/a" if ( $num eq "n/a" );
    return sprintf( "%6.4f", $num );
}

$p = 0;
foreach $run ( reverse( @runs ) )
{
    print WEBPAGE $tablestart if ( $p == 0 );

    if ( $info{$run}{"stillrunning"} eq "done" )
    {
	$a = "<td>";   $z = "</td>";
    }
    else
    {
	$a = "<td><i>";   $z = "</i></td>";
    }

    print WEBPAGE "<tr>
               $a $run $z
               $a $logrecent{$run} $z
               $a " . &fulldate( $log{$run}{$logrecent{$run}}->[0] ) . " $z
               $a " . &fulldate( $info{$run}{"laststoptime"} ) . " $z
               $a " . &atime( $info{$run}{"loadtime"} ) . " $z
               $a " . &atime( $info{$run}{"startuptime"} ) . " $z
               $a " . &atime( $info{$run}{"processtime"} ) . " $z
               $a $info{$run}{numevents} $z
               $a " . &littlefloat( $info{$run}{"secperevent"} ) . " $z
               $a $info{$run}{diskpath} $z
               $a $info{$run}{bbblumi} +- $info{$run}{bbblumierr} $z
               $a $info{$run}{bharej}, " . &bigfloat( $info{$run}{"bharatio"} ) . " $z
               $a $info{$run}{l4rej}, " . &bigfloat( $info{$run}{"l4ratio"} ) . " $z
           </tr>
";

    $p++;
    if ( $p == $page )
    {
	print WEBPAGE "</table><pre>\n\n\n</pre>\n\n";
	$p = 0;
    }
}

print WEBPAGE "</body></html>
";
