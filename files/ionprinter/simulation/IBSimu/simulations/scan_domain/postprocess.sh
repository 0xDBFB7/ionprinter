#find data/1/ -type f -name '*.csv' -n -exec cat {} \; > assembled_output.csv
#find data/1/ -type f -name '*.csv' | sed -e 's/data\/1\///g' -e 's/\/data.csv//g'
echo -n > assembled_output.csv
cat $(find data/1/ -type f -name '*.csv' | sort -V) > assembled_output.csv
