# Install OliWeb & Config to /usr/local/bin

TARGET_PATH="/usr/local/bin"

echo "Copying OliWeb to $TARGET_PATH"
cp OliWeb $TARGET_PATH

if [ -e $TARGET_PATH/OliWebConfig.xml ]
then
  echo "Config file already exists, leaving local copy."
else
  echo "Copying config file to $TARGET_PATH"
  cp OliWebConfig.xml $TARGET_PATH
fi

# Copy startup script to init.d


