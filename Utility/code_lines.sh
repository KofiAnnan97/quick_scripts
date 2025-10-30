#!/bin/bash

EXPECTED_EXTENSIONS=(
  "py" "c" "h" "cpp" "hpp" "rs"
  "java" "sql" "db" "sh" "ino" "ps1"
)

CWD="."
REPO_DIR=$CWD
IGNORE_STR=""

while getopts "d:i:" opt; do
  case $opt in
    d) REPO_DIR=$OPTARG
      ;;
    i) IGNORE_STR=$OPTARG
      ;;
    *) echo "Invalid command: $OPTARG"
      ;;
  esac
done

if [ "$REPO_DIR" == "$CWD" ]; then
  echo "No input given. Defaulting to current working directory."
fi

echo "Looking at '$(realpath "$REPO_DIR")'"

is_valid_extension() {
  is_valid="0"
  for extension in "${EXPECTED_EXTENSIONS[@]}"
  do
    if [[ $1 == *.$extension ]]; then
       is_valid="1"
    fi
  done
  echo $is_valid
}

in_ignored_directory() {
  declare -a IGNORE_ARR=()
  IFS=',' read -r -a IGNORE_ARR <<< "$2"
  can_ignore="0"
  for dir_name in "${IGNORE_ARR[@]}"
  do
    if [[ "$1" == */"$dir_name"/* ]]; then
      can_ignore="1"
      break
    fi
  done
  echo "$can_ignore"
}

is_valid_file() {
  is_valid="1"
  if [[ ! -f "$1" ]]; then
    is_valid="0"
  elif [[ $(is_valid_extension "$1") == "0" ]]; then
    is_valid="0"
  elif [[ $(in_ignored_directory "$1" "$2") == "1" ]]; then
    is_valid="0"
  fi
  echo $is_valid
}

count_lines() {
  file=$1
  count=0
  #if [ -f $1 ]; then
  #  count=$(wc -l < "$1")
  #fi
  while IFS='' read -r LINE || [ -n "${LINE}" ]; do
    if [ -z "$LINE" ]; then
      continue
    else
      ((count++))
    fi
  done < "$file"
  echo "$count"
}

LINE_COUNT=0

echo "+-----------------+"
echo "| File Breakdown  |"
echo "+-----------------+"

FILES=$(find "$REPO_DIR" -printf "%p\n")

for file in $FILES
do
  if [[ $(is_valid_file "$file" "$IGNORE_STR") == "1" ]]; then
    count=$(count_lines "$file")
    echo "$file -> $count"
    LINE_COUNT=$((LINE_COUNT+count))
  fi
done

echo "---------------------------------"
echo "Total Code Line Count (approx.): $LINE_COUNT  (may include comments)"