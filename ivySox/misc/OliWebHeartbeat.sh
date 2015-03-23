# OliWeb Heartbeat
# Check to see if OliWeb is running
# Place heartbeat message in /tmp/OliWebHeartbeat.log
# Restart OliWeb if it's not running
#
# To install:
#
#  1.  Move the file to hourly crontab: "sudo cp OliWebHeartbeat.sh /etc/cron.hourly/"
#  2.  Set permissions: "sudo chmod 755 /etc/cron.hourly/OliWebHeartbeat.sh"
#  3.  Add the script to the crontab file for hourly exeuction: "sudo nano /etc/crontab"
#      15 *	* * *	root	/etc/cron.hourly/OliWebHeartbeat.sh
#
#  (As shown will exeucte every hour, 15 mins after the hour)
#  Note: the "cd" command is necessary if running oliweb in relative path mode
#  The Heartbeat file will show hourly status and restart messages where applicable

# Replace this with your OliWeb location
OLIWEBPATH="/mnt/usb/jdm/projects/c/ivySox"

NOW=$(date)
PID=$(ps -C OliWeb | grep OliWeb)
HEARTBEATFILE="/tmp/OliWebHeartbeat.log"

echo $NOW >> $HEARTBEATFILE

if [ "$PID" ]
then
     echo "OliWeb is up - PID=$PID" >> $HEARTBEATFILE
else
     echo "OliWeb is down!  Restarting!" >> $HEARTBEATFILE
     cd $OLIWEBPATH
     ./OliWeb &
     NEWPID=$(ps -C OliWeb | grep OliWeb)
     echo $NEWPID >> $HEARTBEATFILE
fi


