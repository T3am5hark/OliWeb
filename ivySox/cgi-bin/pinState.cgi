$CGI_BIN/htmlStart.cgi
echo "<H3>"

echo 'GPIO Pin States<br><br>'

for pin in 17 22 23 24
do

  PINSTATE=$(gpio -g read $pin)

  echo "Pin $pin: $PINSTATE"
  echo " <a href=\"flip.cgi?pin=$pin\">(FLIP)</a><br>"

done
echo "</H3>"

$CGI_BIN/htmlEnd.cgi
