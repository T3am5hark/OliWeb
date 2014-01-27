
NLINES="$(util/GetProperty N $QUERY_STRING)"

if [ "$NLINES" = "" ]; then
  NLINES=150
fi


echo "ShowWebLog.cgi invoked"
echo "[Showing most recent $NLINES lines]"
tail -n $NLINES OliWeb.log
