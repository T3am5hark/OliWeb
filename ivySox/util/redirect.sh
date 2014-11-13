
REPLACE_URL=$1

cat $UTIL/redirect.html | sed -e "s/REDIRECT_URL/$1/g"
