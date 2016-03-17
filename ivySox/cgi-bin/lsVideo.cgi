$CGI_BIN/htmlStart.cgi

echo 'Contents of capture directory:<br><br>'

ALLFILES=$WEB/video/*.*

for F in $ALLFILES;
do
  echo '<a href="'$F'">'$F'</a><br>'
done

echo '<br><br>'
echo '<a href="snapshot.cgi">[Take Snapshot]</a>'	

echo '</html>'
