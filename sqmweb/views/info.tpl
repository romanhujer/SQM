<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
<!--  <meta http-equiv="refresh" content="10; URL=/"> -->
  <link rel="stylesheet" href="static/my.css" >
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
<div style='width: 40em;'>
<b>BOX INFO</b><br />
<br />
&nbsp;&nbsp;Serial &nbsp; : <font class='c'>{{serial}}</font><br />
&nbsp;&nbsp;Protokol: <font class='c'>{{protokol}}</font><br />
&nbsp;&nbsp;Model &nbsp;&nbsp;: <font class='c'>{{model}}</font><br />
&nbsp;&nbsp;Feature : <font class='c'>{{feature}}</font><br />
<br />
<br />
<b>CALIBRATION</b><br />
<br />
&nbsp;&nbsp;Temperature calibration : <font class='c'>{{tc}}</font><br />
&nbsp;&nbsp;Sky quality offset : <font class='c'>{{moffset}}</font> Mag/Arc&#xb2;Sec<br />
&nbsp;&nbsp;Temperature offset : <font class='c'>{{toffset}}</font>&deg;C<br />
<br />
<br />
<b>OLED DISPLAY</b><br />
<br />
&nbsp;&nbsp;Oled display is : <font class='c'>{{oled}}</font><br />
&nbsp;&nbsp;Auto dimmer is : <font class='c'>{{dimmer}}</font><br />
&nbsp;&nbsp;Default contras : <font class='c'>{{contras}}</font><br />
 
<br />
</div>
<br />
<a href='/'>Home</a>
<a href='/info' style='background-color: #552222;'>Box Info</a>
<a href='/config'>Configuration</a>
</body>
</html>
