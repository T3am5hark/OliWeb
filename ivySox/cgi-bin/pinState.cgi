cgi-bin/htmlStart.cgi
echo "<H2>"

echo 'GPIO Pin States</br></br>'

#allPins=( 17 22 23 24 )
#for pin in "${allPins[@]}"
for pin in 17 22 23 24
do

  PINSTATE=$(gpio -g read $pin)

  echo "Pin $pin: $PINSTATE"
  #echo $PINSTATE
  echo " <a href=\"flip.cgi?pin=$pin\">(FLIP)</a></br>"

done
echo "</H2>"

cgi-bin/htmlEnd.cgi
