DATESTAMP="$(date +%Y-%m-%d_%H_%M_%S)"
cp web/snapshot/snapshot.jpeg "web/snapshot/$DATESTAMP.jpeg"

cgi-bin/redirect.cgi raspisnap.cgi
