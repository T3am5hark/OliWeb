
REPLACE_URL=$1

cat $CGI_BIN/redirect.html | sed -e "s/REDIRECT_URL/$1/g"
