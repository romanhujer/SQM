#!/bin/bash



dir=/opt/rrdt

rrd_db=$dir/sensor01.rrd

GRAF=$1

if [ "x$GRAF" = "xCRE" ]
then 
echo Create

start=1434284000


rrdtool create $rrd_db \
--step 60  \
--start $start \
DS:T01:GAUGE:600:-50:100 \
DS:P01:GAUGE:600:500:2000 \
DS:H01:GAUGE:600:0:100 \
DS:V01:GAUGE:600:0:24 \
RRA:AVERAGE:0.5:1:1500 \
RRA:AVERAGE:0.5:5:2028 \
RRA:AVERAGE:0.5:60:750 \
RRA:AVERAGE:0.5:720:732


# 1500 - 25 hodin (1 hodnota po minute) 
# 2028 - 7 dni a 1 hod (5 hodnot po minute)
# 750  - 31 dni a 6 hodin (60 hodnot po minute (1 hodina))
# 732 - 1 rok a 1 den (720 hodnot po minute (12 hodin))

#RRA:AVERAGE:0.5:1:108300 \
#RRA:AVERAGE:0.5:5:21660 \
#RRA:AVERAGE:0.5:60:1805 \
#RRA:AVERAGE:0.5:720:732


exit

fi 



start_hour=$((`date +%s`-7200))
start_day=$((`date +%s`-86400))
start_week=$((`date +%s`-604800))
start_month=$((`date +%s`-2678400))
start_year=$((`date +%s`-31536000))
end_now=$((`date +%s`-300))



rrdtool graph $dir/www/sensor01_day_small.png \
--title="Sensor01 den" \
--x-grid HOUR:1:HOUR:3:HOUR:3:0:%H:%M \
--start $start_day \
--end $end_now \
--vertical-label "°C hPa+1000 10x% Volt" \
--width 500 \
--height 180 \
--rigid \
DEF:t01=$rrd_db:T01:AVERAGE \
DEF:p01=$rrd_db:P01:AVERAGE \
DEF:h01=$rrd_db:H01:AVERAGE \
DEF:v01=$rrd_db:V01:AVERAGE \
COMMENT:" \\n" \
CDEF:p02=p01,1000,- \
CDEF:h02=h01,10,/ \
AREA:t01#A000A0:"Teplota °C\\t" \
LINE1:p02#FF0000:"Tlak hPa\\t" \
LINE1:h02#0000FF:"Vlhkost %\\t" \
LINE1:v01#00FF00:"Baterie V\\t" \
COMMENT:" \\n" \
GPRINT:t01:LAST:"Teď\\: %7.2lf\t" \
GPRINT:p01:LAST:" %9.2lf\t" \
GPRINT:h01:LAST:" %10.2lf\t" \
GPRINT:v01:LAST:" %8.2lf\n" \
GPRINT:t01:MAX:"Max\\: %7.2lf\t" \
GPRINT:p01:MAX:" %9.2lf\t" \
GPRINT:h01:MAX:" %10.2lf\t" \
GPRINT:v01:MAX:" %8.2lf\n" \
GPRINT:t01:MIN:"Min\\: %7.2lf\t" \
GPRINT:p01:MIN:" %9.2lf\t" \
GPRINT:h01:MIN:" %10.2lf\t" \
GPRINT:v01:MIN:" %8.2lf\t" \
COMMENT:" \\n"





