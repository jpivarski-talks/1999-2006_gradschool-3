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
$STATSDIR = "/home/pass2/Stats";
$PASS2DIR = "/home/pass2/Pass2Production/pass2/Pass2Scripts";

# Simple constants
$earliestlog = 0;
$readoldunfinished = 1;
$readoldfinished = 0;

# Arguments
$nomail = 0;
$nolog = 0;
foreach my $argv ( @ARGV )
{
    $nomail = 1 if ( $argv =~ /nomail/i );
    $nolog = 1 if ( $argv =~ /nolog/i );
}

$today = `date`;
chop( $today );

# files
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
close LS;

$initoutputfile = "$STATSDIR/initstatus.pl";
$outputfile = "$STATSDIR/status.$n.pl";
$prevoutputfile = "$STATSDIR/status.".( $n - 1 ).".pl";

$stillprocessing = "$STATSDIR/stillProcessing.$n.pl";
$prevstillprocessing = "$STATSDIR/stillProcessing.".( $n - 1 ).".pl";

$completelydone = "$STATSDIR/completelydone.$n.pl";
$prevcompletelydone = "$STATSDIR/completelydone.".( $n - 1 ).".pl";

$majorerrors = "$STATSDIR/majorErrors.$n.txt";
$majorerrorspl = "$STATSDIR/majorErrors.$n.pl";

$mailingList = "$STATSDIR/errorMailingList.txt";

$loglog = "$STATSDIR/log.$n.txt";
open( LOGLOG, "> $loglog" ) if ( $nolog == 0 );

sub lprint
{
    print @_;
    print LOGLOG @_ if ( $nolog == 0 );
}

sub lprintf
{
    printf( @_ );
    printf( LOGLOG @_ ) if ( $nolog == 0 );
}

