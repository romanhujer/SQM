<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="refresh" content="10; URL=/">
<STYLE>
.clear { clear: both; } 
.a { background-color: #111111; } 
.t { padding: 10px 10px 20px 10px; border: 5px solid #551111; margin: 25px 25px 0px 25px; color: #999999; background-color: #111111; min-width: 30em; } 
input { font-weight: bold; width:4em; background-color: #A01010; padding: 2px 2px; }
.b { padding: 10px; border-left: 5px solid #551111; border-right: 5px solid #551111; border-bottom: 5px solid #551111; margin: 0px 25px 25px 25px; color: #999999;background-color: #111111; min-width: 30em; } 
select { width:4em; font-weight: bold; background-color: #A01010; padding: 2px 2px; } 
  .c { color: #A01010; font-weight: bold; }
h1 { text-align: right; } 
a:hover, 
a:active { background-color: red; } 
 .y { color: #FFFF00; font-weight: bold; }
a:link, 
a:visited { background-color: #332222; color: #a07070; border:1px solid red; padding: 5px 10px; margin: none; text-align: center; text-decoration: none; display: inline-block; }
button { background-color: #A01010; font-weight: bold; border-radius: 5px; margin: 2px; padding: 4px 8px; }
 .b1 { float: left; border: 2px solid #551111; background-color: #181818; text-align: center; margin: 5px; padding: 15px; padding-top: 3px; }
 .gb {  font-weight: bold; font-size: 150%; font-family: 'Times New Roman', Times, serif; width: 60px; height: 50px; padding: 0px; }
 .bb {  font-weight: bold; font-size: 105%; } 
 .bbh {  font-weight: bold; font-size: 100%; height: 2.1em; }
</STYLE></head>

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
&nbsp;&nbsp;SQM: <font class='c'>{{mpsas}}&#xb1;{{dmpsas}} </font> Mag/Arc&#xb2;Sec<br />
&nbsp;&nbsp;Temperature: <font class='c'>{{temperature}}</font>&deg;C<br />
&nbsp;&nbsp;Barometric Pressure: <font class='c'>{{pressure}}</font>hPa<br />
&nbsp;&nbsp;Relative Humidity: <font class='c'>{{humidity}}</font>%<br />
&nbsp;&nbsp;Dew Point Temperature: <font class='c'>{{devpoint}} </font>&deg;C<br />
&nbsp;&nbsp;Count: <font class='c'>{{count}}</font><br />
<br />
</div>
<br />
<a href='/'style='background-color: #552222; '>Home</a>
<a href='/info'>Box Info</a>
<a href='/config' > Configuration</a>
</body>
</html>
