#!/usr/bin/perl

# For reverse-engineering the timestamp
use Time::Local;
%months = ( "Jan" => 0, "Feb" => 1, "Mar" => 2, "Apr" => 3, "May" => 4, "Jun" => 5,
	    "Jul" => 6, "Aug" => 7, "Sep" => 8, "Oct" => 9, "Nov" => 10, "Dec" => 11 );
%shtnom = ( 0 => "Jan", 1 => "Feb", 2 => "Mar", 3 => "Apr", 4 => "May", 5 => "Jun",
	    6 => "Jul", 7 => "Aug", 8 => "Sep", 9 => "Oct", 10 => "Nov", 11 => "Dec" );

###########################################################################
# Important global variables
###########################################################################
$STATSDIR = "/home/pass2/Stats";
$PASS2DIR = "/home/pass2/Pass2Production/pass2/Pass2Scripts";

$n = $ARGV[0];
if ( $n !~ /^[0-9]+$/ )
{
    $n = 0;
    open( LS, "ls $STATSDIR |" );
    while( <LS> )
    {
	if ( $_ =~ /status\.([0-9]+)\.pl/ )
	{
	    $n = $1 if ( $n < $1 );
	}
    }
}
$varfile = "$STATSDIR/status.$n.pl";

###########################################################################
# Get the variables stored in "status.$n.pl"
# Syntax:
#   $log{$run}{$logversion} == [ $datetime ]
#   $logrecent{$run} == largest $logversion
#   @runs == sorted list of runs
#   $info{$run}{$varname} == value of variable
###########################################################################

$norecurse = 0;
require( $varfile );
sub numly { $a <=> $b; }
@runs = sort numly keys %log;

# $varcontents = "";
# open( VARFILE, $varfile ) || die;
# $datadate = "";
# while( $_ = <VARFILE> )
# {
#     $varcontents .= $_;
#     if ( $datadate eq ""  &&  $_ =~ /^\# Data gathered (.+)$/ )
#     {
# 	$datadate = $1;
#     }
# }
# close( VARFILE );
# eval( $varcontents );
##########################################################################

###########################################################################
# Write out to the files which are parsed by the web script
###########################################################################
print " * * * * * * * * * * * * * * * * Writing out to files for web script\n";

open( NOTSTILLRUNNING, "> $STATSDIR/not_stillrunning.txt" );  print "    Opened $STATSDIR/not_stillrunning.txt for writing.\n";
open( RUNNUMBER, "> $STATSDIR/runnumber.txt" );            print "    Opened $STATSDIR/runnumber.txt for writing.\n";
open( NOTRUNNUMBER, "> $STATSDIR/not_runnumber.txt" );     print "    Opened $STATSDIR/not_runnumber.txt for writing.\n";
open( LOGFILE, "> $STATSDIR/logfile.txt" );                print "    Opened $STATSDIR/logfile.txt for writing.\n";
open( NOTLOGFILE, "> $STATSDIR/not_logfile.txt" );         print "    Opened $STATSDIR/not_logfile.txt for writing.\n";
open( BHARATIO, "> $STATSDIR/bharatio.txt" );              print "    Opened $STATSDIR/bharatio.txt for writing.\n";
open( BHAREJ, "> $STATSDIR/bharej.txt" );                  print "    Opened $STATSDIR/bharej.txt for writing.\n";
open( DISKPATH, "> $STATSDIR/diskpath.txt" );              print "    Opened $STATSDIR/diskpath.txt for writing.\n";
open( NOTDISKPATH, "> $STATSDIR/not_diskpath.txt" );       print "    Opened $STATSDIR/not_diskpath.txt for writing.\n";
open( EVENTS, "> $STATSDIR/events.txt" );                  print "    Opened $STATSDIR/events.txt for writing.\n";
open( LFOURRATIO, "> $STATSDIR/l4ratio.txt" );             print "    Opened $STATSDIR/l4ratio.txt for writing.\n";
open( LFOURREJ, "> $STATSDIR/l4rej.txt" );                 print "    Opened $STATSDIR/l4rej.txt for writing.\n";
open( LUMI, "> $STATSDIR/lumi.txt" );                      print "    Opened $STATSDIR/lumi.txt for writing.\n";
open( MACHINE, "> $STATSDIR/machine.txt" );                print "    Opened $STATSDIR/machine.txt for writing.\n";
open( NOTMACHINE, "> $STATSDIR/not_machine.txt" );         print "    Opened $STATSDIR/not_machine.txt for writing.\n";
open( SPEEDS, "> $STATSDIR/speeds.txt" );                  print "    Opened $STATSDIR/speeds.txt for writing.\n";
open( STARTTIME, "> $STATSDIR/starttime.txt" );            print "    Opened $STATSDIR/starttime.txt for writing.\n";
open( NOTSTARTTIME, "> $STATSDIR/not_starttime.txt" );     print "    Opened $STATSDIR/not_starttime.txt for writing.\n";
open( STARTUP, "> $STATSDIR/startup.txt" );                print "    Opened $STATSDIR/startup.txt for writing.\n";
open( NOTSTARTUP, "> $STATSDIR/not_startup.txt" );         print "    Opened $STATSDIR/not_startup.txt for writing.\n";
open( STOPTIME, "> $STATSDIR/stoptime.txt" );              print "    Opened $STATSDIR/stoptime.txt for writing.\n";
open( NOTLASTACTIVE, "> $STATSDIR/not_lastactive.txt" );   print "    Opened $STATSDIR/not_lastactive.txt for writing.\n";
open( NOTLASTEVENT, "> $STATSDIR/not_lastevent.txt" );     print "    Opened $STATSDIR/not_lastevent.txt for writing.\n";
open( RELEASE, "> $STATSDIR/release.txt" );                print "    Opened $STATSDIR/release.txt for writing.\n";
open( NOTRELEASE, "> $STATSDIR/not_release.txt" );         print "    Opened $STATSDIR/not_release.txt for writing.\n";
open( MIXEDRELEASES, "> $STATSDIR/mixed_releases.txt" );   print "    Opened $STATSDIR/mixedreleases.txt for writing.\n";
open( NOTMIXEDRELEASES, "> $STATSDIR/not_mixed_releases.txt" );
                                                           print "    Opened $STATSDIR/not_mixedreleases.txt for writing.\n";