###########################################################################
# Get a list of old unfinished (and finished) runs from previous output
# Syntax:
#   $oldunfinished{$run} == 1   (just check to see if it is defined)
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
foreach $run ( keys %oldunfinished )
{
    lprint "    " if ( $row == 0 );
    lprint "$run  ";
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
foreach $run ( keys %olddone )
{
    lprint "    " if ( $row == 0 );
    lprint "$run<$olddone{$run}>  ";
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
#   $log{$run}{$logversion} == [ $datetime ]
#   $logrecent{$run} == largest $logversion
#   @runs == sorted list of runs
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

    foreach $file ( @files )
    {
	$fullpath = "$PASS2DIR/$logdir/$file";
	next if ( -d $fullpath   ||   $file !~ /^([0-9]{6})_pass2\.log$/o );
	$run = $1;

	# log is earlier than commandline argument and doesn't need to be finished
	next if ( $logversion < $earliestlog  &&  ! defined( $oldunfinished{$run} ) );

	# log has already been processed
	next if ( defined( $olddone{$run} )  &&  $olddone{$run} >= $logversion );

	( $dev, $ino, $mode, $nlink, $uid, $gid, $rdev, $size, $atime, $mtime, $ctime, $blksize, $blocks )
	    = stat( $fullpath );
	$datetime = $mtime;

	# Put this run/version into the hash
	$log{$run}{$logversion} = [ $datetime ];
	if ( ! defined( $logrecent{$run} )   ||
	     $logrecent{$run} < $logversion     )
	{
	    $logrecent{$run} = $logversion;
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
	lprintf( "%6s <%3s, %8s>    ", $run, $logversion, "$mon/$d/$y" );
	$row++;
    }
}
lprint "\nDone getting run/log list!\n\n";

# Take a closer look at just a few
# %log = ();
# %logrecent = ();
# $log{112078}{81} = [ 1 ];
# $logrecent{112078} = 81;
# $log{112127}{109} = [ 1 ];
# $logrecent{112127} = 109;

sub numly { $a <=> $b; }
@runs = sort numly keys %log;

###########################################################################

###########################################################################
# Get a list of expected parameters from Dave's textfiles
# Syntax:
#   $p1info{$run} = [ $federation, $events, $lumi, $use ]
###########################################################################
lprint " * * * * * * * * * * * * * * * * Getting information about the runs from Dave's textfiles...\n";
%p1info = ();

opendir( DLK, "/home/dlk/Luminosities/" );
@dlkfiles = readdir( DLK );
closedir( DLK );

foreach my $dlkfile ( @dlkfiles )
{
    my $dlkfed;
    if ( $dlkfile =~ /^data([0-9]+)\.runlist$/o )
    {
	open( DLKFILE, "/home/dlk/Luminosities/$dlkfile" );
	$dlkfed = $1;
    }
    elsif ( $dlkfile =~ /^data([0-9]+)\.runlist\.gz$/o )
    {
	open( DLKFILE, "/usr/local/bin/gunzip -c /home/dlk/Luminosities/$dlkfile |" );
	$dlkfed = $1;
    }
    else
    {
	next;
    }

    lprint "Reading from file $dlkfile:";
    my $row = 0;
    
    while( my $dline = <DLKFILE> )
    {
	my ( $drun, $devents, $dlumi, $duse ) = split( /\s+/, $dline );
	if ( defined( $logrecent{$drun} ) )
	{
	    $p1info{$drun} = [ $dlkfed, $devents, $dlumi, $duse ];
	    
	    lprint "\n    " if ( $row == 0 );
	    lprint " $drun  ";
	} # endif this is a run we're interested in
	else
	{
	    lprint "\n    " if ( $row == 0 );
	    lprint "($drun) ";
	}
	$row++;
	$row = 0 if ( $row == 10 );
    }
    lprint "\n";

    close( DLKFILE );
}

###########################################################################

# ###########################################################################
# # Open the SQL database and store information for those runs which will be processed
# # Syntax:
# #   $dbinfo{$run} = [ { "dbvar" => value, ... }, ... ]
# #   @dbvarnames = ( list of available variable names )
# #   $dbnumericalvar{$dbvarname} == 1  if $dbvarname is numerical
# #   $dbtimevar{$dbvarname} == 1  if $dbvarname is a time
# #   $dbdatevar{$dbvarname} == 1  if $dbvarname is a date
# ###########################################################################
# lprint " * * * * * * * * * * * * * * * * Getting information about all runs from the database...\n";
%dbinfo = ();
@dbvarnames = ( "path", "serverID", "databaseID", "federationID" );
%dbnumericalvar = ( "serverID" => 1, "databaseID" => 1, "federationID" => 1 );
%dbtimevar = ();
%dbdatevar = ();

# $dbhost = `hostname`;
# if ( $dbhost eq "sol301.lns.cornell.edu\n" )
# { $dbhost = "localhost"; }
# else
# { $dbhost = "sol301"; }

# $db = "DBI:mysql:MetaData:$dbhost";
# lprint "Connecting to \"$db\" (very fast)...\n";
# if ( $dbh = DBI->connect( "DBI:mysql:MetaData:$dbhost", "nobody", "" ) )
# {
#     $dbcommand = "select * from FileLocation;";
#     lprint "Calling \"$dbcommand\" (about 10 seconds)...\n";
#     $sth = $dbh->prepare( $dbcommand );
#     if ( $sth->execute() )
#     {
# 	lprint "Filling \%dbinfo hash (about 10 seconds)...\n";
# 	$countruns = 0;
# 	$countdatabases = 0;
# 	while( ( $recordID, $path, $serverID, $HSMdiskID, $timestamp, $databaseID, $federationID )
# 	       = $sth->fetchrow_array() )
# 	{
# 	    if ( $path =~ /Run([0-9]{6})/ )
# 	    {
# 		$run = $1;
# 		if ( defined( $log{$run} ) )
# 		{

# 		    if ( ! defined( $dbinfo{$run} ) )
# 		    {
# 			$dbinfo{$run} = [ { "path"         => $path,
# 					    "serverID"     => $serverID,
# 					    "databaseID"   =>$databaseID,
# 					    "federationID" => $federationID } ];
# 			$countruns++;
# 		    }
# 		    else
# 		    {
# 			push( @{ $dbinfo{$run} }, { "path"         => $path,
# 						    "serverID"     => $serverID,
# 						    "databaseID"   =>$databaseID,
# 						    "federationID" => $federationID } );
# 		    }
# 		    $countdatabases++;
# 		} # endif this is a run we're interested in
# 	    } # endif path has a run number in its name
# 	    else
# 	    {
# 		lprint "Ignoring \"$path\".\n";
# 	    }
# 	} # endwhile there's still data to get from the database
# 	lprint "\n    Saw $countdatabases databases for $countruns / ". ( $#runs + 1 ) ." runs!\n\n";
#     } # endif the command execution worked
#     else
#     {
# 	lprint "Failed to execute command because \"$dbh::errstr\".\n";
#     }
#     lprint "Closing database...\n";
# }
# else
# {
#     lprint "Failed to open database because \"$DBI::errstr\"\n.";
# }
# lprint "Done with the database!\n\n";
# ###########################################################################

###########################################################################
# Get diskspace percentages
# Syntax:
#   $disk{$run}{percentHighWaterMark} == ...
#   $disk{$run}{percentUsed} == ...
#   $lognum{$logversion} == [ $run1, $run2, $run3, ... ]
###########################################################################
lprint " * * * * * * * * * * * * * * * * Getting diskspace percentages...\n";

%lognum = ();
foreach $run ( @runs )
{
    if ( ! defined( $lognum{$logrecent{$run}} ) )
    {
	$lognum{$logrecent{$run}} = [ $run ];
    }
    else
    {
	push( @{ $lognum{$logrecent{$run}} }, $run );
    }
}

foreach $logversion ( sort numly keys %lognum )
{
    $transcript = "Log$logversion/logfile";
    lprint "Using file \"$transcript\" for run(s) ";
    lprint join( " ", sort numly @{ $lognum{$logversion} } );
    lprint "...\n";
    open( LOG, "$PASS2DIR/$transcript" );
    $currun = "";
    while( $text = <LOG> )
    {
	if ( $currun ne ""  &&  $text =~
	     /^percentHighWaterMark, percentUsed=\s*([0-9\.\-\+eE]+)\s+([0-9\.\-\+eE]+)\s*$/o )
	{
	    $disk{$currun}{"percentHighWaterMark"} = $1;
	    $disk{$currun}{"percentUsed"} = $2;
	    lprint "    run $currun has percentHighWaterMark = $disk{$currun}{percentHighWaterMark}";
	    lprint " and percentUsed = $disk{$currun}{percentUsed}\n";
	}

	$currun = $1
	    if ( $text =~ /^Processing runNumber= ([0-9]{6})$/o );
    }
    close( LOG );
}

lprint "Done with diskspace percentages!\n\n";
###########################################################################

###########################################################################
# Read the log files of only the most recent and fill the double-hash %info
# Syntax:
#   $info{$run}{$varname} == value of variable
#   @varnames == ( list of available variable names )
#   $numericalvar{$varname} == 1  if $varname is numerical
#   $timevar{$varname} == 1  if $varname is a time
#   $datevar{$varname} == 1  if $varname is a date
#   @majorerrors == [ $run, $log, $text, [ $line, $line, ... ] ]
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
#	&writevar("fatalerrors");   # I write them at the end now
    }
    else
    {
	push( @{ $errorseen{$text} }, $line );
    }
}

# Be sure to put any new variable names into this list!