rrdtool graph $dir/www/sensor01_week_small.png \
--title="Sensor01 den týden" \
--x-grid HOUR:6:HOUR:24:HOUR:24:0:" %A" \
--start $start_week \
--end $end_now \
--vertical-label "°C hPa+1000 10x% Volt" \
--width 500 \
--height 180  \
--rigid \
DEF:t01=$rrd_db:T01:AVERAGE \
DEF:p01=$rrd_db:P01:AVERAGE \
DEF:h01=$rrd_db:H01:AVERAGE \
DEF:v01=$rrd_db:V01:AVERAGE \
COMMENT:" \\n" \
CDEF:p02=p01,1000,- \
CDEF:h02=h01,10,/ \
AREA:t01#A000A0:"Teplota \\t" \
LINE1:p02#FF0000:"Tlak hPa\\t" \
LINE1:h02#0000FF:"Vlhkost %\\t" \
LINE1:v01#00FF00:"Baterie V\\t" \
COMMENT:" \\n" \
GPRINT:t01:LAST:"Teď\\: %7.2lf\t" \
GPRINT:p01:LAST:" %9.2lf\t" \
GPRINT:h01:LAST:" %10.2lf\t" \
GPRINT:v01:LAST:" %8.2lf\n" \
GPRINT:t01:MAX:"Max\\: %7.2lf\t" \
GPRINT:p01:MAX:" %9.2lf\t" \
GPRINT:h01:MAX:" %10.2lf\t" \
GPRINT:v01:MAX:" %8.2lf\n" \
GPRINT:t01:MIN:"Min\\: %7.2lf\t" \
GPRINT:p01:MIN:" %9.2lf\t" \
GPRINT:h01:MIN:" %10.2lf\t" \
GPRINT:v01:MIN:" %8.2lf\t" \
COMMENT:" \\n"



rrdtool graph $dir/www/sensor01_monht_small.png \
--title="Sensor01 měsíc" \
--x-grid DAY:1:WEEK:1:WEEK:1:0:"Týden %W" \
--start $start_month \
--end $end_now \
--vertical-label "°C hPa+1000 10x% Volt" \
--width 500 \
--height 180  \
--rigid \
DEF:t01=$rrd_db:T01:AVERAGE \
DEF:p01=$rrd_db:P01:AVERAGE \
DEF:h01=$rrd_db:H01:AVERAGE \
DEF:v01=$rrd_db:V01:AVERAGE \
COMMENT:" \\n" \
CDEF:p02=p01,1000,- \
CDEF:h02=h01,10,/ \
AREA:t01#A000A0:"Teplota \\t" \
LINE1:p02#FF0000:"Tlak hPa\\t" \
LINE1:h02#0000FF:"Vlhkost %\\t" \
LINE1:v01#00FF00:"Baterie V\\t" \
COMMENT:" \\n" \
GPRINT:t01:LAST:"Teď\\: %7.2lf\t" \
GPRINT:p01:LAST:" %9.2lf\t" \
GPRINT:h01:LAST:" %10.2lf\t" \
GPRINT:v01:LAST:" %8.2lf\n" \
GPRINT:t01:MAX:"Max\\: %7.2lf\t" \
GPRINT:p01:MAX:" %9.2lf\t" \
GPRINT:h01:MAX:" %10.2lf\t" \
GPRINT:v01:MAX:" %8.2lf\n" \
GPRINT:t01:MIN:"Min\\: %7.2lf\t" \
GPRINT:p01:MIN:" %9.2lf\t" \
GPRINT:h01:MIN:" %10.2lf\t" \
GPRINT:v01:MIN:" %8.2lf\t" \
COMMENT:" \\n"



rrdtool graph $dir/www/sensor01_year_small.png \
--title="Sensor01 rok" \
--start $start_year \
--end $end_now \
--vertical-label "°C hPa+1000 10x% Volt" \
--width 500 \
--height 180  \
--rigid \
DEF:t01=$rrd_db:T01:AVERAGE \
DEF:p01=$rrd_db:P01:AVERAGE \
DEF:h01=$rrd_db:H01:AVERAGE \
DEF:v01=$rrd_db:V01:AVERAGE \
COMMENT:" \\n" \
CDEF:p02=p01,1000,- \
CDEF:h02=h01,10,/ \
AREA:t01#A000A0:"Teplota \\t" \
LINE1:p02#FF0000:"Tlak hPa\\t" \
LINE1:h02#0000FF:"Vlhkost %\\t" \
LINE1:v01#00FF00:"Baterie V\\t" \
COMMENT:" \\n" \
GPRINT:t01:LAST:"Teď\\: %7.2lf\t" \
GPRINT:p01:LAST:" %9.2lf\t" \
GPRINT:h01:LAST:" %10.2lf\t" \
GPRINT:v01:LAST:" %8.2lf\n" \
GPRINT:t01:MAX:"Max\\: %7.2lf\t" \
GPRINT:p01:MAX:" %9.2lf\t" \
GPRINT:h01:MAX:" %10.2lf\t" \
GPRINT:v01:MAX:" %8.2lf\n" \
GPRINT:t01:MIN:"Min\\: %7.2lf\t" \
GPRINT:p01:MIN:" %9.2lf\t" \
GPRINT:h01:MIN:" %10.2lf\t" \
GPRINT:v01:MIN:" %8.2lf\t" \
COMMENT:" \\n"




