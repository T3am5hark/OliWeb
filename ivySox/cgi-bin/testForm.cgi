echo "Test of environment variable passing through CGI:"
echo "QUERY_STRING:   " $QUERY_STRING
echo "REQUEST_METHOD: " $REQUEST_METHOD
echo "WEB_ROOT:       " $WEB_ROOT
echo "CGI_BIN:        " $CGI_BIN
echo "BODY:           " $BODY
echo "UTIL:           " $UTIL
echo "......"

PARAMETER1="$($UTIL/GetProperty parameter1 $BODY)"
PARAMETER2="$($UTIL/GetProperty parameter2 $BODY)"
PARAMETER3="$($UTIL/GetProperty parameter3 $BODY)"
TEXTBOX="$($UTIL/GetProperty TextBox $BODY)"

echo "parameter1 = " $PARAMETER1
echo "parameter2 = " $PARAMETER2
echo "parameter3 = " $PARAMETER3
echo "TextBox = "
echo "{" 
echo $TEXTBOX
echo "}"