@varnames = (
	     "p1fed",
	     "p1events",
	     "p1lumi",

	     "logfile",
	     "machine",
	     "release",
	     "mixedreleases",
	     "diskpath",
	     "percentHighWaterMark",
	     "percentUsed",
	     "controlWriteoutVersion",
	     "RichPexact",
	     "RichExactSawtoothOnly",

	     "numevents",
	     "firstevent",
	     "eventThousand",
	     "lastevent",
	     "secperevent",
	     "secpereventsofar",
	     "secpereventThousand",

	     "l4rej",
	     "l4ratio",
	     "bharej",
	     "bharatio",
	     "bbblumi",
	     "bbblumierr",

	     "loadtime",
	     "startuptime",
	     "processtime",

	     "firststoptime",
	     "stoptimeThousand",
	     "laststoptime",
	     "firsttimertime",
	     "lasttimertime",

	     "badlogformat",
	     "fatalerrors",
	     "daexceptions",
	     "diskspaceerrors",
	     "noisycrystals",
	     "runstatus",
	     );

%numericalvar = ( "p1fed" => 1, "p1events" => 1, "p1lumi" => 1, "logfile" => 1,
		  "numevents" => 1, "l4rej" => 1, "bharej" => 1,
		  "bbblumi" => 1, "bbblumierr" => 1, "secperevent" => 1,
		  "bharatio" => 1, "l4ratio" => 1, "lastevent" => 1,
		  "fatalerrors" => 1, "daexceptions" => 1,
		  "diskspaceerrors" => 1, "noisycrystals" => 1, "secpereventsofar" => 1,
		  "controlWriteoutVersion" => 1, "eventThousand" => 1,
		  "secpereventThousand" => 1 );

%timevar = ( "loadtime" => 1, "startuptime" => 1, "processtime" => 1 );

%datevar = ( "firststoptime" => 1, "laststoptime" => 1,
	     "firsttimertime" => 1, "lasttimertime" => 1, "stoptimeThousand" => 1 );

# these five functions are meant to be used in flexvars
sub addvar
{
    my ( $name, $type ) = @_;

    push( @varnames, $name );

    if ( $type eq "s" )
    { ; }
    elsif ( $type eq "n" )
    { $numericalvar{$name} = 1; }
    elsif ( $type eq "t" )
    { $timevar{$name} = 1; }
    elsif ( $type eq "d" )
    { $datevar{$time} = 1; }
}

sub initvar
{
    my ( $name, $initvalue ) = @_;
    $var{$name} = $initvalue;
}

sub varcontent
{
    my ( $name, $content ) = @_;

    $var{$name} = $content;
    &writevar( $name );
}

sub varincrement
{
    my ( $name ) = @_;
    if ( defined( $var{$name} ) )
    {
	$var{$name}++;
    }
    else
    {
	$var{$name} = 1;
    }
    &writevar( $name );
}

sub getvar
{
    my ( $name ) = @_;
    return $var{$name};
}
# end of five functions

sub bynumerrors
{
    my $tmp = ( $errorseen{$b} <=> $errorseen{$a} );
    $tmp = ( $a cmp $b ) if ( $tmp == 0 );
    $tmp;
}

require( "$STATSDIR/flexvars.pl" );