rrdtool graph $dir/www/sensor01_day.png \
--title="Sensor01 den" \
--x-grid HOUR:1:HOUR:3:HOUR:3:0:%H:%M \
--start $start_day \
--end $end_now \
--vertical-label "°C hPa+1000 10x% Volt" \
--width 1000 \
--height 260  \
--rigid \
DEF:t01=$rrd_db:T01:AVERAGE \
DEF:p01=$rrd_db:P01:AVERAGE \
DEF:h01=$rrd_db:H01:AVERAGE \
DEF:v01=$rrd_db:V01:AVERAGE \
COMMENT:" \\n" \
CDEF:p02=p01,1000,- \
CDEF:h02=h01,10,/ \
AREA:t01#A000A0:"Teplota \\t" \
LINE1:p02#FF0000:"Tlak hPa\\t" \
LINE1:h02#0000FF:"Vlhkost %\\t" \
LINE1:v01#00FF00:"Baterie V\\t" \
COMMENT:" \\n" \
GPRINT:t01:LAST:"Teď\\: %7.2lf\t" \
GPRINT:p01:LAST:" %9.2lf\t" \
GPRINT:h01:LAST:" %10.2lf\t" \
GPRINT:v01:LAST:" %8.2lf\n" \
GPRINT:t01:MAX:"Max\\: %7.2lf\t" \
GPRINT:p01:MAX:" %9.2lf\t" \
GPRINT:h01:MAX:" %10.2lf\t" \
GPRINT:v01:MAX:" %8.2lf\n" \
GPRINT:t01:MIN:"Min\\: %7.2lf\t" \
GPRINT:p01:MIN:" %9.2lf\t" \
GPRINT:h01:MIN:" %10.2lf\t" \
GPRINT:v01:MIN:" %8.2lf\t" \
COMMENT:" \\n"



rrdtool graph $dir/www/sensor01_week.png \
--title="Sensor01 týden" \
--x-grid HOUR:6:HOUR:24:HOUR:24:0:" %A" \
--start $start_week \
--end $end_now \
--vertical-label "°C hPa+1000 10x% Volt" \
--width 1000 \
--height 260  \
--rigid \
DEF:t01=$rrd_db:T01:AVERAGE \
DEF:p01=$rrd_db:P01:AVERAGE \
DEF:h01=$rrd_db:H01:AVERAGE \
DEF:v01=$rrd_db:V01:AVERAGE \
COMMENT:" \\n" \
CDEF:p02=p01,1000,- \
CDEF:h02=h01,10,/ \
AREA:t01#A000A0:"Teplota \\t" \
LINE1:p02#FF0000:"Tlak hPa\\t" \
LINE1:h02#0000FF:"Vlhkost %\\t" \
LINE1:v01#00FF00:"Baterie V\\t" \
COMMENT:" \\n" \
GPRINT:t01:LAST:"Teď\\: %7.2lf\t" \
GPRINT:p01:LAST:" %9.2lf\t" \
GPRINT:h01:LAST:" %10.2lf\t" \
GPRINT:v01:LAST:" %8.2lf\n" \
GPRINT:t01:MAX:"Max\\: %7.2lf\t" \
GPRINT:p01:MAX:" %9.2lf\t" \
GPRINT:h01:MAX:" %10.2lf\t" \
GPRINT:v01:MAX:" %8.2lf\n" \
GPRINT:t01:MIN:"Min\\: %7.2lf\t" \
GPRINT:p01:MIN:" %9.2lf\t" \
GPRINT:h01:MIN:" %10.2lf\t" \
GPRINT:v01:MIN:" %8.2lf\t" \
COMMENT:" \\n"


