position="$(util/GetProperty position $QUERY_STRING)"
pulses="$(util/GetProperty pulses $QUERY_STRING)"

gpioTester 10000 $pulses $position

cgi-bin/snapshot.cgi
