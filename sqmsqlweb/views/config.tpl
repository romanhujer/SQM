<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta http-equiv="refresh" content="600; URL=/main">
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
<b>BOX CALIBRATION</b><br />
<br />
<table>
<tr> 
<td>&nbsp;&nbsp;Temperature calibration : <font class='c'>{{tc}}</font>
<td>
<form action="/config" method="POST">
<input type="hidden"  name="id" value="tc">
%if tc == 'Yes':
 <input type="hidden"  name="stc" value=0> 
 <input type="submit" value="No">
%else:
 <input type="hidden"  name="stc" value=1>
 <input type="submit"  value="Yes">
%end 
</form>
<tr>
<td> &nbsp;&nbsp;Sky quality offset : <font class='c'>{{moffset_s}}</font> Mag/ArcSec&#xb2;&nbsp;
<td>
<form action="/config" method="POST">
 <input type="hidden"  name="id" value="moffset">
 <input type="number" step="0.01" min=-10 max=10  name="smoffset" value="{{moffset}}"> </input>
 <input type="submit" value="Set">
</form>
<tr>
<td>  &nbsp;&nbsp;Temperature offset : <font class='c'>{{toffset_s}}</font>&deg;C&nbsp;
<td>
<form action="/config" method="POST">
<input type="hidden"  name="id" value="toffset">
  <input type="number" step="0.1" min=-10 max=10  name="stoffset" value="{{toffset}}"> </input>
  <input type="submit" value="Set">
</form> 
</table>
<br />
<br />
<b>BOX FACTORY RESET</b><br />
<br />
<form action="/config" method="POST">
&nbsp;&nbsp;Type 4826157 : 
  <input type="hidden"  name="id" value="reset">
  <font class='c'><input type="number" style="width:100px;" size"7" name="sreset" value=""</input></font>
  <input type="submit" value="Reset">
</form >
<br />
<b>CLEAR DATABASE</b><br />
<br />
<form action="/config" method="POST">
&nbsp;&nbsp;Type 2165394 :
  <input type="hidden"  name="id" value="clear">
  <font class='c'><input type="number" style="width:100px;" size"7" name="sclear" value=""</input></font>
  <input type="submit" value="Clear">
</form >
<br />

</div>
<br />
<a href='/main'>Home</a>
<a href='/info'>Box Info</a>
<a href='/config' style='background-color: #552222;'>Configuration</a>
<a href='/init'>Init Page</a>
</body>
</html>
