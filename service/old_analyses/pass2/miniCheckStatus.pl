#!/usr/bin/perl

use DBI;

# For reverse-engineering the timestamp
use Time::Local;
%months = ( "Jan" => 0, "Feb" => 1, "Mar" => 2, "Apr" => 3, "May" => 4, "Jun" => 5,
	    "Jul" => 6, "Aug" => 7, "Sep" => 8, "Oct" => 9, "Nov" => 10, "Dec" => 11 );
%shtnom = ( 0 => "Jan", 1 => "Feb", 2 => "Mar", 3 => "Apr", 4 => "May", 5 => "Jun",
	    6 => "Jul", 7 => "Aug", 8 => "Sep", 9 => "Oct", 10 => "Nov", 11 => "Dec" );

$thisyear = ( (localtime(time))[5] + 1900 );

###########################################################################
# Important global variables
###########################################################################
$STATSDIR = "/home/minip2/MiniStats";
$PASS2DIR = "/home/pass2/MiniPass2/CalibrationFarm/miniPass2Scripts";
@KINDS = ( "bhabha", "radbhabha", "mupair" );

$n = 0;
open( LS, "ls $STATSDIR |" );
while( <LS> )
{
    if ( $_ =~ /status\.([0-9]+)\.pl/ )
    {
	$n = $1 if ( $n < $1 );
    }
}
$n++;
close( LS );

$initoutputfile = "$STATSDIR/initstatus.pl";
$outputfile = "$STATSDIR/status.$n.pl";
$prevoutputfile = "$STATSDIR/status.".( $n - 1 ).".pl";

$stillprocessing = "$STATSDIR/stillProcessing.$n.pl";
$prevstillprocessing = "$STATSDIR/stillProcessing.".( $n - 1 ).".pl";

$completelydone = "$STATSDIR/completelydone.$n.pl";
$prevcompletelydone = "$STATSDIR/completelydone.".( $n - 1 ).".pl";

$majorerrors = "$STATSDIR/majorErrors.$n.txt";
$majorerrorspl = "$STATSDIR/majorErrors.$n.pl";

$loglog = "$STATSDIR/log.$n.txt";
open( LOGLOG, "> $loglog" );

sub lprint
{
    print @_;
    print LOGLOG @_;
}

sub lprintf
{
    printf( @_ );
    printf( LOGLOG @_ );
}

# Simple constants
$earliestlog = 0;
$readoldunfinished = 1;
$readoldfinished = 0;

# Arguments
$nomail = 0;
$nomail = 1 if ( $ARGV[0] eq "nomail" );

$today = `date`;
chop( $today );

###########################################################################
# Get a list of old unfinished (and finished) runs from previous output
# Syntax:
#   $oldunfinished{$run_kind} == 1   (just check to see if it is defined)
###########################################################################
%oldunfinished = ();
%olddone = ();

if ( $readoldunfinished > 0  &&  -e $prevstillprocessing )
{
    lprint " * * * * * * * * * * * * * * * * Reading list of runs that didn't finish last time...\n";
    require( $prevstillprocessing );
}

lprint "The runs which didn't finish last time are:\n";
$row = 0;
foreach $run_kind ( keys %oldunfinished )
{
    lprint "    " if ( $row == 0 );
    lprint "$run_kind  ";
    $row++;
    if ( $row == 8 )
    {
	lprint "\n";
	$row = 0;
    }
}
lprint "\n\n";

if ( $readoldfinidhed == 0  &&  -e $prevcompletelydone )
{
    lprint " * * * * * * * * * * * * * * * * Reading list of runs that need not be processed again...\n";
    require( $prevcompletelydone );
}

lprint "The runs which won't be processed again are:\n";
$row = 0;
foreach $run_kind ( keys %olddone )
{
    lprint "    " if ( $row == 0 );
    lprint "$run_kind<$olddone{$run_kind}>  ";
    $row++;
    if ( $row == 8 )
    {
	lprint "\n";
	$row = 0;
    }
}
lprint "\n\n";
###########################################################################

###########################################################################
# Get a list of the log files and put it in the double-hash "log"
# Syntax:
#   $log{$run_kind}{$logversion} == [ $datetime ]
#   $logrecent{$run_kind} == largest $logversion
#   @runs == sorted list of run_kinds
###########################################################################
lprint " * * * * * * * * * * * * * * * * Getting a list of log files...\n";
lprint "    Format is: \"##RUN# < Log, Date >\"\n\n";
$row = 0;

%log = ();
%logrecent = ();
opendir( TOPDIR, $PASS2DIR );
@logdirs = readdir( TOPDIR );
closedir( TOPDIR );

