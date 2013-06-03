echo "Pin flip..."

PINNUMBER="$(util/GetProperty pin $QUERY_STRING)"

echo "Flipping Pin#" $PINNUMBER

util/flip $PINNUMBER &
