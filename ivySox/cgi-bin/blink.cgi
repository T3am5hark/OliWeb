
$CGI_BIN/htmlStart.cgi
echo '<H2>'
echo 'Running GPIO blink test<br>'
echo '.......................'
echo '</H2>'
$CGI_BIN/htmlEnd.cgi


$UTIL/init
/bin/bash $UTIL/blinktest &



