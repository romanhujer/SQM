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
<b>Select Serial Port</b><br />
<br />
<table>
<tr>
<form action="/init" method="POST">
<td>&nbsp;&nbsp; 
 <input type="hidden" name="id" value="com">
<td> 
 <input type="text" style="width:100%;" name="scom" size="12" value="{{com}}">
<td> 
  <input type="submit" value="Set">
</form>
</tr>
</table>
</body>
</html>