rrdtool graph $dir/www/sensor01_monht.png \
--title="Sensor01 měsíc" \
--x-grid DAY:1:WEEK:1:WEEK:1:0:"Týden %W" \
--start $start_month \
--end $end_now \
--vertical-label "°C hPa+1000 10x% Volt" \
--width 1000 \
--height 260  \
--rigid \
DEF:t01=$rrd_db:T01:AVERAGE \
DEF:p01=$rrd_db:P01:AVERAGE \
DEF:h01=$rrd_db:H01:AVERAGE \
DEF:v01=$rrd_db:V01:AVERAGE \
COMMENT:" \\n" \
CDEF:p02=p01,1000,- \
CDEF:h02=h01,10,/ \
AREA:t01#A000A0:"Teplota \\t" \
LINE1:p02#FF0000:"Tlak hPa\\t" \
LINE1:h02#0000FF:"Vlhkost %\\t" \
LINE1:v01#00FF00:"Baterie V\\t" \
COMMENT:" \\n" \
GPRINT:t01:LAST:"Teď\\: %7.2lf\t" \
GPRINT:p01:LAST:" %9.2lf\t" \
GPRINT:h01:LAST:" %10.2lf\t" \
GPRINT:v01:LAST:" %8.2lf\n" \
GPRINT:t01:MAX:"Max\\: %7.2lf\t" \
GPRINT:p01:MAX:" %9.2lf\t" \
GPRINT:h01:MAX:" %10.2lf\t" \
GPRINT:v01:MAX:" %8.2lf\n" \
GPRINT:t01:MIN:"Min\\: %7.2lf\t" \
GPRINT:p01:MIN:" %9.2lf\t" \
GPRINT:h01:MIN:" %10.2lf\t" \
GPRINT:v01:MIN:" %8.2lf\t" \
COMMENT:" \\n"


rrdtool graph $dir/www/sensor01_year.png \
--title="Sensor01 rok" \
--start $start_year \
--end $end_now \
--vertical-label "°C hPa+1000 10x% Volt" \
--width 1000 \
--height 260  \
--rigid \
DEF:t01=$rrd_db:T01:AVERAGE \
DEF:p01=$rrd_db:P01:AVERAGE \
DEF:h01=$rrd_db:H01:AVERAGE \
DEF:v01=$rrd_db:V01:AVERAGE \
COMMENT:" \\n" \
CDEF:p02=p01,1000,- \
CDEF:h02=h01,10,/ \
AREA:t01#A000A0:"Teplota \\t" \
LINE1:p02#FF0000:"Tlak hPa\\t" \
LINE1:h02#0000FF:"Vlhkost %\\t" \
LINE1:v01#00FF00:"Baterie V\\t" \
COMMENT:" \\n" \
GPRINT:t01:LAST:"Teď\\: %7.2lf\t" \
GPRINT:p01:LAST:" %9.2lf\t" \
GPRINT:h01:LAST:" %10.2lf\t" \
GPRINT:v01:LAST:" %8.2lf\n" \
GPRINT:t01:MAX:"Max\\: %7.2lf\t" \
GPRINT:p01:MAX:" %9.2lf\t" \
GPRINT:h01:MAX:" %10.2lf\t" \
GPRINT:v01:MAX:" %8.2lf\n" \
GPRINT:t01:MIN:"Min\\: %7.2lf\t" \
GPRINT:p01:MIN:" %9.2lf\t" \
GPRINT:h01:MIN:" %10.2lf\t" \
GPRINT:v01:MIN:" %8.2lf\t" \
COMMENT:" \\n"


