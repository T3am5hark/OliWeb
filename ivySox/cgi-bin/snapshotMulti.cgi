cgi-bin/htmlStart.cgi

echo '<H2><font face="arial" color="C8A055">'
echo 'Taking screenshot...<br>'

fswebcam -r 480x322 -d /dev/video0 -v web/snapshot/snapshotx3.jpeg -F 4 --fps 15

#echo 'Done!<br>'
echo '<img src="snapshot/snapshotx3.jpeg"/><br>'
echo '<a href="snapshotMulti.cgi">[re-take]</a><br>'
echo '<a href="cgiScripts.html">scripts</a><br>'
echo '<a href="index.html">back to main</a><br>'

echo '</font></H2>'
cgi-bin/htmlEnd.cgi
