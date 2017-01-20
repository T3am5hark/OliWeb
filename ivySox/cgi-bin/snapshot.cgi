$CGI_BIN/htmlStart.cgi

echo '<H4><font face="arial" color="C8A055">'

$CGI_BIN/takeStill.cgi

echo '<center>'
date
echo '<br>'
echo '<img src="snapshot/snapshot.jpeg" class="img-responsive" alt="Responsive image"/>'

echo '<div class="btn-group btn-group-lg" role="group" aria-label="...">'
#echo '<a href="steerAndSnap.cgi?position=5&pulses=50" class="btn btn-primary">Left</a>'
echo '<a href="steerAndSnap.cgi?position=5&pulses=5" class="btn btn-primary">&lt;&lt;</a>'
echo '<a href="steerAndSnap.cgi?position=5&pulses=2" class="btn btn-primary">&lt;</a>'
echo '<a href="steerAndSnap.cgi?position=15&pulses=30" class="btn btn-primary">Ctr</a>'
echo '<a href="steerAndSnap.cgi?position=25&pulses=2" class="btn btn-primary">&gt;</a>'
echo '<a href="steerAndSnap.cgi?position=25&pulses=5" class="btn btn-primary">&gt;&gt;</a>'
#echo '<a href=steerAndSnap.cgi?position=25&pulses=50" class="btn btn-primary">Right</a>'
echo '</div><br><br>'

#echo '<div class="btn-group btn-group-lg" role="group" aria-label="...">'
echo '<div class="container">' 
echo '<a href="snapshot.cgi" class="btn-lg btn-success">Retake</a>'
#echo '<a class="btn btn-default">...</a>'
echo '<a href="archiveSnapshot.cgi" class="btn-lg btn-warning">Archive</a>'
echo '</div><br>'

#echo '<br>'
echo '<a href="lsSnapshot.cgi">show snapshot directory</a>|'
echo '<a href="cgiScripts.html">scripts</a>|'
echo '<a href="index.html">back to main</a><br>'

echo '</center>'

echo '</font></H4>'
$CGI_BIN/htmlEnd.cgi