foreach $logdir ( @logdirs )
{
    next if ( $logdir !~ /^Log([0-9]+)/o  ||  !( -d "$PASS2DIR/$logdir" ) );
    $logversion = $1;

    opendir( DIR, "$PASS2DIR/$logdir" );
    @files = readdir( DIR );
    closedir( DIR );

    foreach $kind ( @KINDS )
    {
	foreach $file ( @files )
	{
	    $fullpath = "$PASS2DIR/$logdir/$file";
	    if ( ( ! -d $fullpath )  &&  $file =~ /^([0-9]{6})_$kind\.log$/ )
	    {
		$run_kind = "$1_$kind";

		# log is earlier than commandline argument and doesn't need to be finished
		next if ( $logversion < $earliestlog  &&  ! defined( $oldunfinished{$run_kind} ) );

		# log has already been processed
		next if ( defined( $olddone{$run_kind} )  &&  $olddone{$run_kind} >= $logversion );

		( $dev, $ino, $mode, $nlink, $uid, $gid, $rdev, $size, $atime,
		  $mtime, $ctime, $blksize, $blocks ) = stat( $fullpath );
		$datetime = $mtime;

		# Put this run/version into the hash
		$log{$run_kind}{$logversion} = [ $datetime ];
		if ( ! defined( $logrecent{$run_kind} )   ||
		     $logrecent{$run_kind} < $logversion     )
		{
		    $logrecent{$run_kind} = $logversion;
		}

		# Be nice to your screen width
		if ( $row == 3 )
		{
		    lprint "\n";
		    $row = 0;
		}

		# Print out the information gathered for monitoring
		# (backconvert for safety)
		( $s, $m, $h, $d, $mon, $y, $wday, $yday, $isdst )
		    = localtime( $datetime );
		                $d = sprintf( "%02d", $d );
		$mon++;         $mon = sprintf( "%02d", $mon );
		$y = $y % 100;  $y = sprintf( "%02d", $y );
		lprint "    " if ( $row == 0 );
		lprintf( "%20s <%3s, %8s>    ", $run_kind, $logversion, "$mon/$d/$y" );
		$row++;

	    }  # end if file is not a directory and has a good name
	}  # end foreach file in this directory
    }  # end foreach "bhabha", "mupair", etc.
}  # end foreach logdirectory
lprint "\nDone getting run/log list!\n\n";

sub numly { $a <=> $b; }
sub numkindly
{
    my $tmp = ( substr( $a, 0, 6 ) <=> substr( $b, 0, 6  ) );
    return $tmp if ( $tmp != 0 );
    return ( substr( $a, 7 ) cmp substr( $b, 7 ) );
}
@runs = sort numkindly keys %log;

###########################################################################

###########################################################################
# Read the log files of only the most recent and fill the double-hash %info
# Syntax:
#   $info{$run_kind}{$varname} == value of variable
#   @varnames == ( list of available variable names )
#   $numericalvar{$varname} == 1  if $varname is numerical
#   $timevar{$varname} == 1  if $varname is a time
#   $datevar{$varname} == 1  if $varname is a date
#   @majorerrors == [ $run_kind, $log, $text, [ $line, $line, ... ] ]
###########################################################################
lprint " * * * * * * * * * * * * * * * * Reading most recent log files...\n\n";

%info = ();

sub writevar {
    local( $varname ) = @_[0];
    lprintf( "    %6s : %s", $line, $text );
    lprintf( "             ==> $varname = $var{$varname}\n" );
}

sub writeerror {
    local( $errstring ) = $var{"badlogformat"};
    lprintf( "    %6s : %s", $line, $text );
    lprintf( "     FORMAT ERROR: $errstring\n" );
}

sub putinerror { 
    local( $line, $text ) = @_;

    $var{"fatalerrors"}++;
    if ( ! defined( $errorseen{$text} ) )
    {
	$errorseen{$text} = [ $line ];
	&writevar("fatalerrors");
    }
    else
    {
	push( @{ $errorseen{$text} }, $line );
    }
}

# Be sure to put any new variable names into this list!

@varnames = (
	     "numevents",
	     "firstevent",
	     "eventThousand",
	     "lastevent",
	     "secpereventsofar",
	     "secpereventThousand",

	     "firststoptime",
	     "stoptimeThousand",
	     "laststoptime",

	     "badlogformat",
	     "fatalerrors",
	     "daexceptions",
	     "diskspaceerrors",
	     );

