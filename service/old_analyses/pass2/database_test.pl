#!/usr/bin/perl

use DBI;

print scalar localtime(time);
print " Starting...\n";

$dbhost = `hostname`;
if ( $dbhost eq "sol301.lns.cornell.edu\n" )
{ $dbhost = "localhost"; }
else
{ $dbhost = "sol301"; }

%dbinfo = ();
@dbvarnames = ( "path", "serverID", "databaseID", "federationID" );
%runs = ();

$dbh = DBI->connect( "DBI:mysql:MetaData:$dbhost", "nobody", "" )    ||  die $DBI::errstr;
#		     { AutoCommit => 0 } )    ||  die $DBI::errstr;
print scalar localtime(time);
print " Connected to database!\n";

$sth = $dbh->prepare( "select * from FileLocation;" );
$sth->execute()   ||  die $dbh::errstr;

print scalar localtime(time);
print " Filling hash table...\n";
while( ( $recordID, $path, $serverID, $HSMdiskID, $timestamp, $databaseID, $federationID )
       = $sth->fetchrow_array() )
{
    if ( $path =~ /Run([0-9]{6})/ )
    {
	$run = $1;
	$runs{$run} = 1;

	if ( ! defined( $dbinfo{$run} ) )
	{
	    $dbinfo{$run} = [ { "path"         => $path,
				"serverID"     => $serverID,
				"databaseID"   =>$databaseID,
				"federationID" => $federationID } ];
	}
	else
	{
	    push( @{ $dbinfo{$run} }, { "path"         => $path,
					"serverID"     => $serverID,
					"databaseID"   =>$databaseID,
					"federationID" => $federationID } );
	}
	
    }
}
print scalar localtime(time);
print " Done!\n";

$dbh->disconnect()    ||  die $DBI::errstr;

sub numly { $a <=> $b; }
foreach $run ( sort numly keys %runs )
{
    print "# Run $run has ". ( $#{ $dbinfo{$run} } + 1 ) ." databases associated with it.\n";
    print "\$dbinfo{$run} = [ ";
    $first = 1;
    foreach $database ( @{ $dbinfo{$run} } )
    {
	%tmp = %{ $database };
	if ( $first == 1 )
	{ $first = 0; }
	else
	{ print ",\n                    "; }
	print ( "{ \"path\" => \"$tmp{path}\",\n                      \"serverID\" => $tmp{serverID}, ".
		"\"databaseID\" => $tmp{databaseID}, \"federationID\" => $tmp{federationID} }" );
    }
    print " ]\n\n";
}
