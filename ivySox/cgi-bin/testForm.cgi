echo "Test of environment variable passing through CGI:"
echo "QUERY_STRING:   " $QUERY_STRING
echo "REQUEST_METHOD: " $REQUEST_METHOD
echo "WEB_ROOT:       " $WEB_ROOT
echo "CGI_BIN:        " $CGI_BIN
echo "BODY:           " $BODY
echo "......"

PARAMETER1="$(util/GetProperty parameter1 $BODY)"
PARAMETER2="$(util/GetProperty parameter2 $BODY)"
PARAMETER3="$(util/GetProperty parameter3 $BODY)"
TEXTBOX="$(util/GetProperty TextBox $BODY)"

echo "parameter1 = " $PARAMETER1
echo "parameter2 = " $PARAMETER2
echo "parameter3 = " $PARAMETER3
echo "TextBox = "
echo "{" 
echo $TEXTBOX
echo "}"