%numericalvar = ( "numevents" => 1, "lastevent" => 1,
		  "fatalerrors" => 1, "daexceptions" => 1,
		  "diskspaceerrors" => 1, "secpereventsofar" => 1,
		  "eventThousand" => 1, "secpereventThousand" => 1 );

%timevar = ( );

%datevar = ( "firststoptime" => 1, "laststoptime" => 1,
	     "stoptimeThousand" => 1 );

@logfileprocessstart = times;
foreach $run_kind ( @runs )
{
    $logfile = "Log$logrecent{$run_kind}/$run_kind.log";
    lprint "Using file \"$logfile\" for run_kind $run_kind...\n";
    open( LOG, "$PASS2DIR/$logfile" );

    # This is the ONE loop through the file all data should be taken
    # from it here (quadruple-space user output)
    $calledsummary = 0;

    $firststoptime = "";
    $laststoptime = "";
    $seenthousandth = 0;
    $dateThousand = "";

    %var = ();
    $var{"badlogformat"} = "no";
    $var{"fatalerrors"} = 0;
    $var{"daexceptions"} = 0;
    $var{"diskspaceerrors"} = 0;
    %errorseen = ();

    for ( $line = 1;  $text = <LOG>;  $line++ )
    {
	# Save tons of time
	next if ( $var{"badlogformat"} ne "no" );

	if ( $text eq " %% INFO-JobControl.SummaryModule: \n" )
	{
	    # Be on the lookout for "Stream event"
	    $calledsummary = 1;
	}

	elsif ( $calledsummary == 1  &&  $text =~ /^ Stream event : ([0-9]+)$/o )
	{
	    $var{"numevents"} = $1;
	    &writevar( "numevents" );
	    # Don't be so watchful anymore
	    $calledsummary = 0;
	}

	elsif ( substr( $text, 17, 1 ) eq ':'  &&  substr( $text, 20, 1 ) eq ':' )
	{
	    if ( $text =~ /Event:\s+([0-9]+)/o )
	    {
		if ( $firststoptime eq "" )
		{
		    $text =~ /Run:\s+([0-9]+)/o;
		    $var{"badlogformat"} = "Logfile \\\"$logfile\\\" processes run $1!"
			if ( $1 != substr( $run_kind, 0, 6 ) );
		}
		$var{"firstevent"} = $1 if ( $firststoptime eq "" );
		$var{"lastevent"} = $1;
		$firststoptime = substr( $text, 8, 15 ) if ( $firststoptime eq "" );
		$laststoptime = substr( $text, 8, 15 );
		if ( $seenthousandth == 0  &&  $var{"lastevent"} >= 1000 )
		{
		    $var{"eventThousand"} = $var{"lastevent"};
		    $dateThousand = substr( $text, 8, 15 );

		    $seenthousandth = 1;
		}
	    }
	}

	############## The following are for errors that people need to be notified about
	elsif ( substr( $text, 0, 10 ) eq " %% ERROR-" )
	{
	    # Ignore some non-fatal common cases
	    if ( substr( $text, 10, 8 ) ne "CDOffCal"                     &&
		 substr( $text, 10, 7 ) ne "RawData"                      &&
		 substr( $text, 10, 9 ) ne "P1Crystal"                    &&
		 substr( $text, 10, 14 ) ne "RichClustering"              &&
		 substr( $text, 10, 15 ) ne "RICO_PAD_CLUSTM"             &&
		 substr( $text, 10, 20 ) ne "CcP2ReconProd.CcBlob"        &&
		 substr( $text, 10, 26 ) ne "Processor.P1CrystalMonProc"  &&
		 substr( $text, 10, 6 ) ne "MUACCP"                       &&
		 substr( $text, 10, 62 ) ne "JobControl.JobControl: EventShape object initialized with beam"  &&
		 substr( $text, 10, 49 ) ne "TrackShowerMatching.TrackShowerLink: Bad distance"               &&
		 substr( $text, 10, 61 ) ne "DBStorage.ProxyStorer: no event classification info available"   &&
		 substr( $text, 10, 48 ) ne "Processor.WatchDiskSpaceProc: stat call for path" )
	    {
		if ( substr( $text, 10 ) eq
		     "JobControl.JobControl: -eventLoop: caught a DAException:\n" )
		{
		    $var{"daexceptions"}++;
#		    &writevar( "daexceptions" );
		}
		else  # DAExceptions are not fatal errors
		{
		    if ( $text !~ /rico/i )
		    {
			&putinerror( $line, $text );
		    }
		}
	    }
	}

	elsif ( substr( $text, 0, 14 ) eq " %% EMERGENCY-"            &&
		substr( $text, 14, 23 ) ne "BunchFinder.BunchFinder"  &&
 	        substr( $text, 14, 61 ) ne "TriggerL1DataProd.TriggerL1DataProxy:   Invalid board type : "                 &&
	        substr( $text, 14, 76 ) ne "TriggerL1DataProd.TriggerL1DataProxy: Internal trigger data integrity error!"  &&
	        substr( $text, 14, 58 ) ne "TriggerL1Data.TriggerL1Data: Invalid Cesr Phase.  Value = "                    &&
		substr( $text, 14, 50 ) ne "MUOffCal.muoffcal_are_you_mupair_: Found more than"                            &&
		substr( $text, 14, 60 ) ne "MUOffCal.muoffcal_are_you_mupair_: Cannot retrieve CesrPhase"                  &&
		substr( $text, 14, 43 ) ne "Processor.WatchDiskSpaceProc: disk space on"                                       )
	{
	    if ( substr( $text, 14, 28 ) eq "Processor.WatchDiskSpaceProc"               &&
		 ( ($tmp=substr( $text, 44 )) =~ /^not enough/o                     ||
		   ( $tmp =~ /^disk space on/o  &&  $tmp =~ /is below 1000000$/o )
		                                                                        )     )
	    {
		$var{"diskspaceerrors"}++;
	    }
	    else
	    {
		&putinerror( $line, $text );
	    }
	}

	elsif ( substr( $text, 0, 18 ) eq "Assertion failed: " )
	{
	    &putinerror( $line, $text );
	}

	elsif ( ( ($tmp=substr( $text, 0, 5 )) eq "EDT: "  ||
		  $tmp eq "EST: " )                            &&
		( $text =~ /Caught signal/o   ||
		  $text =~ /Exception/o          )                )
	{
	    &putinerror( $line, $text );
	}

	elsif ( substr( $text, 0, 15 ) eq "** System Error" )
	{
	    &putinerror( $line, $text );
	}  

	elsif ( substr( $text, 0, 11 ) eq "/nfs/cleo3/"  &&  $text =~ /Killed/o )
	{
	    &putinerror( $line, $text );
	}

	elsif ( substr( $text, 0, 4 ) eq "/var" &&
		$text =~ /not found/o  &&  $text =~ /sparc\-sun\-solaris2\.8/o )
	{
	    &putinerror( $line, $text );
	}

	##################################################################################
    }
    lprint "    Done with file \"$logfile\"! ($line lines)\n";
    close LOG;

    # Parse the $laststoptime and @lasttimertime
    if ( $firststoptime ne "" )
    {
	( $month, $day, $hour, $minute, $second ) = split( / +|:/, $firststoptime );
	$month = $months{$month};
	$firststoptime = timelocal( $second, $minute, $hour, $day, $month, $thisyear );
	lprint "    First reported event stop happened at ";
	lprint scalar localtime( $firststoptime );
	lprint "\n";
    }
    else
    {
	$firststoptime = "n/a";
	lprint "    No event stops appear in the log file yet (to record firststoptime).\n";
    }
    $var{"firststoptime"} = $firststoptime;

    if ( $laststoptime ne "" )
    {
	( $month, $day, $hour, $minute, $second ) = split( / +|:/, $laststoptime );
	$month = $months{$month};
	$laststoptime = timelocal( $second, $minute, $hour, $day, $month, $thisyear );
	lprint "    Last reported event stop happened at ";
	lprint scalar localtime( $laststoptime );
	lprint "\n";
    }
    else
    {
	$laststoptime = "n/a";
	lprint "    No event stops appear in the log file yet (to record laststoptime).\n";
    }
    $var{"laststoptime"} = $laststoptime;

    if ( $seenthousandth == 1  &&  $var{"eventThousand"} != $var{"firstevent"} )
    {
	( $month, $day, $hour, $minute, $second ) = split( / +|:/, $dateThousand );
	$month = $months{$month};
	$dateThousand = timelocal( $second, $minute, $hour, $day, $month, $thisyear );
	lprint "    Thousandth reported event stop (eventThousand = $var{eventThousand}) happened at ";
	lprint scalar localtime( $dateThousand );
	lprint "\n";
	$var{"secpereventThousand"} = ( ( $dateThousand - $firststoptime ) /
					( $var{"eventThousand"} - $var{"firstevent"} ) );
	lprintf( "    secpereventThousand = %10g.\n", $var{"secpereventThousand"} );
    }
    else
    {
	$dateThousand = "n/a";
	$var{"secpereventThousand"} = "n/a";
	lprint "    The thousandth event stop has not appeared in the log file yet.\n";
    }
    $var{"stoptimeThousand"} = $dateThousand;

    if ( defined( $var{"lastevent"} ) )
    {
	lprint "    Last event stop is event number $var{lastevent}.\n"
    }
    else
    {
	lprint "    No event stops appear in the log file yet (to record lastevent).\n";
    }

    # Computed quantities
    if ( defined( $var{"firststoptime"} )  &&  defined( $var{"laststoptime"} )  &&
	 ( $var{"lastevent"} - $var{"firstevent"} ) != 0 )
    {
	$var{"secpereventsofar"} = ( ( $var{"laststoptime"} - $var{"firststoptime"} )
				     / ( $var{"lastevent"} - $var{"firstevent"} ) );
	lprint "    secpereventsofar = $var{secpereventsofar}\n";
    }

    # Fill the @majorerrors list
    lprint "    There were $var{fatalerrors} fatal errors to report in this file.\n";
    lprint "    There were $var{daexceptions} daexceptions in this file.\n";
    lprint "    There were $var{diskspaceerrors} disk space errors in this file.\n";
    foreach $text ( keys %errorseen )
    {
	$lineslist = $errorseen{$text};
	push( @majorerrors, [ $run_kind, $logrecent{$run_kind}, $text, $lineslist ] );
    }

    # Check to be sure that all variables have been defined (if not, n/a)
    $seenone = 0;
    foreach $varname ( @varnames )
    {
	if ( ! defined( $var{$varname} )  ||  $var{$varname} eq "n/a" )
	{
	    if ( $seenone == 0 )
	    {
		lprint "    Variables not found in the file:\n";
		$seenone++;
	    }

	    $var{$varname} = "n/a";
	    lprint "        * $varname\n";
	}
    }

    foreach $varname ( @varnames )
    {
	$info{$run_kind}{$varname} = $var{$varname};
    }
    lprint "\n";
}

