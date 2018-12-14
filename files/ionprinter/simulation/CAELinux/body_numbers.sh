for s in {1..28}
do
   [ "$s" -eq 15 ] || [ "$s" -eq 25 ] || [ "$s" -eq 26 ] || [ "$s" -eq 16 ] || [ "$s" -eq 28 ]  && continue
   echo -n "$s "
done
