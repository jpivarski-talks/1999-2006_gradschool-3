# Any variables *currently* defined in this script will be accessible
# in CoolTool (after checkStatus.pl has been run for it). Any runs
# processed before the new variable was added will have "n/a" as a
# value for the new variable, unless the ~pass2/Stats directory is
# cleaned and checkStatus.pl is run over everything (not a bad idea to
# do now and then).

#### APPEND VARIABLES TO LIST ###############################################

# add new logfile variables with this subroutine call

# arguments are ( name, type ) where type may be "s" (text string),
# "n" (numerical), "t" (time difference) or "d" (absolute date)

&addvar( "totalPassed", "n" );
&addvar( "percentPassed", "n" );

&addvar( "moreCalibratedSVZthanSpace", "n" );
&addvar( "manyZhitsMakingTable", "n" );
&addvar( "deleteRICHarraySizesExceeded", "n" );

#### DEFINE THEIR CONTENT ###################################################

# for those variables that require initialization, initvar allows you
# to set a starting value. Note that any initialized variable won't
# ever be "n/a".

&initvar( "moreCalibratedSVZthanSpace", 0 );
&initvar( "manyZhitsMakingTable", 0 );
&initvar( "deleteRICHarraySizesExceeded", 0 );

# "flexvars" is a function which is called every time a line of text
# is seen in the logfile (before all other checks are made). Using or
# not using a line here won't affect whether a line is used or not
# used by any of the other variables

sub flexvars
{
    # get a line of text from the logfile, passed as an argument
    my $logline = @_[0];

    # perl regular expressions are described on pages 57 - 76 of the
    # blue-camel perl book and in "man perlre"
    if ( $logline =~ /\*{9}\s+([0-9]+)\s+\(([0-9\.]+)\%\)\s+Total Passed\s+\*{9}/o )
    {
	# "my" denotes a local variable. Without "my", the variable is
	# global and could garble the outer program
	my $total = $1;
	my $percent = $2;

	# set the variable's content with this subroutine call
	&varcontent( "totalPassed", $total );
	&varcontent( "percentPassed", $percent );
    }

    elsif ( $logline =~ /WARNING-Doit\.DoitTrackFinder:  I have [0-9]+ CalibratedSVZHits, space exists only for [0-9]+/o )
    {
	# varincrement adds one to the variable's current value
	&varincrement( "moreCalibratedSVZthanSpace" );
    }

    elsif ( $logline =~ /Found more than [0-9]+ z hits while trying to make FATable<CalibratedSVZHits>/o )
    {
	# could increment this way instead
	&varcontent( "manyZhitsMakingTable", ( &getvar( "manyZhitsMakingTable" ) + 1 ) );
    }

    elsif ( $logline =~ /Delete all RICH clusters since array sizes are exceeded/o )
    {
	&varincrement( "deleteRICHarraySizesExceeded" );
    }

    # make a long elsif chain ("elsif" == "else if", but perl requires
    # the "elsif" notation) ((perl also requires if statements to be
    # defined in blocks--- you always need curly-brackets))

    elsif ( 0 )
    {
	;
    }
}

# If this doesn't return true, the process will die.
1;
