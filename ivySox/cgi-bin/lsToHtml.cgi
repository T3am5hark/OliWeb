DIR="$($UTIL/GetProperty dir $QUERY_STRING)"
EXT="$($UTIL/GetProperty ext $QUERY_STRING)"
noHtml="$($UTIL/GetProperty noHtml $QUERY_STRING)"
FILE_LOCATION=$WEB/$DIR
ALLFILES="*.$EXT"

cd $FILE_LOCATION

if [ $noHtml -eq 1 ]
then
for F in $ALLFILES;
do
  echo $F
done
else
  $CGI_BIN/htmlStart.cgi
  echo '<H2>'
  echo "noHtml=$noHtml<br>"
  echo "Contents of $DIR:<br><br>"
  echo '</H2>'
  echo '<H3>'

for F in $ALLFILES;
do
  echo '<a href="'$DIR/$F'">'$F'</a><br>'
done

echo '<br><br>'
echo '<a href="cgiScripts.html">[Scripts]</a><br>'
echo '<a href="index.html">[Main]</a><br>'

echo '</H3>'
$CGI_BIN/htmlEnd.cgi

fi
