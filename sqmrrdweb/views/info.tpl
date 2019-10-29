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
<form action="/info" method="POST">
<input type="hidden"  name="id" value="oled">
&nbsp;&nbsp; OLED Display is:&nbsp;
%if oled == 'On':
  <input type="hidden"  name="sled" value=0>
  <font class='c'>ON &nbsp;&nbsp;</font> &nbsp; <input type="submit" value="OFF">
%else:
  <input type="hidden"  name="sled" value=1>
  <font class='c'>OFF</font> &nbsp;&nbsp; <input type="submit" value="ON">
%end
</form>
<form action="/info" method="POST">
<input type="hidden"  name="id" value="dimmer">
&nbsp;&nbsp; Auto dimmer is &nbsp; :&nbsp;
%if dimmer == 'On':
  <input type="hidden"  name="sdimmer" value=0>
  <font class='c'>ON &nbsp;&nbsp;</font> &nbsp; <input type="submit" value="OFF">
%else:
  <input type="hidden"  name="sdimmer" value=1>
  <font class='c'>OFF</font>  &nbsp;&nbsp; <input type="submit" value="ON">
%end
</form>
<form action="/info" method="POST">
<input type="hidden"  name="id" value="contras">
&nbsp;&nbsp;Display contras : 
<font class='c'><input type="number" min=0 max=255  name="scontras" value="{{contras}}"</input></font>
<input type="submit" value="Set">
</form >
<br />
</div>
<br />
<a href='/main'>Home</a>
<a href='/info' style='background-color: #552222;'>Box Info</a>
<a href='/config'>Configuration</a>
</body>
</html>