open( DAEXCEPTIONS, "> $STATSDIR/daexceptions.txt" );      print "    Opened $STATSDIR/daexceptions.txt for writing.\n";
open( NOTDAEXCEPTIONS, "> $STATSDIR/not_daexceptions.txt" );
                                                           print "    Opened $STATSDIR/not_daexceptions.txt for writing.\n";
open( DISKSPACEERRORS, "> $STATSDIR/diskspaceerrors.txt" );
                                                           print "    Opened $STATSDIR/diskspaceerrors.txt for writing.\n";
open( NOTDISKSPACEERRORS, "> $STATSDIR/not_diskspaceerrors.txt" );
                                                           print "    Opened $STATSDIR/not_diskspaceerrors.txt for writing.\n";
open( NOTFATALERRORS, "> $STATSDIR/not_fatalerrors.txt" );
                                                           print "    Opened $STATSDIR/not_fatalerrors.txt for writing.\n";
open( NOTRUNSTATUS, "> $STATSDIR/not_runstatus.txt" );     print "    Opened $STATSDIR/not_runstatus.txt for writing.\n";

sub colontime {
    local( $totalsecs ) = @_[0];
    return "n/a" if ( $totalsecs eq "n/a" );

    local( $h, $m, $s );

    $h = int( $totalsecs / 3600 );
    $totalsecs = $totalsecs % 3600;

    $m = int( $totalsecs / 60 );
    $totalsecs = $totalsecs % 60;

    $s = $totalsecs;

    return "$h:$m:$s";
}

sub datestamptime {
    local( $totalsecs ) = @_[0];
    return "n/a" if ( $totalsecs eq "n/a" );

    local( $sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst )
	= localtime( $totalsecs );
    
    $mon = $shtnom{$mon};
    $year += 1900;
    $mday = sprintf( "%0d", $mday );
    $hour = sprintf( "%0d", $hour );
    $min = sprintf( "%0d", $min );
    $sec = sprintf( "%0d", $sec );

    return "$mon\_$mday\_$hour:$min:$sec\_$year";
}

