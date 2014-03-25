cgi-bin/htmlStart.cgi

echo 'Contents of capture directory:<br><br>'

cd web
ALLFILES=video/*.*

for F in $ALLFILES;
do
  echo '<a href="'$F'">'$F'</a><br>'
done

echo '<br><br>'
echo '<a href="snapshot.cgi">[Take Snapshot]</a>'	


#cgi-bin/htmlEnd.cgi
echo '</html>'
