#!/usr/bin/perl

use POSIX; # time
use DBI;

$altitude = 600;  # nadmořská výška v metrech
 
#$DATE = "1.9.2008";

$dir = "/opt/rrdt";
$rrd_db = $dir . "/sensor01.rrd";

$dbh = DBI->connect('DBI:mysql:hujer_net:localhost:3306', 'master', 'z8pis');
  
#$year -= 1900;
#$start =  mktime(0,0,0,$day,$mon,$year,0,0,-1) - 14000 ;

$end = $start + 120000;
$now = int($end / 300);

$sth = $dbh->prepare("select timesec,
	                   substring(date,1,4) rok, 
			   date,
			   time,
			   teplota1,
			   tlak,
			   vlhkost,
			   batery 
	              from teploty 
		      where sensor = 'P139_BME280_01' and timesec = (select max(timesec) from teploty where sensor = 'P139_BME280_01');
		    ");
$sth->execute();


($timesec, $rok, $datum, $cas, $t01, $p01, $h01, $v01 ) = $sth->fetchrow_array; 

####  Docasne 

if ( 0 ) {

$sth = $dbh->prepare("select timesec,
			   teplota1
	              from teploty 
		      where  timesec = (select max(timesec) from teploty);
		    ");

$sth->execute();

($timesec, $t01 ) = $sth->fetchrow_array; 

}

$sth->finish();


#
#  Přepočet na hladinu moře
#
$p02 = sprintf( "%.2f", $p01 + $altitude / 8.3 );

$cmd = "/usr/bin/rrdtool update ". $rrd_db ." N:". $t01 .":". $p02 .":". $h01 .":". $v01; 

print $cmd ."\n";

`$cmd` ;


$dbh->disconnect;


exit;



