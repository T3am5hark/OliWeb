# Set pin 24 to input mode

detector=24
gpio -g mode $detector in

PIN=$(gpio -g read $detector);
LASTPIN=$PIN;
LOOPIT=1;

while [ "$LOOPIT" -eq "$LOOPIT" ]
do

PIN=$(gpio -g read $detector)

if [ "$PIN" -ne "$LASTPIN" ]
then
  STAMP=$(date)
  echo "DETECTED: $LASTPIN -> $PIN [$STAMP]"
  if [ "$PIN" -eq 1 ]
  then
    "$CGI_BIN/birdChatterShort.cgi"
  else
    "$CGI_BIN/birdChatterShort.cgi"
  fi
  PIN=$(gpio -g read $detector)
fi

LASTPIN=$PIN

sleep 0.025

done
