<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="refresh" content="10; URL=/">
  <link rel="stylesheet" href="static/main.css" >
</head>

<title>SQM Web Manager</title>
</head>

<body bgcolor='#26262A'>
<div class='t'>
<table width='100%%'><tr><td><b><font size='5'>
Sky Quality Meter web manager</font></b></td><td align='right'>
<b>TSL2591 and BME280 sensors</b>
</td></tr>
</table>
</div >
<div class='b'>
<br />
<div style='width: 40em;'>
&nbsp;&nbsp;Time:<span id='datetime'></span> UT (web browser) 
<script> 
function pad(num, size) { var s = '000000000' + num; return s.substr(s.length-size); }
var now = new Date(); 
          document.getElementById('datetime').innerHTML = (now.getUTCMonth()+1)+'/'
          +pad(now.getUTCDate().toString(),2)+'/'
          +pad(now.getUTCFullYear().toString().substr(-2),2)+ ' '
          +pad(now.getUTCHours().toString(),2)+':'
          +pad(now.getUTCMinutes().toString(),2)+':'
          +pad(now.getUTCSeconds().toString(),2); 
</script>
<br /><br />

&nbsp;&nbsp;RAW data: <font class='c'>{{sqm_data}}</font><br />
<br />
&nbsp;&nbsp;SQM: <font class='c'>10.0</font> Mag/Arc-Sec<br />
&nbsp;&nbsp;Temperature: <font class='c'>10.0</font>&deg;C<br />
&nbsp;&nbsp;Barometric Pressure: <font class='c'>1010.0</font>hPa<br />
&nbsp;&nbsp;Relative Humidity: <font class='c'>70.0</font>%<br />
&nbsp;&nbsp;Dew Point Temperature: <font class='c'>4.0</font>&deg;C<br />
<br />
</div>
<br />
<a href='/'style='background-color: #552222; '>Home</a>
<a href='/info'>Box Info</a>
<a href='/config' > Configuration</a>
</body>
</html>
