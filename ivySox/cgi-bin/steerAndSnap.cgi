position="$($UTIL/GetProperty position $QUERY_STRING)"
pulses="$($UTIL/GetProperty pulses $QUERY_STRING)"

$CGI_BIN/steer.cgi $pulses $position

$CGI_BIN/snapshot.cgi
