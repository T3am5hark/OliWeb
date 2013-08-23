<!DOCTYPE html>
<HTML>
<p>
<h1>
<?php echo "Here is some PHP!<br><hr>\n" ?>
<h3>
<?php
echo "Parsing URL QUERY_STRING arguments: <br><br>\n";
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
</h1>
</p>
</HTML>
