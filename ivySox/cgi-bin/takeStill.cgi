# Select appropriate still capture here

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
#raspistill -vf -hf -w 640 -h 480 -n -q 75 -t 50 -e jpg -o $WEB/snapshot/snapshot.jpeg
#raspistill -vf -hf -w 1920 -h 1080 -n -q 75 -t 50 -o $WEB/snapshot/snapshot.jpeg
