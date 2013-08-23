<!DOCTYPE html>
<HTML>

<head>
    <title>Raspberry Pi!!!</title>
    <meta name="viewport" content="width=device-width initial-scale=1 " />
</head>

<p>
<h2>
<img src="rhino32.png" />
<?php echo "Here's some PHP!\n" ?>
<img src="rhino32.png" />
<br><hr>
</h2>

<h3>
<?php
echo "Parsing QUERY_STRING: <br><br>\n";
parse_str($_SERVER['QUERY_STRING']);
//echo "Query String =";
//echo $_SERVER['QUERY_STRING'];
//echo "<br><br>\n";
echo "parameter1=";
echo $parameter1;
echo "<br>\n";
echo "parameter2=";
echo $parameter2;
echo "<br>\n";
echo "parameter3=";
echo $parameter3;
echo "<br><hr>\n";

?>
</h3>
</p>
</HTML>
