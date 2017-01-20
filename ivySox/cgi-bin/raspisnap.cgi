$CGI_BIN/htmlStart.cgi

echo '<H4><font face="arial" color="C8A055">'

$CGI_BIN/takeStill.cgi

echo '<center>'
date
echo '<br>'
echo '<img src="snapshot/snapshot.jpeg" class="img-responsive" alt="Responsive image"/><br>'

echo '<H2>'
echo '<div class="btn-group" role="group" aria-label="...">'
echo '<a href="raspiSteer.cgi?pin=20&position=25&pulses=50" class="btn btn-primary">Lf</a>'
echo '<a href="raspiSteer.cgi?pin=20&position=25&pulses=5" class="btn btn-primary">&lt;&lt;</a>'
echo '<a href="raspiSteer.cgi?pin=20&position=25&pulses=1" class="btn btn-primary">&lt;</a>'
echo '<a href="raspiSteer.cgi?pin=20&position=15&pulses=30" class="btn btn-primary">Ctr</a>'
echo '<a href="raspiSteer.cgi?pin=20&position=5&pulses=1" class="btn btn-primary">&gt;</a>'
echo '<a href="raspiSteer.cgi?pin=20&position=5&pulses=5" class="btn btn-primary">&gt;&gt;</a>'
echo '<a href="raspiSteer.cgi?pin=20&position=5&pulses=50" class="btn btn-primary">Rt</a>'
echo '</div><br>'
echo '</H2>'

echo '<div class="btn-group btn-group-lg" role="group" aria-label="...">'
echo '<a href="raspisnap.cgi" class="btn btn-success">Retake</a>'
echo '<a href="archiveRaspi.cgi" class="btn btn-warning">Archive</a>'
echo '</div><br>'

echo '<br>'
echo '<a href="lsSnapshot.cgi">show snapshot directory</a>|'
echo '<a href="cgiScripts.html">scripts</a>|'
echo '<a href="index.html">back to main</a><br>'

echo '</center>'

echo '</font></H4>'
$CGI_BIN/htmlEnd.cgi
