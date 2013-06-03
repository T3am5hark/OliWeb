cgi-bin/htmlStart.cgi

echo 'Contents of snapshot directory:</br></br>'

cd web
ALLFILES=snapshot/*.jpeg

for F in $ALLFILES;
do
  echo '<a href="'$F'">'$F'</a></br>'
done

echo '</br></br>'
echo '<a href="snapshot.cgi">[Take Snapshot]</a>'	


cgi-bin/htmlEnd.cgi
