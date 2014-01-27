
REPLACE_URL=$1

cat util/redirect.html | sed -e "s/REDIRECT_URL/$1/g"
