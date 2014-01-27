cgi-bin/htmlStart.cgi

echo '<H2>'
echo 'Contents of snapshot directory:<br><br>'
echo '</H2>'

echo '<H3>'
cd web/snapshot
#ALLFILES=snapshot/*.jpeg
ALLFILES=*.jpeg

#ALLFILESSORTED=$(ls -rt *.jpeg)

for F in $ALLFILES;
#for F in 'ls -altr *.jpeg'
do
  echo '<a href="snapshot/'$F'">'$F'</a><br>'
done

#for F in $ALLFILESSORTED
#do
#  echo '<a href="'$F'">'$F'</a><br>'
#done

echo '<br><br>'
echo '<a href="snapshot.cgi">[Take Snapshot]</a>'	

echo '</H3>'
cgi-bin/htmlEnd.cgi
#echo '</html>'
