position="$(util/GetProperty position $QUERY_STRING)"
pulses="$(util/GetProperty pulses $QUERY_STRING)"

cgi-bin/steer.cgi $pulses $position

cgi-bin/snapshot.cgi
