DATESTAMP="$(date +%Y-%m-%d_%H_%M_%S)"
cp $WEB/snapshot/snapshot.jpeg "$WEB/snapshot/$DATESTAMP.jpeg"

$CGI_BIN/redirect.cgi snapshot.cgi
