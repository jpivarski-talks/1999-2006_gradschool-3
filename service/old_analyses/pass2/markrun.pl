#!/usr/bin/perl

$usage = "usage: perl markrun.pl [-m|--message \"new value of variable 'runstatus'\"] ###### [###### [###### [ ... ] ] ]
            If a message is omitted, message will become \"ok\".
            (\"ok\" is the official way to denote a run that is good for unstaging and physics use.)
";

@runs = ();
$message = "ok";
for ( $i = 0;  $i <= $#ARGV;  $i++ )
{
    if ( $ARGV[$i] eq "-m"  ||  $ARG[$i] eq "--message" )
    {
	$i++; die $usage if ( $i > $#ARGV );
	$message = $ARGV[$i];
	$ARGV[$i] = "\"$ARGV[$i]\"";
    }
    elsif ( $ARGV[$i] =~ /^[0-9]{6}$/ )
    {
	push( @runs, $ARGV[$i] );
    }
    else
    {
	die $usage;
    }
}

die "No runs selected!\n" if ( $#runs == -1 );

###########################################################################
# Important global variables
###########################################################################
$STATSDIR = "/home/pass2/Stats";
$PASS2DIR = "/home/pass2/Pass2Production/pass2/Pass2Scripts";

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

$outputfile = "$STATSDIR/status.$n.pl";
$prevoutputfile = "$STATSDIR/status.".( $n - 1 ).".pl";

$stillprocessing = "$STATSDIR/stillProcessing.$n.pl";
$prevstillprocessing = "$STATSDIR/stillProcessing.".( $n - 1 ).".pl";

$completelydone = "$STATSDIR/completelydone.$n.pl";
$prevcompletelydone = "$STATSDIR/completelydone.".( $n - 1 ).".pl";

$permfile = "$STATSDIR/permanant_marks.txt";
$today = `date`;
chop( $today );
open( PERM, ">> $permfile" );
printf( PERM "%-30s /usr/local/bin/perl /home/pass2/Stats/markrun.pl %s\n", $today, join( " ", @ARGV ) );
close( PERM );

print "Writing out to $outputfile:\n";

open( OUT, "> $outputfile" );
print OUT "#!/usr/bin/perl

warn \"File \\\"$outputfile\\\" is being executed.\\n\";
";

if ( -e $prevoutputfile )
{
    print OUT "\nrequire( \"$prevoutputfile\" );\n";
}
else
{
    print OUT "\nrequire( \"$initoutputfile\" );\n";
}

print OUT "
###########################################################################
# This is a markrun.pl execution number, not data
###########################################################################

# START EVERYTHING
# START LOG
# STOP LOG
# START LOGRECENT
# STOP LOGRECENT
# START RUNS
# STOP RUNS
# START DBINFO
# STOP DBINFO
# START INFO

";

foreach $run ( @runs )
{
    print OUT "\$info{$run}{\"runstatus\"} = \"$message\";\n";
    print "    \$info{$run}{\"runstatus\"} = \"$message\";\n";
}

print OUT "
# STOP INFO
# STOP EVERYTHING

warn \"File \\\"$outputfile\\\" has finished.\\n\";
";
close( OUT );

############

print "Writing blank $stillprocessing...\n";

open( STILLPROCESSING, "> $stillprocessing" );

print STILLPROCESSING "#!/usr/bin/perl

warn \"File \\\"$stillprocessing\\\" is being executed.\\n\";
";

if ( -e $prevcompletelydone )
{
    print STILLPROCESSING "\nrequire( \"$prevstillprocessing\" );\n";
}

print STILLPROCESSING "
###########################################################################
# This is a markrun.pl execution number, not data
###########################################################################

# START EVERYTHING
# START OLDUNFINISHED
# STOP OLDUNFINISHED
# STOP EVERYTHING

warn \"File \\\"$stillprocessing\\\" has finished.\\n\";
";
close( STILLPROCESSING );

##############

print "Writing blank $completelydone...\n";

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
# This is a markrun.pl execution number, not data
###########################################################################

# START EVERYTHING
# START OLDDONE
# STOP OLDDONE
# STOP EVERYTHING

warn \"File \\\"$completelydone\\\" has finished.\\n\";
";
close( COMPLETELYDONE );

print "All done!\n";
