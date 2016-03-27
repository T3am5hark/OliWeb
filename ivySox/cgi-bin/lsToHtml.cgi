$CGI_BIN/htmlStart.cgi

DIR="$($UTIL/GetProperty dir $QUERY_STRING)"
EXT="$($UTIL/GetProperty ext $QUERY_STRING)"
FILE_LOCATION=$WEB/$DIR

echo '<H2>'
echo "Contents of $DIR:<br><br>"
echo '</H2>'

echo '<H3>'
cd $FILE_LOCATION
#ALLFILES=snapshot/*.jpeg
#ALLFILES="$FILE_LOCATION/*.$EXT"
#ALLFILESSORTED=$(ls -rt *.jpeg)
ALLFILES="*.$EXT"

for F in $ALLFILES;
do
  echo '<a href="'$DIR/$F'">'$F'</a><br>'
done

echo '<br><br>'
echo '<a href="cgiScripts.html">[Scripts]</a><br>'
echo '<a href="index.html">[Main]</a><br>'

echo '</H3>'
$CGI_BIN/htmlEnd.cgi
