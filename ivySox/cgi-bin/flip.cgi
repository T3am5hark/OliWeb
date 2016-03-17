echo "Pin flip..."

PINNUMBER="$($UTIL/GetProperty pin $QUERY_STRING)"

echo "Flipping Pin#" $PINNUMBER

$UTIL/flip $PINNUMBER &
