#!/usr/local/bin/perl

###########################################################################
# Important global variables
###########################################################################
$STATSDIR = "/home/pass2/Stats";
$PASS2DIR = "/home/pass2/Pass2Production/pass2/Pass2Scripts";

###########################################################################
# Get a list of the log files and put it in the double-hash "log"
# Syntax:
#   $log{$run}{$logversion} == [ $datetime ]
#   $logrecent{$run} == largest $logversion
#   @runs == sorted list of runs
###########################################################################
$row = 0;

%log = ();
%logrecent = ();
opendir( TOPDIR, $PASS2DIR );
@logdirs = readdir( TOPDIR );
closedir( TOPDIR );

print "Determining which files to read:\n";
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

	( $dev, $ino, $mode, $nlink, $uid, $gid, $rdev, $size, $atime, $mtime, $ctime, $blksize, $blocks )
	    = stat( $fullpath );
	$datetime = $mtime;

	print "    file $PASS2DIR/$logdir/$file\n";

	# Put this run/version into the hash
	$log{$run}{$logversion} = [ $datetime ];
	if ( ! defined( $logrecent{$run} )   ||
	     $logrecent{$run} < $logversion     )
	{
	    $logrecent{$run} = $logversion;
	}
    }
}

sub numly { $a <=> $b; }
@runs = sort numly keys %log;

###########################################################################

%freq = ();
$filenum = 0;
$filetot = ( $#runs + 1 );
print "Reading files:\n";
foreach $run ( @runs )
{
    $logfile = "Log$logrecent{$run}/$run\_pass2.log";
    open( LOG, "$PASS2DIR/$logfile" );
    $filenum++;
    print "    reading $PASS2DIR/$logfile (file $filenum / $filetot)...\n";
    for ( $line = 1;  $text = <LOG>;  $line++ )
    {
	chop( $text );
	if ( substr( $text, 0, 10 ) eq " %% ERROR-" )
	{
	    $proc = "";
	    if ( substr( $text, 10, 8 ) eq "CDOffCal" )
	    { $proc = "CDOffCal"; }
	    elsif ( substr( $text, 10, 7 ) eq "RawData" )
	    { $proc = "RawData"; }
	    elsif ( substr( $text, 10, 9 ) eq "P1Crystal" )
	    { $proc = "P1Crystal"; }
	    elsif ( substr( $text, 10, 14 ) eq "RichClustering" )
	    { $proc = "RichClustering"; }
	    elsif ( substr( $text, 10, 15 ) eq "RICO_PAD_CLUSTM" )
	    { $proc = "RICO_PAD_CLUSTM"; }
	    elsif ( substr( $text, 10, 20 ) eq "CcP2ReconProd.CcBlob" )
	    { $proc = "CcP2ReconProd.CcBlob"; }
	    elsif ( substr( $text, 10, 26 ) eq "Processor.P1CrystalMonProc" )
	    { $proc = "Processor.P1CrystalMonProc"; }
	    elsif ( substr( $text, 10, 6 ) eq "MUACCP" )
	    { $proc = "MUACCP"; }

	    if ( $proc ne "" )
	    {
		if ( ! defined( $freq{$proc}{$text} ) )
		{ $freq{$proc}{$text} = 1; }
		else
		{ $freq{$proc}{$text}++; }
	    }
	}

	elsif ( substr( $text, 0, 14 ) eq " %% EMERGENCY-" )
	{
	    $proc = "";
	    if ( substr( $text, 14, 23 ) eq "BunchFinder.BunchFinder" )
	    { $proc = "BunchFinder.BunchFinder"; }

	    if ( $proc ne "" )
	    {
		if ( ! defined( $freq{$proc}{$text} ) )
		{ $freq{$proc}{$text} = 1; }
		else
		{ $freq{$proc}{$text}++; }
	    }
	}
    }
    close( LOG );
}

sub byfreq
{
    my $tmp = ( $f{$b} <=> $f{$a} );
    $tmp = ( $a cmp $b ) if ( $tmp == 0 );
    $tmp;
}

foreach $proc ( sort keys %freq )
{
    print "Producer/Processor $proc:
---------+==============================================================
";
    %f = %{ $freq{$proc} };
    foreach $text ( sort byfreq keys %f )
    {
	printf( "%8d | %s\n", $f{$text}, $text );
    }

    print "\n\n";
}
