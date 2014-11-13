echo "Test of environment variable passing through CGI:"
echo "QUERY_STRING:   " $QUERY_STRING
echo "REQUEST_METHOD: " $REQUEST_METHOD
echo "WEB_ROOT:       " $WEB_ROOT
echo "CGI_BIN:        " $CGI_BIN
echo "UTIL:           " $UTIL
echo "......"

PARAMETER1="$($UTIL/GetProperty parameter1 $QUERY_STRING)"
PARAMETER2="$($UTIL/GetProperty parameter2 $QUERY_STRING)"
PARAMETER3="$($UTIL/GetProperty parameter3 $QUERY_STRING)"

echo "parameter1 = " $PARAMETER1
echo "parameter2 = " $PARAMETER2
echo "parameter3 = " $PARAMETER3

echo "......"

echo "Command Line Args:"

for F in $*
do
  echo $F
done

