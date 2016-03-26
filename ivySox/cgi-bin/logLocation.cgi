LAT="$($UTIL/GetProperty Lat $QUERY_STRING)"
LONG="$($UTIL/GetProperty Long $QUERY_STRING)"

DATESTR="$(date +%Y_%m_%d )"
FILENAME="$WEB/tracker/$DATESTR.dat"

echo "Lat=$LAT"
echo "Long=$LONG"
echo $DATESTR

echo "$LAT, $LONG" >> $FILENAME
