#!/usr/bin/perl

$PASS2DIR = "/home/pass2/Pass2Production/pass2/Pass2Scripts";
$STATSDIR = "/home/pass2/Stats";

# Get commandline arguments

$searchstring = "false";
$fields = "";
$separate_table_window = 0;
$gui = 1;
$summarize = 0;
for( my $i = 0;  $i <= $#ARGV;  $i++ )
{
    if ( $ARGV[$i] eq "-w" )
    {
	$separate_table_window = 1;
    }
    elsif ( $ARGV[$i] eq "-ng"  ||  $ARGV[$i] eq "--nogui" )
    {
	$gui = 0;
    }
    elsif ( $ARGV[$i] eq "-V" )
    {
	require( "$STATSDIR/initstatus.pl" );
	print "The following logfile variables are available for use:";
	my $row = 0;
	foreach my $v ( @varnames )
	{
	    print "\n    " if ( $row == 0 );
	    printf( "* %-25s ", $v );
	    $row++;
	    $row = 0 if ( $row == 3 );
	}
	print "\nFull documentation on each can be found in section II.B of the help file:
    less -l 89 $STATSDIR/pass2lookup_help.txt
";
	exit( 0 );
    }
    elsif ( $ARGV[$i] eq "-ss" )
    {
	$summarize = 1;
    }
    elsif ( $ARGV[$i] eq "-s"  &&  defined( $ARGV[($i+1)] ) )
    {
	$searchstring = $ARGV[($i+1)];
	$i++;
    }
    elsif ( $ARGV[$i] =~ /^\-s([^ ]+)$/ )
    {
	$searchstring = $1;
    }
    elsif ( $ARGV[$i] eq "-f"  &&  defined( $ARGV[($i+1)] ) )
    {
	$fields = $ARGV[($i+1)];
	$i++;
    }
    elsif ( $ARGV[$i] =~ /^\-f([^ ]+)$/ )
    {
	$fields = $1;
    }
    else
    {
	die "Usage: perl pass2lookup.pl [-ng|--nogui] [-w] [-s 'search string'] [-f 'format string'] [-V]
    -ng or --nogui     Print a table to standard out using values passed to -s and -f
                       without invoking the GUI.
    -w                 If the GUI is invoked, separate the table window from the control window.
    -s 'search string' Provide an initial search string (the ONLY search string if -ng).
    -f 'format string' Provide an initial format string (the ONLY format string if -ng).
    -ss                Show summations at the end of the table (same as the checkbutton).
    -V                 Do nothing but provide a list of logfile variables.
";
    }
}
$searchstring = "false" if ( $searchstring =~ /^[ \t\r\n]*$/ );

$uname = `uname`;
chop( $uname );

if ( $gui )
{
    use Tk;
    require Tk::LabFrame;
    require Tk::Dialog;
    require Tk::DirTree if ( $uname ne "OSF1" );
    require Tk::ItemStyle if ( $uname ne "OSF1" );
    require Tk::ROText;
}
use Time::Local;

$n = 0;
open( LS, "ls $STATSDIR |" );
while( <LS> )
{
    if ( $_ =~ /status\.([0-9]+)\.pl/ )
    {
	$n = $1 if ( $n < $1 );
    }
}
$norecurse = 0;
warn "Loading data from $STATSDIR/status.$n.pl...\n";
require( "$STATSDIR/status.$n.pl" );
warn "Done loading data!\n";
sub numly { $a <=> $b; }
@runs = sort numly keys %log;
foreach my $run ( @runs )
{
    $info{$run}{"run"} = $run;   # so that people can search by run number

    foreach my $varname ( @varnames )
    {
	$info{$run}{$varname} = "n/a" if ( ! defined( $info{$run}{$varname} ) );
    }
}
@varnames = ( "run", @varnames );

@pad = ( -padx => 2, -pady => 2 );
$whoami = `whoami`;
chop( $whoami );

if ( $gui )
{
    # The main window and its major frames
    $w_main = Tk::MainWindow->new;
    $w_main->title( "Pass2 Run Status Lookup" );

    $f_alias = $w_main->Frame()->pack( -fill => "x", -expand => 1, @pad );
    $f_interaction = $w_main->Frame()->pack( -fill => "x", -expand => 1, @pad );
    $f_controls = $f_interaction->Frame()->pack( -side => "left", -fill => "x", -expand => 1, @pad );
    $f_search = $f_controls->LabFrame( -label => "Search String", -labelside => "acrosstop"
				       )->pack( -fill => "x", -expand => 1, @pad );
    $f_comments = $f_controls->LabFrame( -label => "Run Comments", -labelside => "acrosstop"
					 )->pack( -fill => "x", -expand => 1, @pad );
    $f_view = $f_interaction->LabFrame( -label => "Run View", -labelside => "acrosstop"
					)->pack( -side => "right", -fill => "both", -expand => 1, @pad );
    &maketable( $separate_table_window );

    # The alias frame

    $l_alias = $f_alias->Label( -text => "Commandline: " )->pack( -side => "left", @pad );
    $b_alias_exit = $f_alias->Button( -text => "Exit", -command => sub { print "$alias\n"; exit( 0 ); }
				      )->pack( -side => "right", @pad );

    $b_help = $f_alias->Button( -text => "Help", -command => sub {
	my $helpdata = "";
	open( HELPFILE, "$STATSDIR/pass2lookup_help.txt" );
	while( <HELPFILE> )
	{ $helpdata .= $_; }
	close( HELPFILE );
	$w_help = $w_main->Toplevel;
	$w_help->title( "pass2lookup.pl Help File" );
	$t_help = $w_help->Scrolled( ROText, -scrollbars => "e",
				     -wrap => "word"
				     )->pack( -fill => "both",
					      -expand => 1 );
	$t_help->delete( "0.0", "end" );
	$t_help->insert( "end", $helpdata );
	$b_helpexit = $w_help->Button( -text => "Close", -command => sub {
	    $w_help->destroy(); }
				       )->pack(); }
				)->pack( -side => "right", @pad );

    $b_alias = $f_alias->Button( -text => "STDOUT", -command => sub { print "$alias\n"; }
				 )->pack( -side => "right", @pad );
    $ms_alias = $f_alias->Entry( -textvariable => \$alias
				 )->pack( -side => "right", -fill => "x", -expand => 1, @pad );
}

sub updatecommandline
{
    # set this appropraitely when this finds its final home!
    my $tmp = "perl " . $0 . " ";
    $tmp .= "-w " if ( $separate_table_window == 1 );
    $tmp .= "-ss " if ( $summarize == 1 );
    my $tmpss = $searchstring;
    $tmpss =~ tr/\t\n\r/   /;
    $tmp .= "-s '$tmpss' " if ( $tmpss ne "false"  &&  $tmpss =~ /[^ ]/ );
    $tmp .= "-f '$fields' " if ( $fields ne "" );
    $alias = $tmp;
}

if ( $gui )
{
    &updatecommandline();

    # The search frame

    $f_searchentry = $f_search->Frame()->pack( -fill => "x", -expand => 1 );
    $t_searchentry = $f_searchentry->Scrolled( Text, -scrollbars => "oe",
					       -height => 3, -width => 65, -wrap => "char"
					       )->pack( -fill => "x", -expand => 1 );
    $t_searchentry->delete( "0.0", "end" );
    $t_searchentry->insert( "end", $searchstring );
    
    $f_buttons = $f_search->Frame()->pack( -fill => "x", -expand => 1 );
    $m_logfilevars = $f_buttons->Menubutton( -text => "Logfile Variables", -tearoff => 0,
					     -relief => "raised", -indicatoron => 1
					     )->pack( -side => "left", @pad );
    foreach $varname ( @varnames )
    {
	$m_logfilevars->command( -label => $varname, -command
				 => eval( "sub { \$t_searchentry->insert( \"insert\", $varname ); }" ) );
    }

    # $m_databasevars = $f_buttons->Menubutton( -text => "Database Variables", -tearoff => 0,
    # 					  -relief => "raised", -indicatoron => 1
    # 					  )->pack( -side => "left", @pad );
    # foreach $dbvarname ( @dbvarnames )
    # {
    #     $m_databasevars->command( -label => $dbvarname, -command
    # 			      => eval( "sub { \$t_searchentry->insert( \"insert\", $dbvarname ); }" ) );
    # }
    
    $b_search = $f_buttons->Button( -text => "Search", -command
				    => sub{ $searchstring = $t_searchentry->get( "0.0", "end" );
					    $searchstring = "false" if ( $searchstring eq "" );
					    &updatecommandline();
					    &search();
					    &formattable(); }
				    )->pack( -side => "right", @pad );
    $b_clearsearch = $f_buttons->Button( -text => "Clear", -command
					 => sub { $t_searchentry->delete( "0.0", "end" ); }
					 )->pack( -side => "right", @pad );
    
    # The comments frame

    if ( $uname ne "OSF1" )
    {
	$li_comments = $f_comments->Scrolled( HList, -scrollbars => "ose",
					      -height => 5, -header => 1, -columns => 2,
					      -selectforeground => "black", -selectbackground => "lightblue"
					      )->pack( -fill => "x", -expand => 1 );
	$li_columnstyle = $li_comments->ItemStyle( "text" );
	$li_columnstyle->configure( -font => "-*-Courier-Medium-R-Normal--*-120" );
	$li_style = $li_comments->ItemStyle( "text" );
	$li_style->configure( -font => "fixed" );
	$li_comments->header( "create", 0, -text => "UserID", -style => $li_columnstyle );
	$li_comments->header( "create", 1, -text => "Comment", -style => $li_columnstyle );
	
	$t_comments = $f_comments->Scrolled( Text, -scrollbars => "oe",
					     -height => 3, -width => 10, -wrap => "char"
					     )->pack( -fill => "x", -expand => 1 );
	$f_commentsbuttons = $f_comments->Frame()->pack( -fill => "x", -expand => 1 );
	$b_clearcomment = $f_commentsbuttons->Button( -text => "Clear", -command
						      => sub { $t_comments->delete( "0.0", "end" ); }
						      )->pack( -side => "left", @pad );
	$b_submitcomment = $f_commentsbuttons->Button( -text => "Comment Run", -state => "disabled",
						       -command => \&submit_comment
						       )->pack( -side => "right", @pad );
	$b_submitcommentrange = $f_commentsbuttons->Button( -text => "Comment Selected Range",
							    -command => \&submit_comment_range
							    )->pack( -side => "right", @pad );
    }
    else
    {
	$l_comments = $f_comments->Label( -text => "Comments not available on OSF1.",
					  -height => 9
					  )->pack( -fill => "both", -expand => 1, @pad );
    }
}

sub submit_comment_range
{
    if ( $t_table->tagNextrange( "sel", "1.0", "end" ) )
    {
	my $oldrun = $runsel;
	$t_view->delete( "1.0", "end" );
	$t_view->insert( "end", "A comment was submitted to\nthe following runs:\n\n" );
	foreach $runsel ( @subruns )
	{
	    &submit_comment() if ( $t_table->tagNextrange( $runsel, "sel.first", "sel.last" ) );
	    $t_view->insert( "end", "    $runsel\n" );
	}
	$runsel = $oldrun;
    }
    else
    {
	$t_view->delete( "1.0", "end" );
	$t_view->insert( "end", "No group of runs is selected!\n" );
    }
}

if ( $gui )
{
    # The view frame

    $t_view = $f_view->Scrolled( ROText, -scrollbars => "ose",
				 -height => 20, -width => 25, -wrap => "none",
				 )->pack( -fill => "both", -expand => 1, @pad );

    @w_logs = (); @t_logs = ();

    $b_view_logfile = 0;
}

# The table frame

sub maketable {
    my $separate = @_[0];

    if ( $separate )
    {
	$w_table = $w_main->Toplevel;
	$w_table->title( "Pass2 Run Status Table" );
	$f_table = $w_table->Frame()->pack( -fill => "both", -expand => 1, @pad );
    }
    else
    {
	$f_table = $w_main->LabFrame( -label => "Run Table", -labelside => "acrosstop"
				      )->pack( -fill => "both", -expand => 1, @pad );
    }

    $e_fields = $f_table->Scrolled( Entry, -scrollbars => "os", -textvariable => \$fields
				    )->pack( -fill => "x", -expand => 1, @pad );

    $f_tablecontrols = $f_table->Frame( -relief => "raised" )->pack( -fill => "x", -expand => 1, @pad );

    $b_exit = $f_tablecontrols->Button( -text => "Exit", -command => sub{ print "$alias\n"; exit( 0 ); }
					)->pack( -side => "right", @pad );
    if ( $separate )
    {
	$b_tablefree = $f_tablecontrols->Button( -text => "Attach Window", -command
						 => sub{ $w_table->destroy();
							 &maketable( $separate_table_window = 0 );
							 &updatecommandline();
							 &formattable(); }
						 )->pack( -side => "right", @pad );
    }
    else
    {
	$b_tablefree = $f_tablecontrols->Button( -text => "Free Window", -command
						 => sub{ $f_table->destroy();
							 &maketable( $separate_table_window = 1 );
							 &updatecommandline();
							 &formattable(); }
						 )->pack( -side => "right", @pad );
    }
    $b_save = $f_tablecontrols->Button( -text => "Save Table", -command => \&savefile
					)->pack( -side => "right", @pad );

    $b_update = $f_tablecontrols->Button( -text => "Update Table", -command
					  => sub { &updatecommandline(); &formattable(); }
					  )->pack( -side => "left", @pad );

    $m_table_logfilevars = $f_tablecontrols->Menubutton( -text => "Logfile Variables", -tearoff => 0,
							 -relief => "raised", -indicatoron => 1
							 )->pack( -side => "left", @pad );

    $cb_summarize = $f_tablecontrols->Checkbutton( -text => "Show Summations", -variable => \$summarize
						   )->pack( -side => "left", @pad );

    foreach $varname ( @varnames )
    {
	$m_table_logfilevars->command( -label => $varname, -command
				       => eval( "sub { \$e_fields->insert( \"insert\", $varname ); }" ) );
    }

    if ( $separate )
    {
	$t_table = $f_table->Scrolled( ROText, -scrollbars => "ose",
				       -height => 35, -width => 110, -wrap => "none",
				       -exportselection => 1
				       )->pack( -fill => "both", -expand => 1, @pad );
    }
    else
    {
	$t_table = $f_table->Scrolled( ROText, -scrollbars => "ose",
				       -height => 10, -width => 50, -wrap => "none",
				       -exportselection => 1
				       )->pack( -fill => "both", -expand => 1, @pad );
    }

    $t_table->bind( "<ButtonRelease>", sub
		    { $t_table->tagAdd( "sel", "sel.first linestart", "sel.last lineend + 1 chars" )
			  if ( $t_table->tagNextrange( "sel", "1.0", "end" ) ); } );
}

sub savefile
{
    $savedialog = $w_main->Dialog( -title => "Save Table as...",
				   -default_button => "Save", -buttons => [ "Cancel", "Save" ] )
	if ( ! Exists( $savedialog ) );

    $filename = `pwd`;
    chop( $filename );
    $filename .= "/";

    my $f_sd = $savedialog->Frame()->pack( -fill => "both", -expand => 1 );
    my $directories;
    if ( $uname ne "OSF1" )
    {
	$directories = $f_sd->Scrolled( DirTree, -scrollbars => "ose", -width => 70,
					-command => sub { my $dirname = @_[0];
							  $dirname =~ s/^\/\//\//;
							  $filename =~ /\/([^\/]+)$/;
							  $filename = $1;
							  $filename = "$dirname/$filename"; }
					)->pack( -fill => "both", -expand => 1, @pad );
    }
    else
    {
	$directories = $f_sd->Label( -text => "Can't browse directories on OSF1.",
				     -height => 9
				     )->pack( -fill => "both", -expand => 1, @pad );
    }
    my $filebox = $f_sd->Entry( -textvariable => \$filename, -width => 70
				)->pack( -fill => "x", -expand => 1, @pad );

    my $retval = $savedialog->Show();

    if ( $retval eq "Save" )
    {
	open( FILE, "> $filename" );

	my $tabledata = $t_table->get( "0.0", "end" );
	print FILE $tabledata;

	close( FILE );
    }
}

sub search
{
    my $realsearch = " $searchstring ";
    foreach $varname ( @varnames )
    {
	$realsearch =~ s/([^A-Za-z0-9]{1})($varname)([^A-Za-z0-9]{1})/$1\$info\{\$run\}\{$2\}$3/g;
    }
#     foreach $dbvarname ( @dbvarnames )
#     {
# 	$realsearch =~ s/([^A-Za-z0-9]{1})($dbvarname)([^A-Za-z0-9]{1})/$1\$dbinfo\{\$run\}\{$2\}$3/g;
#     }

    $realsearch =~ s/([^A-Za-z0-9]{1})false([^A-Za-z0-9]{1})/$1 0 $2/g;
    $realsearch =~ s/([^A-Za-z0-9]{1})true([^A-Za-z0-9]{1})/$1 1 $2/g;
    $realsearch =~ s/([^A-Za-z0-9]{1})and([^A-Za-z0-9]{1})/$1 && $2/g;
    $realsearch =~ s/([^A-Za-z0-9]{1})or([^A-Za-z0-9]{1})/$1 || $2/g;
    $realsearch =~ s/([^A-Za-z0-9]{1})not([^A-Za-z0-9]{1})/$1 ! $2/g;

    while ( $realsearch =~ /\"([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})\"/ )
    {
	my ( $h, $m, $s ) = ( $1, $2, $3 );
	my $numtime = ($h * 60 * 60 ) + ( $m * 60 ) + $s;
	$realsearch =~ s/\"$h:$m:$s\"/$numtime/;
    }

    while ( $realsearch =~ /\"([0-9]+)\s*[dD]\s*([0-9]{1,2}):([0-9]{1,2}):([0-9]{1,2})\"/ )
    {
	my ( $d, $h, $m, $s ) = ( $1, $2, $3, $4 );
	my $numtime = ( $d * 24 * 60 * 60 ) + ($h * 60 * 60 ) + ( $m * 60 ) + $s;
	$realsearch =~ s/\"$d\s*[dD]\s*$h:$m:$s\"/$numtime/;
    }

    while ( $realsearch =~ /\"([0-9]+)\s*\/\s*([0-9]+)\s*\/\s*([0-9]+)\s+([0-9]+)\s*:\s*([0-9]+)\s*:\s*([0-9]+)\"/ )
    {
	my ( $mon, $day, $year, $hour, $min, $sec ) = ( $1, $2, $3, $4, $5, $6 );
	$mon--;
	$year += 100 if ( $year < 70 );  # We have a Y2K70 (2070) bug.
	my $numdate = timelocal( $sec, $min, $hour, $day, $mon, $year );
	$realsearch =~ s/\"$1\s*\/\s*$2\s*\/\s*$3\s+$4\s*:\s*$5\s*:\s*$6\"/$numdate/;
    }

    my %subrunhash = ();
    foreach my $run ( @runs )
    {
	eval( "\$subrunhash{\$run} = 1 if ( $realsearch );" );
	if ( $@ ne "" )
	{
	    if ( $gui )
	    {
		$t_view->delete( "0.0", "end" );
		$t_view->insert( "end", "Bad search string!\n$@\n" );
		last;
	    }
	    else
	    {
		die "Bad search string!\n$@\n";
	    }
	}
    }
    @subruns = keys %subrunhash;  # subruns is intentionally global
}
&search();

sub bysortnumber { ${ $a }[1] <=> ${ $b }[1]; }

sub bysortorder
{
    my $retval = 0;
    foreach my $var ( @sortorder )
    {
	my ( $field, $number, $order ) = @{ $var };
	
	if ( defined( $numericalvar{$field} )   ||
	     defined( $timevar{$field} )        ||
	     defined( $datevar{$field} )            )
	{
	    $retval = ( $info{$a}{$field} <=> $info{$b}{$field} );
	}
	else
	{
	    $retval = ( $info{$a}{$field} cmp $info{$b}{$field} );
	}

	$retval *= -1 if ( $order eq "-" );
	last if $retval != 0;
    }
    $retval = ( $a <=> $b ) if ( $retval == 0 );
    return $retval;
}

sub timestring
{
    my $unformattedstring = @_[0];
    my $seconds = $unformattedstring;
    my $minutes = int( $seconds / 60 );   $seconds = $seconds % 60;
    my $hours = int( $minutes / 60 );     $minutes = $minutes % 60;
    my $days = int( $hours / 24 );        $hours = $hours % 24;
    $seconds = sprintf( "%02d", $seconds );
    $minutes = sprintf( "%02d", $minutes );
    $hours = sprintf( "%02d", $hours );
    
    my $timestring = "$hours:$minutes:$seconds";
    if ( $days != 0 )
    { $timestring = sprintf( "%d d $timestring", $days ); }
    else
    { $timestring = "$timestring"; }
    
    return $timestring;
}

sub datestring
{
    my $unformattedstring = @_[0];
    my ( $s, $m, $h, $d, $mon, $y, $wday, $yday, $isdst )
	= localtime( $unformattedstring );
    $s = sprintf( "%02d", $s );
    $m = sprintf( "%02d", $m );
    $h = sprintf( "%02d", $h );
    $d = sprintf( "%02d", $d );
    $mon = sprintf( "%02d", ( $mon + 1 ) );
    $y = sprintf( "%02d", ( $y % 100 ) );

    return "$mon/$d/$y $h:$m:$s";
}

sub formattable
{
    my $fieldscopy = $fields;
    $fieldscopy = "run" if ( $fieldscopy eq "" );
    my @fields = ();
    @sortorder = ();  # intentionally global

    while( $fieldscopy ne "" )
    {
	if ( substr( $fieldscopy, 0, 1 ) =~ /\s{1}/o )
	{
	    $fieldscopy =~ s/^(\s+)//;
	    push( @fields, $1 );
	}

	elsif ( substr( $fieldscopy, 0, 1 ) eq "|" )
	{
	    $fieldscopy = substr( $fieldscopy, 1 );
	    push( @fields, "|" );
	}

	elsif ( substr( $fieldscopy, 0, 1 ) =~ /[a-zA-Z0-9]{1}/o )
	{
	    $fieldscopy =~ s/^([a-zA-Z0-9]+)//;
	    push( @fields, $1 );
	    my $isvarname = 0;
	    foreach $varname ( @varnames )
	    {
		if ( $1 eq $varname )
		{
		    $isvarname = 1;
		    last;
		}
	    }
	    if ( $isvarname == 0 )
	    {
		if ( $gui )
		{
		    $t_view->delete( "0.0", "end" );
		    $t_view->insert( "end", "Bad format string!\n" );
		    my $wheredied = length( $fields ) - length( $fieldscopy );
		    $t_view->insert( "end", "$fields\n" );
		    foreach my $i ( 1..( $wheredied - 1 ) )
		    {
			$t_view->insert( "end", "-" );
		    }
		    $t_view->insert( "end", "^\n\"$fields[$#fields]\" is not a logfile variable!\n" );
		    return;
		}
		else
		{
		    warn "Bad format string!\n";
		    my $wheredied = length( $fields ) - length( $fieldscopy );
		    warn "$fields\n";
		    my $arrow = "";
		    foreach my $i ( 1..( $wheredied - 1 ) )
		    {
			$arrow .= "-";
		    }
		    warn "$arrow^\n";
		    die "\"$fields[$#fields]\" is not a logfile variable!\n";
		}
	    }
	    if ( substr( $fieldscopy, 0, 1 ) eq "(" )
	    {
		if ( $fieldscopy =~ /^\([\-\+]{0,1}([0-9]*\.{0,1}[0-9]*)\s*[\-\+]{0,1}\)/o  &&
		     $1 =~ /[0-9]{1}/o )
		{
		    $fieldscopy =~ s/^\(([\-\+]{0,1}[0-9]*\.{0,1}[0-9]*)\s*([\-\+]{0,1})\)//;
		    push( @sortorder, [ $fields[$#fields], $1, $2 ] );
		}
		else
		{
		    if ( $gui )
		    {
			$t_view->delete( "0.0", "end" );
			$t_view->insert( "end", "Bad format string!\n" );
			my $wheredied = length( $fields ) - length( $fieldscopy );
			$t_view->insert( "end", "$fields\n" );
			foreach my $i ( 1..( $wheredied - 1 ) )
			{
			    $t_view->insert( "end", "-" );
			}
			$t_view->insert( "end", "^\nBadly formatted sort order criteria!\n" );
			return;
		    }
		    else
		    {
			warn "Bad format string!\n";
			my $wheredied = length( $fields ) - length( $fieldscopy );
			warn "$fields\n";
			my $arrow = "";
			foreach my $i ( 1..( $wheredied - 1 ) )
			{
			    $arrow .= "-";
			}
			warn "$arrow^\n";
			die "Badly formatted sort order criteria!\n";
		    }
		}
	    }
	}

	else
	{
	    if ( $gui )
	    {
		$t_view->delete( "0.0", "end" );
		$t_view->insert( "end", "Bad format string!\n" );
		my $wheredied = length( $fields ) - length( $fieldscopy );
		$t_view->insert( "end", "$fields\n" );
		foreach my $i ( 1..( $wheredied - 1 ) )
		{
		    $t_view->insert( "end", "-" );
		}
		$t_view->insert( "end", "^\nNot a variable name, space or \"|\"!\n" );
		return;
	    }
	    else
	    {
		warn "Bad format string!\n";
		my $wheredied = length( $fields ) - length( $fieldscopy );
		warn "$fields\n";
		my $arrow = "";
		foreach my $i ( 1..( $wheredied - 1 ) )
		{
		    $arrow .= "-";
		}
		warn "$arrow^\n";
		die "Not a variable name, space or \"|\"!\n";
	    }
	}
    }
    @sortorder = sort bysortnumber @sortorder;
    @subruns = sort bysortorder @subruns;

    my %summation = ();

    my @lines = ();
    my %beforedecimal = ();
    my %afterdecimal = ();

    # The header line with all the field titles
    {
	my @line = ();
	foreach my $field ( @fields )
	{
	    $beforedecimal{$field} = 0;
	    $afterdecimal{$field} = 0;

	    push( @line, $field );
	}
	push( @lines, [ @line ] );
    }

    # The horizontal rule separating the header line from the data
    {
	my @line = ();
	foreach my $field ( @fields )
	{
	    if ( $field eq "|" )
	    { push( @line, "+" ); }
	    else
	    { push( @line, "-" ); }
	}
	push( @lines, [ @line ] );
    }

    # The data lines
    foreach my $run ( @subruns )
    {
	my @line = ();
	foreach my $field ( @fields )
	{
	    if ( $field =~ /^\s+$/o  ||  $field eq "|" )
	    {
		push( @line, $field );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );
	    }

	    elsif ( $field eq "run" )
	    {
		push( @line, $run );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );

		# Just want a total number here
		if ( ! defined( $summation{$field} ) )
		{ $summation{$field} = 1; }
		else
		{ $summation{$field}++; }
	    }

	    elsif ( $info{$run}{$field} eq "n/a" )
	    {
		push( @line, "n/a" );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );
	    }

	    elsif ( defined( $numericalvar{$field} ) )
	    {
		my $trial = sprintf( "%g", $info{$run}{$field} );
		$trial = sprintf( "%6g", $info{$run}{$field} )
		    if ( length( $trial ) > 6 );

		push( @line, $trial );
		if ( $line[$#line] =~ /^([^\.]*)\.([^\.]*)$/ )
		{
		    $beforedecimal{$field} = length( $1 )
			if ( $beforedecimal{$field} < length( $1 ) );
		    $afterdecimal{$field} = length( $2 )
			if ( $afterdecimal{$field} < length( $2 ) );
		}
		else
		{
		    $beforedecimal{$field} = length( $line[$#line] )
			if ( $beforedecimal{$field} < length( $line[$#line] ) );
		}

		# accumulate sums
		if ( ! defined( $summation{$field} ) )
		{ $summation{$field} = $info{$run}{$field}; }
		else { $summation{$field} += $info{$run}{$field}; }

	    }

	    elsif ( defined( $timevar{$field} ) )
	    {
		push( @line, &timestring( $info{$run}{$field} ) );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );

		# accumulate sums
		if ( ! defined( $summation{$field} ) )
		{ $summation{$field} = $info{$run}{$field}; }
		else { $summation{$field} += $info{$run}{$field}; }
	    }

	    elsif ( defined( $datevar{$field} ) )
	    {
		push( @line, &datestring( $info{$run}{$field} ) );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );
	    }

	    else
	    {
		push( @line, $info{$run}{$field} );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );
	    }

	} # end foreach field

	push( @lines, [ @line ] );

    } # end loop over runs

    # The horizontal rule separating the data lines from the summations
    {
	my @line = ();
	foreach my $field ( @fields )
	{
	    if ( $field eq "|" )
	    { push( @line, "+" ); }
	    else
	    { push( @line, "-" ); }
	}
	push( @lines, [ @line ] );
    }

    # The summation lines
    if ( $summarize == 1 )
    {
	my @line = ();
	foreach my $field ( @fields )
	{
	    if ( $field =~ /^\s+$/o  ||  $field eq "|" )
	    {
		push( @line, $field );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );
	    }

	    elsif ( $field eq "run" )
	    {
		push( @line, $summation{$field} );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );
	    }

	    elsif ( defined( $numericalvar{$field} )  ||  defined( $timevar{$field} ) )
	    {
		my $ending;
		if ( defined( $timevar{$field} ) )
		{ $ending = " s"; }
		else
		{ $ending = ""; }

		my $trial = sprintf( "%g$ending", $summation{$field} );
		$trial = sprintf( "%6g$ending", $summation{$field} )
		    if ( length( $trial ) > 6 );

		push( @line, $trial );
		if ( $line[$#line] =~ /^([^\.]*)\.([^\.]*)$/ )
		{
		    $beforedecimal{$field} = length( $1 )
			if ( $beforedecimal{$field} < length( $1 ) );
		    $afterdecimal{$field} = length( $2 )
			if ( $afterdecimal{$field} < length( $2 ) );
		}
		else
		{
		    $beforedecimal{$field} = length( $line[$#line] )
			if ( $beforedecimal{$field} < length( $line[$#line] ) );
		}
	    }

	    else
	    {
		push( @line, "" );
		$beforedecimal{$field} = length( $line[$#line] )
		    if ( $beforedecimal{$field} < length( $line[$#line] ) );
	    }
	}
	push( @lines, [ @line ] );
    }

    $t_table->delete( "0.0", "end" ) if ( $gui );

    my $whichline = 1;
    foreach my $lineref ( @lines )
    {
	my $lineout = "";
	my @line = @{ $lineref };
	for( $i = 0;  $i <= $#line;  $i++ )
	{
	    my $field = $fields[$i];
	    my $line = $line[$i];

	    if ( $whichline == 1 )     # the header line
	    {
		my $howmany = $beforedecimal{$field} + $afterdecimal{$field};
		$howmany++ if ( defined( $numericalvar{$field} ) );
		$howmany++ if ( defined( $timevar{$field} )  &&  $summarize == 1 );

		if ( length( $line ) > $howmany )
		{
		    $beforedecimal{$field} += length( $line ) - $howmany;
		    $howmany = length( $line );
		}

		my $formatline = "%-$howmany"."s";
		$lineout .= sprintf( $formatline, $line );
	    }

	    elsif ( $whichline == 2  ||  $whichline == $#lines )  # the separator line
	    {
		if ( $line eq "+" )
		{
		    $lineout .= "+";
		}
		else
		{
		    my $howmany = $beforedecimal{$field} + $afterdecimal{$field};
		    $howmany++ if ( $numericalvar{$field} );
		    $howmany++ if ( defined( $timevar{$field} )  &&  $summarize == 1 );
		    foreach my $char ( 1..$howmany )
		    {
			$lineout .= "-";
		    }
		}
	    }

	    else                       # a data line
	    {
		if ( defined( $numericalvar{$field} )  ||
		     defined( $timevar{$field} ) )
		{
		    if ( $line =~ /^([^\.]*)\.([^\.]*)$/ )
		    {
			my $formatline = "%+$beforedecimal{$field}s.%-$afterdecimal{$field}s";
			$lineout .= sprintf( $formatline, $1, $2 );
		    }
		    else
		    {
			my $formatline = "%+$beforedecimal{$field}s %+$afterdecimal{$field}s";
			$lineout .= sprintf( $formatline, $line, "" );
		    }
		}
		else
		{
		    my $formatline = "%+$beforedecimal{$field}s";
		    $lineout .= sprintf( $formatline, $line );
		}
	    }
	}
	$lineout .= "\n";

	if ( $whichline > 2  &&  $whichline < $#lines )
	{
	    my $run = $subruns[( $whichline - 3 )];
	    if ( $gui )
	    {
		$t_table->insert( "end", $lineout, $run );
		$t_table->tag( "bind", $run, "<1>" => eval( "sub{ &view_run( $run ); }" ) );
	    }
	    else
	    {
		print $lineout;
	    }
	}
	elsif ( ! ( $summarize == 0  &&  $whichline >= $#lines ) )
	{
	    if ( $gui )
	    {
		$t_table->insert( "end", $lineout );
	    }
	    else
	    {
		print $lineout;
	    }
	}

	$whichline++;
    }
}
&formattable();

sub view_run
{
    my $run = @_[0];
    $runsel = $run;   # intentionally global

    $t_view->delete( "0.0", "end" );
    $t_view->insert( "end", "Run $run (using logfile number $logrecent{$run}):\n" );
    foreach my $varname ( @varnames )
    {
	my $value = $info{$run}{$varname};

	if ( defined( $timevar{$varname} ) )
	{ $value = &timestring( $value ); }

	elsif ( defined( $datevar{$varname} ) )
	{ $value = &datestring( $value ); }

	$t_view->insert( "end", "    $varname = $value\n" );
    }
    
    $t_view->insert( "end", "\n" );

    $b_view_logfile = $t_view->Button( -text => "View Log for Run $run", -command => sub {
	system( "xterm -font fixed +sb -T 'less Log$logrecent{$run}/$runsel\_pass2.log' -e less -S $PASS2DIR/Log$logrecent{$run}/$runsel\_pass2.log &" ); }, -cursor  => "top_left_arrow" );

    $t_view->window( "create", "end", -window => $b_view_logfile, @pad );

    $t_view->insert( "end", "\n\nRun $run has " . ( $#{ $dbinfo{$run} } + 1 ) .
		     " databases associated with it:\n" );

    foreach my $database ( @{ $dbinfo{$run} } )
    {
	my %tmp = %{ $database };
	foreach my $dbvarname ( @dbvarnames )
	{
	    my $value = $tmp{$dbvarname};

	    if ( defined( $dbtimevar{$dbvarname} ) )
	    { $value = &timestring( $value ); }

	    elsif ( defined( $dbdatevar{$dbvarname} ) )
	    { $value = &datestring( $value ); }

	    $t_view->insert( "end", "    $dbvarname = $value\n" );
	}
	$t_view->insert( "end", "\n" );
    }

    &update_comments( $run );
}

sub commently { $a->[1] <=> $b->[1]; }

sub update_comments
{
    my $run = @_[0];
    $f_comments->configure( -label => "Run Comments for $run" );
    return if ( $uname eq "OSF1" );
    $b_submitcomment->configure( -text => "Comment Run $run", -state => "normal" );

    my $parent = substr( $run, 0, 3 );
    my $child = substr( $run, 3, 3 );

    opendir( COMMENTSDIR, "$STATSDIR/comments/$parent/$child" );
    my @commentsdir = readdir( COMMENTSDIR );
    closedir( COMMENTSDIR );

    my @relevantcomments = ();       # now mostly to get rid of "." and ".."
    foreach my $c ( @commentsdir )   # but also good for losing other junk, should it exist
    {
	next if ( $c !~ /^c([0-9]+)_([A-Za-z0-9]+)\.txt$/o );
	my $num = $1;
	my $user = $2;

	open( COMMENTFILE, "$STATSDIR/comments/$parent/$child/$c" );
	my $contents = "";
	while( $_ = <COMMENTFILE> )
	{ $contents .= $_; }
	close( COMMENTFILE );
	$contents =~ s/^[ \t\r\n]*//;
	$contents =~ s/[ \t\r\n]*$//;

	push( @relevantcomments, [ $c, $num, $contents, $user ] );
    }
    @relevantcomments = sort commently @relevantcomments;

    $li_comments->delete( "all" );
    foreach my $r ( @relevantcomments )
    {
	my $child = $li_comments->addchild( "" );
	$li_comments->itemCreate( $child, 0, -text => $r->[3], -style => $li_style );
	$li_comments->itemCreate( $child, 1, -text => $r->[2], -style => $li_style );
    }
}

sub submit_comment
{
    return if ( $runsel !~ /^[0-9]{6}$/o );

    my $comment = $t_comments->get( "0.0", "end" );
    
    my $parent = substr( $runsel, 0, 3 );
    my $child = substr( $runsel, 3, 3 );

    opendir( COMMENTSDIR, "$STATSDIR/comments/$parent/$child" );
    my @commentsdir = readdir( COMMENTSDIR );
    closedir( COMMENTSDIR );
    
    my $n = 0;
    foreach my $c ( @commentsdir )
    {
	next if ( $c !~ /^c([0-9]+)_[A-Za-z0-9]+\.txt$/o );
	$n = $1 if ( $n < $1 );
    }
    $n++;

    open( COMMENTSFILE, "> $STATSDIR/comments/$parent/$child/c$n\_$whoami\.txt" );
    print COMMENTSFILE $comment;
    close( COMMENTSFILE );

    &update_comments( $runsel );
}

MainLoop() if ( $gui );