@logfileprocessstart = times;
foreach $run ( @runs )
{
    $logfile = "Log$logrecent{$run}/$run\_pass2.log";
    lprint "Using file \"$logfile\" for run $run...\n";

    open( LOG, "$PASS2DIR/$logfile" );

    # This is the ONE loop through the file all data should be taken
    # from it here (quadruple-space user output)
    $whichdelta = 0;

    $calledtimer = 0;
    $calledsummary = 0;          $permcalledsummary = 0;
    $calledlumimonproc = 0;      $permcalledlumimonproc = 0;
    $calledexit = 0;             $permcalledexit = 0;

    $founddiskpath = 0;

    $firststoptime = "";
    $laststoptime = "";
    $seentimer = 0;
    $firsttimertime = ();
    $lasttimertime = ();
    $seenthousandth = 0;
    $dateThousand = "";

    %var = ();
    $var{"badlogformat"} = "no";
    $var{"fatalerrors"} = 0;
    $var{"daexceptions"} = 0;
    $var{"diskspaceerrors"} = 0;
    $var{"noisycrystals"} = 0;
    %errorseen = ();

    if ( defined( $p1info{$run} ) )
    {
	( $var{"p1fed"}, $var{"p1events"}, $var{"p1lumi"}, $p1use ) = @{ $p1info{$run} };
	lprint "    p1fed = $var{p1fed}\n";
	lprint "    p1events = $var{p1events}\n";
	lprint "    p1lumi = $var{p1lumi}\n";
    }
    else
    {
	$var{"p1fed"} = $var{"p1events"} = $var{"p1lumi"} = "n/a";
	lprint "    p1fed = p1events = p1lumi = \"n/a\"\n";
    }

    if ( defined( $disk{$run} ) )
    {
	$var{"percentHighWaterMark"} = $disk{$run}{"percentHighWaterMark"};
	$var{"percentUsed"} = $disk{$run}{"percentUsed"};
	lprint "    percentHighWaterMark = $var{percentHighWaterMark}\n";
	lprint "    percentUsed = $var{percentUsed}\n";
    }
    else
    {
	$var{"percentHighWaterMark"} = "n/a";
	$var{"percentUsed"} = "n/a";
	lprint "    percentHighWaterMark = percentUsed = \"n/a\"\n";
    }

    $var{"logfile"} = $logrecent{$run};

    for ( $line = 1;  $text = <LOG>;  $line++ )
    {
	# Save tons of time
	next if ( $var{"badlogformat"} ne "no" );

	&flexvars( $text );

	# machine
	if ( $line == 1 )
	{
	    if ( $text !~ /^([a-zA-Z0-9]+)\.lns\.cornell\.edu$/o )
	    {
		$var{"badlogformat"} = "Machine name isn't the first line!";
		&writeerror();
	    }
	    else
	    {
		$var{"machine"} = $1;
		&writevar( "machine" );
	    }
	}

	elsif ( $text =~ /^Suez> \# This is version\s+([0-9\.]+)\s+of controlWriteout_pass2\.tcl/ )
	{
	    $var{"controlWriteoutVersion"} = $1;
	    &writevar( "controlWriteoutVersion" );
	}
	
	elsif ( $text =~ /^Suez>\s{1,4}(\#*)\s{0,4}param RichAnglesProd Pexact (.*)$/ )
	{
	    $commenting = $1;
	    $value = $2;
	    
	    if ( length( $commenting ) == 0 )
	    { $var{"RichPexact"} = $value; }
	    else
	    { $var{"RichPexact"} = "commented"; }
	    &writevar( "RichPexact" );
	}

	elsif ( $text =~ /^Suez>\s{1,4}(\#*)\s{0,4}param RichAnglesProd ExactForSawtoothOnly (.*)$/ )
	{
	    $commenting = $1;
	    $value = $2;
	    
	    if ( length( $commenting ) == 0 )
	    { $var{"RichExactSawtoothOnly"} = $value; }
	    else
	    { $var{"RichExactSawtoothOnly"} = "commented"; }
	    &writevar( "RichExactSawtoothOnly" );
	}

	elsif ( substr( $text, 0, 12 ) eq "C3LIB     = " )
	{
	    $var{"release"} = substr( $text, 12 );
	    chop( $var{"release"} );
	    &writevar( "release" );
	}

	elsif ( defined( $var{"release"} )    &&
		substr( $text, -3 ) eq ".so"  &&  $text !~ $var{"release"} )
	{
	    @libname = split( /\s+/, $text );
	    $var{"mixedreleases"} .= "$libname[ $#libname ] ";
	    &writevar( "mixedreleases" );
	}

	elsif ( $text eq "Suez> timer display\n" )
	{
	    # Be on the lookout for a timermodule line
	    $calledtimer = 1;
	}

	elsif ( $calledtimer == 1  &&  $text =~
  /^ %% INFO-TimerModule.TimerModule: Cur time   = [A-Z][a-z]{2} ([A-Z][a-z]{2}) ([0-9]{2}) ([0-9]{1,2}):([0-9]{2}):([0-9]{2})/o )
	{
	    if ( $seentimer == 0 )
	    {
		@firsttimertime = ( $1, $2, $3, $4, $5 );
		$seentimer = 1;
	    }
	    @lasttimertime = ( $1, $2, $3, $4, $5 );
	    # Don't close $calledtimer because "Delta t" still needs to be read off
	}
	
	elsif ( $calledtimer == 1  &&  $text =~
  /^ %% INFO\-TimerModule\.TimerModule: Delta t = ([0-9]{1,3}):([0-9]{2}):([0-9]{2})/o )
	{
	    $delta = ( 60 * 60 * $1  +  60 * $2  +  $3 );
	    if ( $whichdelta == 0 )
	    {
		# skip the first reset
		$whichdelta++;
	    }
	    elsif ( $whichdelta == 1 )
	    {
		$var{"loadtime"} = $delta;
		&writevar( "loadtime" );
		$whichdelta++;
	    }
	    elsif ( $whichdelta == 2 )
	    {
		$var{"startuptime"} = $delta;
		&writevar( "startuptime" );
		$whichdelta++;	
	    }
	    elsif ( $whichdelta == 3 )
	    {
		$var{"processtime"} = $delta;
		&writevar( "processtime" );
		$whichdelta++;	
	    }
	    else
	    {
		$var{"badlogformat"} = "Too many TimerModule calls!";
		&writeerror();
	    }
	    # Don't be so watchful anymore
	    $calledtimer = 0;
	}

	elsif ( $text eq " %% INFO-JobControl.SummaryModule: \n" )
	{
	    # Be on the lookout for "Stream event"
	    $permcalledsummary = $calledsummary = 1;
	}

	elsif ( $calledsummary == 1  &&  $text =~ /^ Stream event : ([0-9]+)$/o )
	{
	    $var{"numevents"} = $1;
	    &writevar( "numevents" );
	    # Don't be so watchful anymore
	    $calledsummary = 0;
	}
	elsif ( substr( $text, 0, 42 ) eq "   *            Luminosity Results for Run" )
	{
	    $permcalledlumimonproc = $calledlumimonproc = 1;
	}

	elsif ( $calledlumimonproc == 1  &&  $text =~
		/Barrel Bhabha Lum = BBLM =\s+([0-9\.]+) \+\-\s+([0-9\.]+)/o )
	{
	    $var{"bbblumi"} = $1;
	    $var{"bbblumierr"} = $2;
	    &writevar( "bbblumi" );
	    &writevar( "bbblumierr" );

	    # Bharej happens after "Barrel Bhabha Lum" in P1LumiMonProc
	}

	elsif ( $calledlumimonproc == 1  &&  $text =~
		/^Filtered out a total of ([0-9]+) Bhabha events/o )
	{
	    $var{"bharej"} = $1;
	    &writevar( "bharej" );

	    # Bharej happens after "Barrel Bhabha Lum" in P1LumiMonProc
	    $calledlumimonproc = 0;
	}

	elsif ( $text eq "Suez> exit\n" )
	{
	    $permcalledexit = $calledexit = 1;
	}

	elsif ( $calledexit == 1  &&  $text =~
		/^ %% NOTICE-Processor\.Level4Proc: L4Kill     : ([0-9]+)$/o )
	{
	    $var{"l4rej"} = $1;
	    &writevar( "l4rej" );

	    # Don't worry about this so near the end (besides, I might later want
	    # a variable that comes after calling exit)
	    # $calledexit = 0;
	}

	elsif ( $founddiskpath == 0  &&  $text =~ /^Suez> \# diskpath ([^\n]+)$/o )
	{
	    $full_diskpath = $1;
	    if ( $full_diskpath !~ /(sol[0-9]{3}\/disk[0-9])/o )
	    {
		$var{"badlogformat"} = "diskpath is $full_diskpath!";
		&writeerror();
	    }
	    else
	    {
		$var{"diskpath"} = $1;
		&writevar( "diskpath" );
	    
		# This is near the beginning of the file, so just turn off as soon
		# as you've found it.
		$founddiskpath = 1;
	    }
	}

	elsif ( substr( $text, 17, 1 ) eq ':'  &&  substr( $text, 20, 1 ) eq ':' )
	{
	    if ( $text =~ /Event:\s+([0-9]+)/o )
	    {
		if ( $firststoptime eq "" )
		{
		    $text =~ /Run:\s+([0-9]+)/o;
		    $var{"badlogformat"} = "Logfile \\\"$logfile\\\" processes run $1!" if ( $1 != $run );
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

	elsif ( substr( $text, 0, 25 ) eq "***** Noisy CC crystal = " )
	{
	    $var{"noisycrystals"}++;
	    &writevar( "noisycrystals" );
	}

	############## The following are for errors that people need to be notified about
	elsif ( substr( $text, 0, 10 ) eq " %% ERROR-" )
	{
	    # Ignore some non-fatal common cases
	    if (
		 substr( $text, 10, 52 ) ne "Processor.P1CrystalMonProc: Too many dead crystals ="                               &&
		 substr( $text, 10, 81 ) ne "RichClustering.RichClustering: Delete all clusters since array sizes are exceeded"  &&
		 substr( $text, 10, 66 ) ne "RichClustering.RichClustering: too many calibrated hits to cluster"                 &&
		 substr( $text, 10, 81 ) ne "RichClustering.RichClustering: EventShape object initialized with beam energy out"  &&
		 substr( $text, 10, 37 ) ne "RICO_PAD_CLUSTM:  Too many RICH bumps"                                              &&
		 substr( $text, 10, 40 ) ne "RICO_PAD_CLUSTM:  Too many RICH clusters"                                           &&
		 substr( $text, 10, 76 ) ne "RICO_PAD_CLUSTM: EventShape object initialized with beam energy out of range"       &&
		 substr( $text, 10, 45 ) ne "CcP2ReconProd.CcBlob: No peaks found in blob="                                      &&
		 substr( $text, 10, 37 ) ne "MUACCP:  Error in error ellipse DXCP "                                              &&
		 substr( $text, 10, 62 ) ne "JobControl.JobControl: EventShape object initialized with beam"                     &&
		 substr( $text, 10, 51 ) ne "AlarmInterface.AIAlarmManager:  Noisy crystal list:"                                &&
		 ! ( substr( $text, 10, 30 ) eq "AlarmInterface.AIAlarmManager:" && $text =~ /, Sending level 4 alarm\:/ )       &&
		 substr( $text, 10 ) !~ /^Processor\.P1CrystalMonProc\:\s*$/o                                                    &&
		 ! ( substr( $text, 10, 38 ) eq "RichClustering.RichClustering:  Sector" && $text =~ /Has too many hits/ )       &&
		 substr( $text, 10, 77 ) ne "RichClustering.RichClustering: EventShape object initialized with beam energy"      &&
		 ! ( substr( $text, 10, 25 ) eq "RawData.RawUnit: For unit" && $text =~ /are not equal \!/ )                     &&
		 substr( $text, 10, 56 ) ne "C3ccReconProd.CcEnergyHitProxy: Invalid RawCCHits object"                           &&
		 substr( $text, 10, 36 ) ne "OBBase.OBOSPath: stat failed on path"                                               &&
		 1  )
	    {
		if ( substr( $text, 10 ) eq
		     "JobControl.JobControl: -eventLoop: caught a DAException:\n" )
		{
		    $var{"daexceptions"}++;
#		    &writevar( "daexceptions" );

		    &putinerror( $line, "(This is DAException number 100.) ==> $text" )
			if ( $var{"daexceptions"} == 100 );
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
		substr( $text, 14, 62 ) ne "BunchFinder.BunchFinder: No good DR hits for bunch finding :-("  &&
 	        substr( $text, 14, 61 ) ne "TriggerL1DataProd.TriggerL1DataProxy:   Invalid board type : "                 &&
	        substr( $text, 14, 76 ) ne "TriggerL1DataProd.TriggerL1DataProxy: Internal trigger data integrity error!"  &&
	        substr( $text, 14, 58 ) ne "TriggerL1Data.TriggerL1Data: Invalid Cesr Phase.  Value = "   )
	{
	    if ( substr( $text, 14, 28 ) eq "Processor.WatchDiskSpaceProc"               &&
		 ( ($tmp=substr( $text, 44 )) =~ /^not enough/o                     ||
		   ( $tmp =~ /^disk space on/o  &&  $tmp =~ /is below 1000000$/o )  ||
		   $tmp =~ /stat call for path/o                                        )     )
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

	elsif ( substr( $text, 0, 29 ) eq "**** Dead CsI crystal count =" )
	{
	    &putinerror( $line, $text );
	}

	##################################################################################
    }
    lprint "    Done with file \"$logfile\"! ($line lines)\n";
    close LOG;

    if ( defined( $var{"daexceptions"} )  &&  defined( $var{"numevents"} )
	 &&  $var{"numevents"} > 0 )
    {
	my $daratio = ( $var{"daexceptions"} / $var{"numevents"} );
	if ( $daratio > 0.001 )
	{
	    $daratio *= 100;
	    &putinerror( 0, "$daratio% of the events have DAExceptions.\n" );
	}
    }
	
    if ( ! defined( $var{"controlWriteoutVersion"} ) )
    {
	$var{"controlWriteoutVersion"} = 1.1;
	lprint "    Couldn't find \"controlWriteoutVersion\", assuming 1.1.\n";
    }

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

    if ( @firsttimertime != () )
    {
	( $month, $day, $hour, $minute, $second ) = @firsttimertime;
	$month = $months{$month};
	$firsttimertime = timelocal( $second, $minute, $hour, $day, $month, $thisyear );
	lprint "    First timer call happened at ";
	lprint scalar localtime( $firsttimertime );
	lprint "\n";
    }
    else
    {
	$firsttimertime = "n/a";
	lprint "    No timer module calls appear in the log file yet.\n";
    }
    $var{"firsttimertime"} = $firsttimertime;

    if ( @lasttimertime != () )
    {
	( $month, $day, $hour, $minute, $second ) = @lasttimertime;
	$month = $months{$month};
	$lasttimertime = timelocal( $second, $minute, $hour, $day, $month, $thisyear );
	lprint "    Last timer call happened at ";
	lprint scalar localtime( $lasttimertime );
	lprint "\n";
    }
    else
    {
	$lasttimertime = "n/a";
	lprint "    No timer module calls appear in the log file yet.\n";
    }
    $var{"lasttimertime"} = $lasttimertime;

    # Computed quantities
    if ( defined( $var{"processtime"} )  &&  defined( $var{"numevents"} )  &&
	 $var{"numevents"} != 0 )
    {
	$var{"secperevent"} = ( $var{"processtime"} / $var{"numevents"} );
	lprint "    secperevent = $var{secperevent}\n";
    }
    if ( defined( $var{"bharej"} )  &&  defined( $var{"numevents"} )  &&
	 $var{"numevents"} != 0 )
    {
	$var{"bharatio"} = ( $var{"bharej"} / $var{"numevents"} );
	lprint "    bharatio = $var{bharatio}\n";
    }
    if ( defined( $var{"l4rej"} )  &&  defined( $var{"numevents"} )  &&
	 $var{"numevents"} != 0 )
    {
	$var{"l4ratio"} = ( $var{"l4rej"} / $var{"numevents"} );
	lprint "    l4ratio = $var{l4ratio}\n";
    }
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
    my @errtext = sort bynumerrors keys %errorseen;
    if ( $#errtext != -1 )
    {
	lprint "    ---------+-----------------------------------------------------------\n";
	foreach $text ( @errtext )
	{
	    $lineslist = $errorseen{$text};
	    lprintf( "    %8d | %s", $errorseen{$text}, $text );
	    push( @majorerrors, [ $run, $logrecent{$run}, $text, $lineslist ] );
	}
	lprint "    ---------+-----------------------------------------------------------\n";
    }

    if ( $var{"fatalerrors"} > 0 )
    {
	$var{"runstatus"} = "fatalerrors = $var{fatalerrors}";
    }
    elsif ( $permcalledsummary == 0  ||  $permcalledlumimonproc == 0  ||  $permcalledexit == 0 )
    {
	$var{"runstatus"} = "script unfinished";
    }
    else
    {
	$var{"runstatus"} = "ok";
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
	$info{$run}{$varname} = $var{$varname};
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
# Look at qstat to see which ones are still running
# Syntax:
#   $info{$run}{"stillrunning"} eq "none", "still running", "unresponsive" or "pending"
#   @varnames == ( list of available variable names )
#   $newunfinished{$run} == $logversion
#   $newdone{$run} == $logversion
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
    foreach $run ( @runs )
    {
	$info{$run}{"stillrunning"} = "n/a";
	lprint "    " if ( $row == 0 );
	lprint "$run  ";
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
	elsif ( $text =~ /pass2\s+([0-9]{6})_pa/o )
	{
	    $run = $1;
	    if ( defined( $logrecent{$run} ) )
	    {
		$newunfinished{$run} = $logrecent{$run};
		
		if ( $pendyet == 0 )
		{
		    if ( $info{$run}{"laststoptime"} eq "n/a" )
		    {
			$info{$run}{"stillrunning"} = "starting up";
			lprintf( "    %6s : %s", $line, $text );
			lprintf( "             laststoptime = \"n/a\".\n" );
			lprintf( "             ==> Run $run hasn't started yet (\"starting up\").\n" );
		    }
		    elsif ( ( $now - $info{$run}{"laststoptime"} ) < ( 12 * 60 * 60 ) )
		    {
			$laststoptime = &showtime( $info{$run}{"laststoptime"} );
			$delta = &showdelta( $now - $info{$run}{"laststoptime"} );
			$info{$run}{"stillrunning"} = "still running";
			lprintf( "    %6s : %s", $line, $text );
			lprintf( "             laststoptime = $laststoptime, which is $delta ago.\n" );
			lprintf( "             ==> Run $run is still running ok (\"still running\").\n" );
		    }
		    else
		    {
			$laststoptime = &showtime( $info{$run}{"laststoptime"} );
			$delta = &showdelta( $now - $info{$run}{"laststoptime"} );
			$info{$run}{"stillrunning"} = "unresponsive";
			lprintf( "    %6s : %s", $line, $text );
			lprintf( "             laststoptime = $laststoptime, which is $delta ago.\n" );
			lprintf( "             ==> Run $run might have hit an infinite loop (\"unresponsive\").\n" );
		    }
		}
		else
		{
		    $info{$run}{"stillrunning"} = "pending";
		    lprintf( "    %6s : %s", $line, $text );
		    lprintf( "             ==> Run $run is waiting to be processed (\"pending\").\n" );
		}
	    }
	}
	$line++;
    }
    close QSTAT;
    lprint "Done with the qstat process!\n";
    
    lprint "The rest of the runs have finished processing (\"done\"):\n";
    $row = 0;
    foreach $run ( @runs )
    {
	if ( ! defined( $info{$run}{"stillrunning"} ) )
	{
	    $newdone{$run} = $logrecent{$run};
	    $info{$run}{"stillrunning"} = "done";
	    lprint "    " if ( $row == 0 );
	    lprint "$run  ";
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
# Make a file of the major errors and e-mail Adi and Jim
###########################################################################

$doitanyway = 0;
foreach $run ( @runs )
{
    next if ( $info{$run}{"runstatus"} ne "script unfinished" );
    next if ( $info{$run}{"stillrunning"} ne "done"  &&  $info{$run}{"stillrunning"} ne "unresponsive" );
    $doitanyway = 1;
}

if ( $#majorerrors >= 0  ||  $doitanyway == 1 )
{
    open( MAJORERRORS, "> $majorerrors" );
    open( MAJORERRORSPL, "> $majorerrorspl" );
    if ( $nomail == 0 )
    {
	open( MAILLIST, $mailingList );
	@mailing_list = ();
	while( $person = <MAILLIST> )
	{
	    chop( $person );
	    if ( $person =~ /@/ )
	    { $person =~ s/@/\\@/; }
	    else
	    { $person .= "\@mail.lns.cornell.edu"; }
	    push( @mailing_list, $person );
	}
	close( MAILLIST );

	$mailline = ( "| mail -t " . join( " ", @mailing_list ) );

	open( MAIL, $mailline );
    }

    $tmp = "Subject: checkStatus.pl error report $today (execution $n)\n";
    print MAIL $tmp;

    $tmp = "Major errors found by checkStatus.pl (execution number $n):\n\n";
    print MAIL $tmp;

    $tmp = sprintf( "%6s | %3s | %8s | Line from Logfile\n", "RunNum", "Log", "Line" );
    print MAJORERRORS $tmp;  print ADI $tmp;  print JIM $tmp;
    $tmp = "-------+-----+----------+-------------------------------------------------------------------------------------------\n";
    print MAJORERRORS $tmp;  print MAIL $tmp;

    print MAJORERRORSPL "#!/usr/bin/perl

warn \"File \\\"$majorerrorspl\\\" is being executed.\\n\";

\@majorerrors = ();

###########################################################################
# Data gathered $today
###########################################################################

# START EVERYTHING
# START MAJORERRORS
";

    $tmp = "Runs with 'runstatus' == \"script unfinished\":\n";
    $row = 0;
    foreach $run ( @runs )
    {
	next if ( $info{$run}{"runstatus"} ne "script unfinished" );
	$tmp .= $run;
	$tmp .= "*" if ( $info{$run}{"stillrunning"} eq "finished" );
	$tmp .= "!" if ( $info{$run}{"stillrunning"} eq "unresponsive" );
	$tmp .= " ";
	$row++;
	if ( $row == 14 )
	{
	    $row = 0;
	    $tmp .= "\n";
	}
    }
    $tmp .= "\n(*) means that the job is not on the queue any more,
(!) means that it is unresponsive (stalled for 12 hours)\n\n";
    print MAJORERRORS $tmp;  print MAIL $tmp;

    $prevrun = 0;
    foreach $error ( @majorerrors )
    {
	( $run, $log, $text, $linesref ) = @{ $error };
	@lines = sort numly @{ $linesref };
	$tmp = "";
	$tmp .= "       |     |          |\n" if ( $run != $prevrun );
	$tmp .= sprintf( "%6d | %3d | %8d | %s", $run, $log, $lines[0], $text );
	chop( $text );
	$text =~ s/\"/\\\"/g;
	$text =~ s/\$/\\\$/g;
	$text =~ s/\@/\\\$/g;
	print MAJORERRORSPL "push( \@majorerrors, [ $run, $log, \"$text\\n\", [ $lines[0]";
	$row = 0;
	if ( $#lines > 0 )
	{
	    foreach $line ( @lines[ 1..$#lines ] )
	    {
		$tmp .= sprintf( "%6s | %3s | ", "", "" ) if ( $row == 0 );
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
	print MAJORERRORS $tmp;  print MAIL $tmp;
	$prevrun = $run;
    }
    
    print MAJORERRORSPL "# STOP MAJORERRORS
# STOP EVERYTHING

warn \"File \\\"$majorerrorspl\\\" has finished.\\n\";
";
    close( MAJORERRORSPL );
    close( MAJORERRORS );
    close( MAIL ) if ( $nomail == 0 );
}

###########################################################################

###########################################################################
# Make sure that the necessary comments directories exist
###########################################################################
lprint " * * * * * * * * * * * * * * * * Making comments directories\n";

foreach $run ( @runs )
{
    $parent = substr( $run, 0, 3 );
    $child = substr( $run, 3, 3 );

    if ( -e "$STATSDIR/comments/$parent"  &&  -d "$STATSDIR/comments/$parent" )
    {
	# lprint "    $STATSDIR/comments/$parent already exists.\n";
    }
    else
    {
	lprint "    Creating new directory $STATSDIR/comments/$parent.\n";
	mkdir( "$STATSDIR/comments/$parent" );
	system( "chmod 777 $STATSDIR/comments/$parent" );
    }

    if ( -e "$STATSDIR/comments/$parent/$child"  &&  -d "$STATSDIR/comments/$parent/$child" )
    {
	lprint "        $STATSDIR/comments/$parent/$child already exists.\n";
    }
    else
    {
	lprint "        Creating new directory $STATSDIR/comments/$parent/$child.\n";
	mkdir( "$STATSDIR/comments/$parent/$child" );
	system( "chmod 777 $STATSDIR/comments/$parent/$child" );
    }
}

lprint "Done making comments directories!\n\n";

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

\@dbvarnames = ( " . join( ", ", &quoteall( @dbvarnames ) ) . " );
\%dbnumericalvar = ();
\%dbtimevar = ();
\%dbdatevar = ();
";

foreach $dbvarname ( @dbvarnames )
{
    print INIT "\$dbnumericalvar{\"$dbvarname\"} = 1;\n" if ( defined( $dbnumericalvar{$dbvarname} ) );
    print INIT "\$dbtimevar{\"$dbvarname\"} = 1;\n" if ( defined( $dbtimevar{$dbvarname} ) );
    print INIT "\$dbdatevar{\"$dbvarname\"} = 1;\n" if ( defined( $dbdatevar{$dbvarname} ) );
}

print INIT "\%dbinfo = ();

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
foreach $run ( @runs )
{
    foreach $logversion ( sort numly keys %{ $log{$run} } )
    {
	print OUT "\$log{$run}{$logversion} = [ ";
	print OUT join( ", ", @{ $log{$run}{$logversion} } );
	print OUT " ];\n";
    }
}

print OUT "# STOP LOG\n\n# START LOGRECENT
";

lprint "    Writing out \%logrecent hash...\n";
foreach $run ( @runs )
{
    print OUT "\$logrecent{$run} = $logrecent{$run};\n";
}

print OUT "# STOP LOGRECENT\n\n# START RUNS
";
lprint "    Writing out sorted \@runs list...\n";
foreach $run ( @runs )
{
    print OUT "push( \@runs, $run );\n";
}

print OUT "# STOP RUNS\n\n# START DBINFO
";
lprint "    Writing out \%dbinfo hash of arrays of hashes...\n";
foreach $run ( @runs )
{
    print OUT "# Run $run has ". ( $#{ $dbinfo{$run} } + 1 ) ." databases associated with it.\n";
    print OUT "\$dbinfo{$run} = [ ";
    $first = 1;
    foreach $database ( @{ $dbinfo{$run} } )
    {
	%tmp = %{ $database };
	if ( $first == 1 )
	{ $first = 0; }
	else
	{ print OUT ",\n                    "; }
	
	print OUT "{ ";
	$firstfirst = 1;
	foreach $dbvarname ( @dbvarnames )
	{
	    if ( $firstfirst == 1 )
	    { $firstfirst = 0; }
	    else
	    { print OUT ", "; }

	    if ( $tmp{$dbvarname} !~ /^[0-9\.\-\+eE]+$/o )
	    { $quote = "\""; }
	    else
	    { $quote = ""; }

	    print OUT "\"$dbvarname\" => $quote$tmp{$dbvarname}$quote";
	}
	print OUT " }";
    }
    print OUT " ];\n\n";
}
print OUT "# STOP DBINFO\n\n# START INFO
";
lprint "    Writing out \%info double-hash...\n";
foreach $run ( @runs )
{
    foreach $varname ( @varnames )
    {
	if ( $info{$run}{$varname} !~ /^[0-9\.\-\+eE]+$/o )
	{ $quote = "\""; }
 	else
 	{ $quote = ""; }

	print OUT "\$info{$run}{\"$varname\"} = $quote$info{$run}{$varname}$quote;\n";
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
foreach $run ( keys %newunfinished )
{
    print STILLPROCESSING "\$oldunfinished{$run} = $newunfinished{$run};\n";
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
# START OLDDONE
";

lprint "    Writing out \%olddone hash...\n";
foreach $run ( keys %newdone )
{
    print COMPLETELYDONE "\$olddone{$run} = $newdone{$run};\n";
}

print COMPLETELYDONE "# STOP OLDDONE
# STOP EVERYTHING

warn \"File \\\"$completelydone\\\" has finished.\\n\";
";
close( COMPLETELYDONE );

lprint "Done writing out to $completelydone!\n\n";

###########################################################################

###########################################################################
# Mailing about which runs are done
###########################################################################

if ( $nomail == 0 )
{
    lprint " * * * * * * * * * * * * * * * * Mailing about which runs are done\n";
    
    open( MAILLIST, $mailingList );
    @mailing_list = ();
    while( $person = <MAILLIST> )
    {
	chop( $person );
	if ( $person =~ /@/ )
	{ $person =~ s/@/\\@/; }
	else
	{ $person .= "\@mail.lns.cornell.edu"; }
	push( @mailing_list, $person );
    }
    close( MAILLIST );
    
    $mailline = ( "| mail -t " . join( " ", @mailing_list ) );
    
    open( MAIL, $mailline );

    print MAIL "Subject: checkStatus.pl execution number $n general report $today\n";
    
    my $tmp = "Runs summarized in execution number $n:
(widen your window)

RunNum | Log | % done | fatals | DAexcepts | diskerrs |  queue status | do again? | runstatus
-------+-----+--------+--------+-----------+----------+---------------+-----------+------------------
";

    lprint $tmp;
    print MAIL $tmp;

    foreach $run ( @runs )
    {
	my $doagain = "yes";
	$doagain = " no" if ( defined( $newdone{$run} ) );

        my $p1ratio = "n/a";
        $p1ratio = ( 100 * $info{$run}{"lastevent"} / $info{$run}{"p1events"} )
            if ( $info{$run}{"lastevent"} ne "n/a"
                    &&  defined( $info{$run}{"p1events"} ) && $info{$run}{"p1events"} > 0 );
        $p1ratio = ( 100 * $info{$run}{"numevents"} / $info{$run}{"p1events"} )
            if ( $info{$run}{"numevents"} ne "n/a"
                    &&  defined( $info{$run}{"p1events"} ) && $info{$run}{"p1events"} > 0 );

	my $tmp = sprintf( "%6d | %3d | %6.2f | %6d | %9d | %8d | %13s |    %3s    | %s\n",
			   $run, $logrecent{$run}, $p1ratio,
                           $info{$run}{"fatalerrors"},
			   $info{$run}{"daexceptions"}, $info{$run}{"diskspaceerrors"},
			   $info{$run}{"stillrunning"}, $doagain, $info{$run}{"runstatus"} );

	lprint $tmp;
	print MAIL $tmp;
    }

    $tmp = "
'queue status' is the variable 'stillrunning'

'do again?' is \"yes\" if this log version of the run will be
                        summarized again the next time
                        checkStatus.pl is run
         it is \"no\"  if the run will only be summarized again if it
                        is processed again by pass2
";

    lprint $tmp;
    print MAIL $tmp;

    close( MAIL );
}

lprint "All done!\n\n";
