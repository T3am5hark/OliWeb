echo "Test of environment variable passing through CGI:"
echo "QUERY_STRING:   " $QUERY_STRING
echo "REQUEST_METHOD: " $REQUEST_METHOD
echo "WEB_ROOT:       " $WEB_ROOT
echo "CGI_BIN:        " $CGI_BIN
echo "BODY:           " $BODY
echo "UTIL:           " $UTIL
echo "......"

TEENSY="/dev/ttyACM0"
echo "TEENSY:         " $TEENSY
echo "......"

LINE1="$($UTIL/GetProperty line1 $QUERY_STRING)"
LINE2="$($UTIL/GetProperty line2 $QUERY_STRING)"

echo "line1 = " $LINE1
echo "line2 = " $LINE2

echo -n $LINE1 > $TEENSY
sleep 0.05
echo -n $LINE2 > $TEENSY

