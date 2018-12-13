for s in {1..18}
do
   [ "$s" -eq 3 ] || [ "$s" -eq 18 ] || [ "$s" -eq 1 ] || [ "$s" -eq 12 ]  && continue
   echo -n "$s "
done
