cgi-bin/htmlStart.cgi

echo '<H2><font face="arial" color="C8A055">'
echo 'Taking screenshot...</br>'

fswebcam -r 640x472 -d /dev/video0 -v web/snapshot/snapshot.jpeg 
#fswebcam -r 320x240 -d /dev/video1 -v web/snapshot/snapshot.jpeg
#mplayer -vo jpeg -frames 1 driver=v4l2:width=320:height=200:device=/dev/video0 tv:// 
#mplayer -vo png driver=v4l2:width=320:height=200:device=/dev/video0 -frames 1 tv:// 
#mplayer -vo png driver=v4l2:width=320:height=200:device=/dev/video0 -fps 15 -frames 1 tv:// 
#vlc -I dummy v4l2:///dev/video0 --video-filter scene --no-audio --scene-path . --scene-prefix snapshot --scene-format png vlc://quit --run-time=1

#echo 'Done!</br>'
echo '<img src="snapshot/snapshot.jpeg"/></br>'
echo '<a href="snapshot.cgi">[re-take]</a></br>'
echo '<a href="cgiScripts.html">scripts</a></br>'
echo '<a href="index.html">back to main</a></br>'

echo '</font></H2>'
cgi-bin/htmlEnd.cgi