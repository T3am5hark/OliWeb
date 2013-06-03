echo "Test of environment variable passing through CGI:"
echo "QUERY_STRING:   " $QUERY_STRING
echo "REQUEST_METHOD: " $REQUEST_METHOD
echo "WEB_ROOT:       " $WEB_ROOT
echo "CGI_BIN:        " $CGI_BIN
echo "......"

PARAMETER1="$(util/GetProperty parameter1 $QUERY_STRING)"
PARAMETER2="$(util/GetProperty parameter2 $QUERY_STRING)"
PARAMETER3="$(util/GetProperty parameter3 $QUERY_STRING)"

echo "parameter1 = " $PARAMETER1
echo "parameter2 = " $PARAMETER2
echo "parameter3 = " $PARAMETER3

echo "......"

echo "Command Line Args:"

for F in $*
do
  echo $F
done
