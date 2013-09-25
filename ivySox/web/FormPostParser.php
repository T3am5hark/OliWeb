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
echo "Parsing request BODY: <br><br>\n";
echo "BODY=<br>";
echo "[";
echo $_SERVER['BODY'];
echo "]<br>";
parse_str($_SERVER['BODY']);
echo "parameter1=";
echo $parameter1;
echo "<br>\n";
echo "parameter2=";
echo $parameter2;
echo "<br>\n";
echo "parameter3=";
echo $parameter3;
echo "<br>\n";
echo "TextBox contents: <br>\n";
echo "{<br>\n";
echo $TextBox;
echo "\n<br>}";
echo "<hr>\n";

//echo "TextBox via PHP post: <br>\n";
//echo "{<br>\n";
//echo $_POST["TextBox"];
//echo "\n<br>}";
//echo "<hr>\n";

?>
</h3>
</p>
</HTML>
