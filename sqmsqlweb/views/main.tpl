<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="refresh" content="60; URL=/main">
  <link rel="stylesheet" href="static/my.css" >
</head>

<title>SQM Web Manager</title>
</head>

<body bgcolor='#26262A'>
<div class='t'>
<table width='100%%'><tr><td><b><font size='5'>
Sky Quality Meter web manager</font></b></td><td align='right'><b>TSL2591 and BME280 sensors</b>
</td></tr>
</table>
</div >
<div class='b'>
<br />
<div style='width: 40em;'>
&nbsp;&nbsp;Time:<font class='c'><span id='datetime'></span></font> 
<script> 
function pad(num, size) { var s = '000000000' + num; return s.substr(s.length-size); }
var now = new Date(); 
          document.getElementById('datetime').innerHTML = (now.getMonth()+1)+'/'
          +pad(now.getDate().toString(),2)+'/'
          +pad(now.getFullYear().toString().substr(-2),2)+ ' '
          +pad(now.getHours().toString(),2)+':'
          +pad(now.getMinutes().toString(),2)+':'
          +pad(now.getSeconds().toString(),2); 
</script>
<br />
<br />
<b>SQM</>
<br />
<table width='100%%'>
<tr>
<td>
<div style='width:17em;'>
<br />
&nbsp;&nbsp;Sky quality: <font class='c'>{{mpsas}}&#xb1;{{dmpsas}} </font> Mag/ArcSec&#xb2;<br />
&nbsp;&nbsp;Temperature: <font class='c'>{{temperature}}</font>&deg;C<br />
&nbsp;&nbsp;Barometric Pressure: <font class='c'>{{pressure}}</font>hPa<br />
&nbsp;&nbsp;Relative Humidity: <font class='c'>{{humidity}}</font>%<br />
&nbsp;&nbsp;Dew Point Temperature: <font class='c'>{{dewpoint}} </font>&deg;C<br />
&nbsp;&nbsp;Count: <font class='c'>{{count}}</font><br />
</div>
<td>
<a href="/longtimegraph"><img src=static/sqm.png></a>
<tr>
<td>&nbsp;<td>
<form action="/main" method="POST">
<input type="hidden" name="id" value="graph">
<table>
<tr>
<td>1 hours: 
%if start_time == '-1h':
 <input type="radio"  name="graph" value="-1h" checked >
%else:
 <input type="radio"  name="graph" value="-1h">
%end
<td>3 hours:
%if start_time == '-3h':
 <input type="radio"  name="graph" value="-3h" checked>
%else: 
 <input type="radio"  name="graph" value="-3h">
%end
<td>8 hours:
%if start_time == '-8h':  
  <input type="radio"  name="graph" value="-8h" checked>
%else:
  <input type="radio"  name="graph" value="-8h">
%end
<td>Long time:
 <input type="radio"  name="graph" value="long">
<tr>
</table>
 <button type="submit">Change graph</button>
</form>
</table>
<br />
<br />
<!--
<b>RAW Data</b>
<br />
<br />
&nbsp;&nbsp;Full:<font class='c'>{{full}}</font><br />
&nbsp;&nbsp;Vis: <font class='c'>{{vis}}</font><br />
&nbsp;&nbsp;Ir : <font class='c'>{{ir}}</font><br />

<br />
-->

<form action='/main' method='POST'>
<input type="hidden" name="id" value="oled">
&nbsp;&nbsp; OLED Display is:
%if oled == '1':
  <input type="hidden"  name="sled" value=0>
  <font class='c'>ON &nbsp; </font> &nbsp; <input type="submit" value="OFF">
%else:
  <input type="hidden"  name="sled" value=1>
  <font class='c'>OFF</font> &nbsp; <input type="submit" value="ON">
%end
</form>

</div>
<br />
<a href='/main'style='background-color: #552222;'>Home</a>
<a href='/info'>Box Info</a>
<a href='/config' >Configuration</a>
</body>
</html>
