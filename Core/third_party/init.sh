#!/bin/bash
third_party_list=(
  "./jsoncpp"
  "https://github.com/open-source-parsers/jsoncpp.git"
)

for ((i=0; i < ${#third_party_list[*]}; i+=2))
do
  if [ ! -d "${third_party_list[${i}]}" ]; then
     git clone "${third_party_list[${i+1}]}"
  fi
done  
echo 'Finish clone third-party:'
for dir in $(ls)
do
  [ -d $dir ] && echo $dir
done