@logfileprocessend = times;
if ( $#runs != 0 )
{
    $logfileprocesstime = ( $logfileprocessend[0] - $logfileprocessstart[0] );
    $logfileaveragetime = ( $logfileprocesstime / $#runs );
    lprintf( "%3d logfiles were processed in %5g seconds, or %5g seconds per logfile.\n",
	    ( $#runs + 1 ), $logfileprocesstime, $logfileaveragetime );
}

lprint "Done reading logfiles!\n\n";
###########################################################################

###########################################################################
# Make a file of the major errors and e-mail Curtis, Dave and Jim
###########################################################################

if ( $#majorerrors >= 0 )
{
    %uniqueerrors = ();
    %uniqueerrorsbyfile = ();

    open( MAJORERRORS, "> $majorerrors" );
    open( MAJORERRORSPL, "> $majorerrorspl" );
    open( CURTIS, "| mail csj6\@mail.lns.cornell.edu" ) if ( $nomail == 0 );
    open( DAVE, "| mail dlk\@mail.lns.cornell.edu" ) if ( $nomail == 0 );
    open( JIM, "| mail mccann+cleo.pass2auto\@cyrus.watson.org" ) if ( $nomail == 0 );

    $tmp = "Subject: Major errors found by miniCheckStatus.pl
Major errors found by miniCheckStatus.pl (execution number $n):\n\n";
    print CURTIS $tmp;  print DAVE $tmp;  print JIM $tmp;

    $tmp = sprintf( "%-20s | %3s | %8s | Line from Logfile\n", "Run_Kind", "Log", "Line" );
    print MAJORERRORS $tmp;  print CURTIS $tmp;  print DAVE $tmp;  print JIM $tmp;
    $tmp = "---------------------+-----+----------+-------------------------------------------------------------------------------------------\n";
    print MAJORERRORS $tmp;  print CURTIS $tmp;  print DAVE $tmp;  print JIM $tmp;

    print MAJORERRORSPL "#!/usr/bin/perl

warn \"File \\\"$majorerrorspl\\\" is being executed.\\n\";

\@majorerrors = ();

###########################################################################
# Data gathered $today
###########################################################################

# START EVERYTHING
# START MAJORERRORS
";

    $prevrun = 0;
    foreach $error ( @majorerrors )
    {
	( $run_kind, $log, $text, $linesref ) = @{ $error };
	@lines = sort numly @{ $linesref };
	$tmp = "";
	$tmp .= "                     |     |          |\n" if ( $run_kind != $prevrun );
	$tmp .= sprintf( "%-20s | %3d | %8d | %s", $run_kind, $log, $lines[0], $text );
	chop( $text );

	if ( defined( $uniqueerrors{$text} ) )
	{
	    $uniqueerrors{$text} = ( $#lines + 1 );
	    $uniqueerrorsbyfile{$text} = 1;
	}
	else
	{
	    $uniqueerrors{$text} += ( $#lines + 1 );
	    $uniqueerrorsbyfile{$text}++;
	}

	$text =~ s/\"/\\\"/g;
	$text =~ s/\$/\\\$/g;
	$text =~ s/\@/\\\$/g;
	print MAJORERRORSPL "push( \@majorerrors, [ $run_kind, $log, \"$text\\n\", [ $lines[0]";
	$row = 0;
	if ( $#lines > 0 )
	{
	    foreach $line ( @lines[ 1..$#lines ] )
	    {
		$tmp .= sprintf( "%20s | %3s | ", "", "" ) if ( $row == 0 );
		$tmp .= sprintf( "%8d ", $line );
		print MAJORERRORSPL ", $line";
		$row++;
		if ( $row == 14 )
		{
		    $row = 0;
		    $tmp .= "\n";
		}
	    }
	    $tmp .= "\n";
#	    $tmp .= "-------+-----+--------+-------------------------------------------------------------------------------------------\n";
#	    $tmp .= sprintf( "%6s | %3s | %6s | Line from Logfile\n", "RunNum", "Log", "Line" );
#	    $tmp .= "-------+-----+--------+-------------------------------------------------------------------------------------------\n";
	}
	print MAJORERRORSPL " ] ] );\n";
	print MAJORERRORS $tmp;  print CURTIS $tmp;  print DAVE $tmp;  print JIM $tmp;
	$prevrun = $run_kind;
    }
    
    print MAJORERRORSPL "# STOP MAJORERRORS
# STOP EVERYTHING

warn \"File \\\"$majorerrorspl\\\" has finished.\\n\";
";

    print MAJORERRORS "---------------------+-----+----------+-------------------------------------------------------------------------------------------

Unique error strings:
  times     files     message
---------+---------+--------------------------------------------------------------------------------------------------------------
";
    foreach $text ( sort bytimes keys %uniqueerrors )
    {
	printf( MAJORERRORS "%8d |%8d | %s\n", $uniqueerrors{$text}, $uniqueerrorsbyfile{$text}, $text );
    }

    close( MAJORERRORSPL );
    close( MAJORERRORS );
    close( CURTIS ) if ( $nomail == 0 );
    close( DAVE ) if ( $nomail == 0 );
    close( JIM ) if ( $nomail == 0 );
}

sub bytimes
{
    my $tmp = ( $uniqueerrors{$b} <=> $uniqueerrors{$a} );
    return $tmp if ( $tmp != 0 );
    return ( $a cmp $b );
}

###########################################################################

###########################################################################
# Look at qstat to see which ones are still running
# Syntax:
#   $info{$run_kind}{"stillrunning"} eq "none", "still running", "unresponsive" or "pending"
#   @varnames == ( list of available variable names )
#   $newunfinished{$run_kind} == $logversion
#   $newdone{$run_kind} == $logversion
#
# It is in the queue if I see: "pass2  [space]  ######_pa  [anything else]"
###########################################################################
lprint " * * * * * * * * * * * * * * * * Calling qstat to find which are still on the queue\n";

push( @varnames, "stillrunning" );

%newunfinished = ();
%newdone = ();

sub showtime {
    local( $totalsecs ) = @_[0];
    local( $sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst )
	= localtime( $totalsecs );
    
    $mon = $shtnom{$mon};
    $year += 1900;
    $mday = sprintf( "%0d", $mday );
    $hour = sprintf( "%0d", $hour );
    $min = sprintf( "%0d", $min );
    $sec = sprintf( "%0d", $sec );

    return "$hour:$min:$sec $mon $mday";
}

sub showdelta {
    local( $totalsecs ) = @_[0];
    local( $d, $h, $m, $s, $plural );

    $d = int( $totalsecs / 86400 );
    $totalsecs = $totalsecs % 86400;
    $plural = "s";
    $plural = "" if ( $d == 1 );

    $h = int( $totalsecs / 3600 );
    $totalsecs = $totalsecs % 3600;

    $m = int( $totalsecs / 60 );
    $totalsecs = $totalsecs % 60;

    $s = $totalsecs;

    return "$d day$plural $h:$m:$s";
}

open( QSTAT, "/usr/local/bin/qstat -f -cell rcf |" );
$qstatbroken = 0;

$qstatbroken = 1 if ( ! ( $text = <QSTAT> ) );
$qstatbroken = 1 if ( $text =~ /^Queue Name                       Queue Type    Quan  Load          State$/ );

if ( $qstatbroken == 1 )
{
    close( QSTAT );  # don't want a broken pipe
    lprint "Couldn't start qstat process!\n";
    lprint "stillrunning will be set to \"n/a\" for all runs:\n";
    $row = 0;
    foreach $run_kind ( @runs )
    {
	$info{$run_kind}{"stillrunning"} = "n/a";
	lprint "    " if ( $row == 0 );
	lprint "$run_kind  ";
	$row++;
	if ( $row == 8 )
	{
	    lprint "\n";
	    $row = 0;
	}
    }
}
else
{
    $pendyet = 0;
    $now = time;
    $line = 0;
    while( $text = <QSTAT> )
    {
	if ( $text =~ /pending jobs/i )
	{
	    $pendyet = 1;
	    lprintf( "    %6s : %s", $line, $text );
	    lprintf( "             ==> Everything after this is pending.\n" );
	}
	
	# For each list item, store the run number in hash tables
	elsif ( $text =~ /pass2\s+([0-9]{6})_([a-zA-Z0-9]{2})/o )
	{
	    $thisrun = $1;
	    $thiskind = $2;

	    $run_kind = "";
	    foreach $kind ( @KINDS )
	    {
		$run_kind = "$thisrun\_$thiskind" if ( substr( $kind, 0, 2 ) eq $thiskind );

		last if ( $run_kind ne "" );
	    }

	    if ( defined( $logrecent{$run_kind} ) )
	    {
		$newunfinished{$run_kind} = $logrecent{$run_kind};
		
		if ( $pendyet == 0 )
		{
		    if ( ( $now - $info{$run_kind}{"laststoptime"} ) < ( 12 * 60 * 60 ) )
		    {
			$laststoptime = &showtime( $info{$run_kind}{"laststoptime"} );
			$delta = &showdelta( $now - $info{$run_kind}{"laststoptime"} );
			$info{$run_kind}{"stillrunning"} = "still running";
			lprintf( "    %6s : %s", $line, $text );
			lprintf( "             laststoptime = $laststoptime, which is $delta ago.\n" );
			lprintf( "             ==> Run_Kind $run_kind is still running ok (\"still running\").\n" );
		    }
		    else
		    {
			$laststoptime = &showtime( $info{$run_kind}{"laststoptime"} );
			$delta = &showdelta( $now - $info{$run_kind}{"laststoptime"} );
			$info{$run_kind}{"stillrunning"} = "unresponsive";
			lprintf( "    %6s : %s", $line, $text );
			lprintf( "             laststoptime = $laststoptime, which is $delta ago.\n" );
			lprintf( "             ==> Run_Kind $run_kind might have hit an infinite loop (\"unresponsive\").\n" );
		    }
		}
		else
		{
		    $info{$run}{"stillrunning"} = "pending";
		    lprintf( "    %6s : %s", $line, $text );
		    lprintf( "             ==> Run_Kind $run_kind is waiting to be processed (\"pending\").\n" );
		}
	    }
	}
	$line++;
    }
    close QSTAT;
    lprint "Done with the qstat process!\n";
    
    lprint "The rest of the runs have finished processing (\"done\"):\n";
    $row = 0;
    foreach $run_kind ( @runs )
    {
	if ( ! defined( $info{$run_kind}{"stillrunning"} ) )
	{
	    $newdone{$run_kind} = $logrecent{$run_kind};
	    $info{$run_kind}{"stillrunning"} = "done";
	    lprint "    " if ( $row == 0 );
	    lprint "$run_kind  ";
	    $row++;
	    if ( $row == 8 )
	    {
		lprint "\n";
		$row = 0;
	    }
	}
    }
}
lprint "\n\n";
###########################################################################

###########################################################################
# Write out to my perl data files
###########################################################################
lprint " * * * * * * * * * * * * * * * * Writing out to $outputfile\n";

sub quoteall
{
    local( @in ) = @_;
    local( @out ) = ();
    local( $thing );
    
    foreach $thing ( @in )
    { push( @out, "\"$thing\"" ); }

    return @out;
}

open( INIT, "> $initoutputfile" );
print INIT "#!/usr/bin/perl

warn \"File \\\"$initoutputfile\\\" is being executed.\\n\";

\%log = ();
\%logrecent = ();
\@runs = ();
\@varnames = ( " . join( ", ", &quoteall( @varnames ) ) . " );
";

foreach $varname ( @varnames )
{
    print INIT "\$numericalvar{\"$varname\"} = 1;\n" if ( defined( $numericalvar{$varname} ) );
    print INIT "\$timevar{\"$varname\"} = 1;\n" if ( defined( $timevar{$varname} ) );
    print INIT "\$datevar{\"$varname\"} = 1;\n" if ( defined( $datevar{$varname} ) );
}

print INIT "\%info = ();

warn \"File \\\"$initoutputfile\\\" has finished.\\n\";
";
close( INIT );

open( OUT, "> $outputfile" );
print OUT "#!/usr/bin/perl

warn \"File \\\"$outputfile\\\" is being executed.\\n\";
";

if ( -e $prevoutputfile )
{
    print OUT "\nif ( \$norecurse == 1 )
{ require( \"$initoutputfile\" ); }
else
{ require( \"$prevoutputfile\" ); }\n";
}
else
{
    print OUT "\nrequire( \"$initoutputfile\" );\n";
}

print OUT "
###########################################################################
# Data gathered $today
###########################################################################

# START EVERYTHING
# START LOG
";

lprint "    Writing out \%log double-hash...\n";
foreach $run_kind ( @runs )
{
    foreach $logversion ( sort numly keys %{ $log{$run_kind} } )
    {
	print OUT "\$log{\"$run_kind\"}{$logversion} = [ ";
	print OUT join( ", ", @{ $log{$run_kind}{$logversion} } );
	print OUT " ];\n";
    }
}

print OUT "# STOP LOG\n\n# START LOGRECENT
";

lprint "    Writing out \%logrecent hash...\n";
foreach $run_kind ( @runs )
{
    print OUT "\$logrecent{\"$run_kind\"} = $logrecent{$run_kind};\n";
}

print OUT "# STOP LOGRECENT\n\n# START RUNS
";
lprint "    Writing out sorted \@runs list...\n";
foreach $run_kind ( @runs )
{
    print OUT "push( \@runs, \"$run_kind\" );\n";
}

print OUT "# STOP RUNS\n\n# START INFO
";
lprint "    Writing out \%info double-hash...\n";
foreach $run_kind ( @runs )
{
    foreach $varname ( @varnames )
    {
	if ( $info{$run_kind}{$varname} !~ /^[0-9\.\-\+eE]+$/o )
	{ $quote = "\""; }
 	else
 	{ $quote = ""; }

	print OUT "\$info{\"$run_kind\"}{\"$varname\"} = $quote$info{$run_kind}{$varname}$quote;\n";
    }
    print OUT "\n";
}
print OUT "# STOP INFO
# STOP EVERYTHING

warn \"File \\\"$outputfile\\\" has finished.\\n\";
";
close( OUT );

lprint "Done writing out to $outputfile!\n\n";

lprint " * * * * * * * * * * * * * * * * Writing out to $stillprocessing\n";

open( STILLPROCESSING, "> $stillprocessing" );

print STILLPROCESSING "#!/usr/bin/perl

warn \"File \\\"$stillprocessing\\\" is being executed.\\n\";

\%oldunfinished = ();

###########################################################################
# Data gathered $today
###########################################################################

# START EVERYTHING
# START OLDUNFINISHED
";

lprint "    Writing out \%oldunfinished hash...\n";
foreach $run_kind ( keys %newunfinished )
{
    print STILLPROCESSING "\$oldunfinished{\"$run_kind\"} = $newunfinished{$run_kind};\n";
}

print STILLPROCESSING "# STOP OLDUNFINISHED
# STOP EVERYTHING

warn \"File \\\"$stillprocessing\\\" has finished.\\n\";
";
close( STILLPROCESSING );

lprint "Done writing out to $stillprocessing!\n\n";

############

lprint " * * * * * * * * * * * * * * * * Writing out to $completelydone\n";

open( COMPLETELYDONE, "> $completelydone" );

print COMPLETELYDONE "#!/usr/bin/perl

warn \"File \\\"$completelydone\\\" is being executed.\\n\";
";

if ( -e $prevcompletelydone )
{
    print COMPLETELYDONE "\nrequire( \"$prevcompletelydone\" );\n";
}

print COMPLETELYDONE "
###########################################################################
# Data gathered $today
###########################################################################

# START EVERYTHING
# START OLDUNFINISHED
";

lprint "    Writing out \%olddone hash...\n";
foreach $run_kind ( keys %newdone )
{
    print COMPLETELYDONE "\$olddone{\"$run_kind\"} = $newdone{$run_kind};\n";
}

print COMPLETELYDONE "# STOP OLDDONE
# STOP EVERYTHING

warn \"File \\\"$completelydone\\\" has finished.\\n\";
";
close( COMPLETELYDONE );

lprint "Done writing out to $completelydone!\n\n";

###########################################################################

lprint "All done!\n\n";
