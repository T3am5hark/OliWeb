cgi-bin/htmlStart.cgi

echo '<H3><font face="arial" color="C8A055">'
echo 'Taking screenshot...<br>'

fswebcam -r 640x472 -d /dev/video0 -v web/snapshot/snapshot.jpeg 
#fswebcam -r 320x240 -d /dev/video1 -v web/snapshot/snapshot.jpeg
#mplayer -vo jpeg -frames 1 driver=v4l2:width=320:height=200:device=/dev/video0 tv:// 
#mplayer -vo png driver=v4l2:width=320:height=200:device=/dev/video0 -frames 1 tv:// 
#mplayer -vo png driver=v4l2:width=320:height=200:device=/dev/video0 -fps 15 -frames 1 tv:// 
#vlc -I dummy v4l2:///dev/video0 --video-filter scene --no-audio --scene-path . --scene-prefix snapshot --scene-format png vlc://quit --run-time=1

DATESTAMP="$(date +%Y-%m-%d_%H_%M_%S)"
cp web/snapshot/snapshot.jpeg "web/snapshot/$DATESTAMP.jpeg"

echo '<img src="snapshot/snapshot.jpeg" class="img-responsive" alt="Responsive image"/><br>'
echo '<a href="lsSnapshot.cgi">show snapshot directory</a><br>'
echo '<a href="snapshot.cgi">[re-take]</a><br>'
echo '<H5>'
echo '<a href="steerAndSnap.cgi?position=5&pulses=100">[LEFT]</a>'
echo '<a href="steerAndSnap.cgi?position=5&pulses=5">[&lt;&lt;]</a>'
echo '<a href="steerAndSnap.cgi?position=5&pulses=1">[&lt;]</a>'
echo '<a href="steerAndSnap.cgi?position=15&pulses=50">[CENTER]</a>'
echo '<a href="steerAndSnap.cgi?position=25&pulses=1">[&gt;]</a>'
echo '<a href="steerAndSnap.cgi?position=25&pulses=5">[&gt;&gt;]</a>'
echo '<a href="steerAndSnap.cgi?position=25&pulses=100">[RIGHT]</a>'
echo '</H5>'
echo '<br>'
echo '<a href="cgiScripts.html">scripts</a><br>'
echo '<a href="index.html">back to main</a><br>'


echo '</font></H3>'
cgi-bin/htmlEnd.cgi
