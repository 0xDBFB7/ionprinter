for s in {1..66}
do
   [ "$s" -eq 16 ] || [ "$s" -eq 52 ] || [ "$s" -eq 27 ] || [ "$s" -eq 48 ] || [ "$s" -eq 40 ]  && continue
   echo -n "$s "
done
