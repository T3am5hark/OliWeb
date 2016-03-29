LAT="$($UTIL/GetProperty Lat $QUERY_STRING)"
LONG="$($UTIL/GetProperty Long $QUERY_STRING)"
TIMEMS="$($UTIL/GetProperty Time $QUERY_STRING)"
TAG="$($UTIL/GetProperty Tag $QUERY_STRING)"

DATESTR="$(date +%Y_%m_%d)"

FILENAME="$WEB/tracker/$DATESTR.$TAG.dat"

echo "Lat:    $LAT<br>"
echo "Long:   $LONG<br>"
echo "TimeMs: $TIMEMS<br>"
#echo "Tag:    $TAG<br>"
#echo "$FILENAME<br>"

echo "$TIMEMS, $LAT, $LONG" >> $FILENAME
