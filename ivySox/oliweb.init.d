#!/bin/sh

### BEGIN INIT INFO
# Provides:        oliweb
# Required-Start:  $network $remote_fs $syslog
# Required-Stop:   $network $remote_fs $syslog
# Default-Start:   2 3 4 5
# Default-Stop:
# Short-Description: OliWeb Server Start
### END INIT INFO

#. /lib/lsb/init-functions

#DAEMON=/usr/sbin/ntpd
#PIDFILE=/var/run/ntpd.pid
OLIWEB="/opt/OliWeb/OliWeb"
CONFIGFILE="/opt/OliWeb/OliWebConfig.xml"

#test -x $OLIWEB || exit 5

#RUNASUSER=oliweb
#UGID=$(getent passwd $RUNASUSER | cut -f 3,4 -d:) || true
#if test "$(uname -s)" = "Linux"; then
#        NTPD_OPTS="$NTPD_OPTS -u $UGID"
#fi

case $1 in
	start)
		echo "Starting OliWeb server"
		echo "$OLIWEB $CONFIGFILE &"
		#log_daemon_msg "Starting OliWeb server" "oliweb"
		#tmp="$($OLIWEB $CONFIGFILE &)"
                /usr/local/bin/OliWeb /usr/local/bin/OliWebConfig.xml &
		#if [ -z "$UGID" ]; then
		#	log_failure_msg "user \"$RUNASUSER\" does not exist"
		#	exit 1
		#fi
		#lock_ntpdate
  		#start-stop-daemon --start --quiet --oknodo --pidfile $PIDFILE --startas $DAEMON -- -p $PIDFILE $NTPD_OPTS
		status=$?
		#unlock_ntpdate
		#log_end_msg $status
		exit 0
  		;;
	stop)
		#log_daemon_msg "OliWeb stop doesn't work right now." "oliweb"
  		#start-stop-daemon --stop --quiet --oknodo --pidfile $PIDFILE
		#log_end_msg $?
		#rm -f $PIDFILE
  		;;
	restart|force-reload)
		#$0 stop && sleep 2 && $0 start
  		;;
	try-restart)
		#if $0 status >/dev/null; then
		#	$0 restart
		#else
		#	exit 0
		#fi
		;;
	reload)
		exit 3
		;;
	status)
		#status_of_proc $DAEMON "OliWeb server"
		;;
	*)
		echo "Usage: $0 {start|stop|restart|try-restart|force-reload|status}"
		exit 2
		;;
esac
