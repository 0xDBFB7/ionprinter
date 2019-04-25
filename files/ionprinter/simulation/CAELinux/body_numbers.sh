for s in {1..66}
do
   [ "$s" -eq 17 ] || [ "$s" -eq 52 ] || [ "$s" -eq 27 ] || [ "$s" -eq 48 ] || [ "$s" -eq 39 ]  && continue
   echo -n "$s "
done
