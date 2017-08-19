# Install OliWeb & Config to /usr/local/bin

TARGET_PATH="/opt/OliWeb"
if [ -e $TARGET_PATH ]
then
  echo "$TARGET_PATH already exists."
else
  echo "Creating $TARGET_PATH"
  mkdir $TARGET_PATH
fi

echo "Copying OliWeb to $TARGET_PATH"
cp OliWeb $TARGET_PATH/
cp -r web $TARGET_PATH/web
cp -r util $TARGET_PATH/util
cp -r cgi-bin $TARGET_PATH/cgi-bin

if [ -e $TARGET_PATH/OliWebConfig.xml ]
then
  echo "Config file already exists, leaving local copy."
else
  echo "Copying config file to $TARGET_PATH"
  #echo "NOTE: you will need to edit OliWebConfig.xml to point to absolute paths"
  #echo "for util, web, and cgi-bin folder locations."
  cp OliWebConfig.Install.xml $TARGET_PATH/OliWebConfig.xml
fi

# Copy startup script to init.d
if [ -e /etc/init.d/oliweb ]
then
  echo "Init script already in place."
else
  echo "Copying autorun script to etc/init.d/oliweb"
  cp oliweb.init.d /etc/init.d/oliweb
  # Tell system about it
  echo "Activating auto-start"
  sudo update-rc.d oliweb defaults
fi
