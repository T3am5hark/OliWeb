$CGI_BIN/htmlStart.cgi

echo '<H4><font face="arial" color="C8A055">'

#fswebcam -r 160x90 -d /dev/video0 -v $WEB/snapshot/snapshot.jpeg 
#fswebcam -r 320x200 -d /dev/video0 -v $WEB/snapshot/snapshot.jpeg 
fswebcam -r 640x480 -d /dev/video0 -v --jpeg=90 $WEB/snapshot/snapshot.jpeg 
#fswebcam -r 640x472 -d /dev/video0 -v $WEB/snapshot/snapshot.jpeg 
#fswebcam -r 640x472 -d /dev/video0 -v $WEB/snapshot/snapshot.jpeg 
#fswebcam -r 640x472 -d /dev/video0 -v --jpeg=90 $WEB/snapshot/snapshot.jpeg 
#fswebcam -r 320x240 -d /dev/video1 -v $WEB/snapshot/snapshot.jpeg
#mplayer -vo jpeg -frames 1 driver=v4l2:width=320:height=200:device=/dev/video0 tv:// 
#mplayer -vo png driver=v4l2:width=320:height=200:device=/dev/video0 -frames 1 tv:// 
#mplayer -vo png driver=v4l2:width=320:height=200:device=/dev/video0 -fps 15 -frames 1 tv:// 
#vlc -I dummy v4l2:///dev/video0 --video-filter scene --no-audio --scene-path . --scene-prefix snapshot --scene-format png vlc://quit --run-time=1

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
