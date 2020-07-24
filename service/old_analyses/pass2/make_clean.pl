#!/usr/local/bin/perl

# /home/pass2/Stats/completelydone.1.pl   /home/pass2/Stats/majorErrors.1.txt
# /home/pass2/Stats/log.1.txt             /home/pass2/Stats/status.1.pl
# /home/pass2/Stats/majorErrors.1.pl      /home/pass2/Stats/stillProcessing.1.pl

$STATSDIR = "/home/pass2/Stats/";

$today = `date`;
chop( $today );
$permfile = "$STATSDIR/permanant_marks.txt";
open( PERM, ">> $permfile" );
printf( PERM "%-30s /usr/local/bin/perl /home/pass2/Stats/make_clean.pl\n", $today );
close( PERM );

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

foreach $i ( 1..$n )
{
    print "Deleting files associated with execution number $i...\n";
    foreach $file ( "log.$i.txt", "status.$i.pl", "completelydone.$i.pl", "stillProcessing.$i.pl",
		    "majorErrors.$i.txt", "majorErrors.$i.pl" )
    {
	if ( -e "$STATSDIR/$file" )
	{
	    print "    $file : ";
	    unlink( "$STATSDIR/$file" );
	    die "Couldn't delete $STATSDIR/$file!\n" if ( -e "$STATSDIR/$file" );
	    print "gone!\n";
	}
    }
}
print "Finished deleting all files!\n";