$eventsum = 0;
$lumisum = 0;
foreach $run ( @runs )
{
    if ( $info{$run}{"badlogformat"} eq "no" )
    {
	if ( $info{$run}{"runstatus"} eq "ok" )
	{
	    print "    Run $run has finished processing successfully.\n";

	    print "        \" Runnumber : $run\"\n";
	    print RUNNUMBER " Runnumber : $run\n";
	    
	    print "        \" Logfile : $logrecent{$run}\"\n";
	    print LOGFILE " Logfile : $logrecent{$run}\n";

	    print "        \" Machine : $info{$run}{machine}\"\n";
	    print MACHINE " Machine : $info{$run}{machine}\n";

	    print "        \"  Events : $info{$run}{numevents}\"\n";
	    print EVENTS " Events : $info{$run}{numevents}\n";
	    $eventsum += $info{$run}{"numevents"} if ( $info{$run}{"numevents"} =~ /^[0-9]+$/o );
	    print "        (Events seen so far in finished runs = $eventsum.)\n";

	    if ( $info{$run}{"secperevent"} ne "n/a" )
	    {
		printf( "        \"   Speed : %4.2f\"\n", $info{$run}{"secperevent"} );
		printf( SPEEDS "   Speed : %4.2f\n", $info{$run}{"secperevent"} );
	    }
	    else
	    {
		print "        \"   Speed : n/a\"\n";
		print SPEEDS "   Speed : n/a\n";
	    }

	    $loadtime2 = &colontime( $info{$run}{"loadtime"} );
	    $startuptime2 = &colontime( $info{$run}{"startuptime"} );
	    print "        \"   Start : $loadtime2/$startuptime2\"\n";
	    print STARTUP "   Start : $loadtime2/$startuptime2\n";

	    print "        \"  BhaRej : $info{$run}{bharej}\"\n";
	    print BHAREJ "  BhaRej : $info{$run}{bharej}\n";

	    if ( $info{$run}{"secperevent"} ne "n/a" )
	    {
		printf( "        \" BhaRatio : %6.4f\"\n", $info{$run}{"bharatio"} );
		printf( BHARATIO " BhaRatio : %6.4f\n", $info{$run}{"bharatio"} );
	    }
	    else
	    {
		print "        \" BhaRatio : n/a\"\n";
		print BHARATIO " BhaRatio : n/a\n";
	    }

	    print "        \"   L4Rej : $info{$run}{l4rej}\"\n";
	    print LFOURREJ "   L4Rej : $info{$run}{l4rej}\n";

	    if ( $info{$run}{"l4ratio"} ne "n/a" )
	    {
		printf( "        \" L4Ratio : %6.4f\"\n", $info{$run}{"l4ratio"} );
		printf( LFOURRATIO " L4Ratio : %6.4f\n", $info{$run}{"l4ratio"} );
	    }
	    else
	    {
		print "        \" L4Ratio : n/a\"\n";
		print LFOURRATIO " L4Ratio : n/a\n";
	    }
	    
	    print "        \"    Disk : $info{$run}{diskpath}\"\n";
	    print DISKPATH "    Disk : $info{$run}{diskpath}\n";

	    if ( $info{$run}{"bbblumi"} eq "n/a"  ||  $info{$run}{"bbblumierr"} eq "n/a" )
	    {
		print "        \"    Lumi : n/a\"\n";
		print LUMI "    Lumi : n/a\n";
	    }
	    else
	    {
		print "        \"    Lumi : $info{$run}{bbblumi}+-$info{$run}{bbblumierr}\"\n";
		print LUMI "    Lumi : $info{$run}{bbblumi}+-$info{$run}{bbblumierr}\n";
		$lumisum += $info{$run}{"bbblumi"} if ( $info{$run}{"bbblumi"} =~ /^[0-9\.\-\+eE]+$/o );
	    }
	    
	    $starttime = &datestamptime( $info{$run}{"firsttimertime"} );
	    print "        \"  StartT : $starttime\"\n";
	    print STARTTIME "  StartT : $starttime\n";

	    $stoptime = &datestamptime( $info{$run}{"lasttimertime"} );
	    print "        \"   StopT : $stoptime\"\n";
	    print STOPTIME "   StopT : $stoptime\n";

	    print "        \" Release : $info{$run}{release}\"\n";
	    print RELEASE " Release : $info{$run}{release}\n";

	    print "        \" MixedReleases : $info{$run}{mixedreleases}\"\n";
	    print MIXEDRELEASES " MixedReleases : $info{$run}{mixedreleases}\n";

	    print "        \" DAExceptions : $info{$run}{daexceptions}\"\n";
	    print DAEXCEPTIONS " DAExceptions : $info{$run}{daexceptions}\n";

	    print "        \" DiskSpaceErrors : $info{$run}{diskspaceerrors}\"\n";
	    print DISKSPACEERRORS " DiskSpaceErrors : $info{$run}{diskspaceerrors}\n";

	}
	else  # runstatus ne "ok"
	{
	    print "    Run $run is either still on the queue or has crashed.\n";
#  	    if ( $info{$run}{"stillrunning"} eq "done" )
#  	    {
#  		print NOTSTILLRUNNING " StillRunning : 0\n";
#  	    }
#  	    elsif ( $info{$run}{"stillrunning"} eq "still running" )
#  	    {
#  		print NOTSTILLRUNNING " StillRunning : 1\n";
#  	    }
#  	    elsif ( $info{$run}{"stillrunning"} eq "unresponsive" )
#  	    {
#  		print NOTSTILLRUNNING " StillRunning : -1\n";
#  	    }
#  	    elsif ( $info{$run}{"stillrunning"} eq "starting up" )
#  	    {
#  		print NOTSTILLRUNNING " StillRunning : 1.5\n";
#  	    }
#  	    elsif ( $info{$run}{"stillrunning"} eq "pending" )
#  	    {
#  		print NOTSTILLRUNNING " StillRunning : 2\n";
#  	    }
#  	    else { die $info{$run}{"stillrunning"}; }

	    print "        \" StillRunning : $info{$run}{stillrunning}\"\n";
	    print NOTSTILLRUNNING " StillRunning : $info{$run}{stillrunning}\n";
	    
	    print "        \" Runnumber : $run\"\n";
	    print NOTRUNNUMBER " Runnumber : $run\n";

	    print "        \" Logfile : $logrecent{$run}\"\n";
	    print NOTLOGFILE " Logfile : $logrecent{$run}\n";

	    print "        \" Machine : $info{$run}{machine}\"\n";
	    print NOTMACHINE " Machine : $info{$run}{machine}\n";

	    $loadtime2 = &colontime( $info{$run}{"loadtime"} );
	    $startuptime2 = &colontime( $info{$run}{"startuptime"} );
	    print "        \"   Start : $loadtime2/$startuptime2\"\n";
	    print NOTSTARTUP "   Start : $loadtime2/$startuptime2\n";

	    print "        \"    Disk : $info{$run}{diskpath}\"\n";
	    print NOTDISKPATH "    Disk : $info{$run}{diskpath}\n";

	    $starttime = &datestamptime( $info{$run}{"firsttimertime"} );
	    print "        \"  StartT : $starttime\"\n";
	    print NOTSTARTTIME "  StartT : $starttime\n";

	    $lastactive = &datestamptime( $info{$run}{"laststoptime"} );
	    print "        \"  StartT : $lastactive\"\n";
	    print NOTLASTACTIVE "  StartT : $lastactive\n";

	    print "        \"  StartT : $info{$run}{lastevent}\"\n";
	    print NOTLASTEVENT "  StartT : $info{$run}{lastevent}\n";

	    print "        \" Release : $info{$run}{release}\"\n";
	    print NOTRELEASE " Release : $info{$run}{release}\n";

	    print "        \" MixedReleases : $info{$run}{mixedreleases}\"\n";
	    print NOTMIXEDRELEASES " MixedReleases : $info{$run}{mixedreleases}\n";

	    print "        \" DAExceptions : $info{$run}{daexceptions}\"\n";
	    print NOTDAEXCEPTIONS " DAExceptions : $info{$run}{daexceptions}\n";

	    print "        \" DiskSpaceErrors : $info{$run}{diskspaceerrors}\"\n";
	    print NOTDISKSPACEERRORS " DiskSpaceErrors : $info{$run}{diskspaceerrors}\n";

	    print "        \" FatalErrors : $info{$run}{fatalerrors}\"\n";
	    print NOTFATALERRORS " FatalErrors : $info{$run}{fatalerrors}\n";

	    print "        \" NotRunStatus : $info{$run}{runstatus}\"\n";
	    print NOTRUNSTATUS " NotRunStatus : $info{$run}{runstatus}\n";
	}
	print "\n";
    }
}

print EVENTS " - - -------\nSum :  $eventsum\n";
print LUMI " - - -------\nSum :  $lumisum\n";

close( RUNNUMBER );      close( NOTRUNNUMBER );   close( LOGFILE );
close( NOTLOGFILE );     close( BHARATIO );       close( BHAREJ );
close( DISKPATH );       close( NOTDISKPATH );    close( EVENTS );
close( LFOURRATIO );     close( LFOURREJ );       close( LUMI );
close( MACHINE );        close( NOTMACHINE );     close( SPEEDS );
close( STARTTIME );      close( NOTSTARTTIME );   close( STARTUP );
close( NOTSTARTUP );     close( STOPTIME );       close( NOTLASTACTIVE );
close( NOTLASTEVENT );   close( RELEASE );        close( NOTRELEASE );
close( MIXEDRELEASES );  close( NOTMIXEDRELEASES );

print "Closed all files.\n";
print "Done writing out files for the web script!\n\n";
###########################################################################
