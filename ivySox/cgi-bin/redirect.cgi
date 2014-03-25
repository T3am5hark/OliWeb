
REPLACE_URL=$1

cat cgi-bin/redirect.html | sed -e "s/REDIRECT_URL/$1/g"
