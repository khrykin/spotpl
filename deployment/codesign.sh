#!/usr/bin/env sh

status() {
	[ -n "$2" ] && color=$2 || color=4

	bold=$(tput -T ansi bold; tput -T ansi setaf $color)
	normal=$(tput -T ansi sgr0)

	printf "[codesign] %s%s%s\n" "$bold" "$1" "$normal"
}

message() {
  prefix="$3"

  if [ -n "$2" ]
  then
    colored=$(tput -T ansi setaf $2)
	  normal=$(tput -T ansi sgr0)
  fi

  echo "$1" |
  while IFS= read -r line; do
	  printf "[codesign] %s%s%s\n" "$colored" "$line" "$normal"
  done
}

warning() {
  message "$1" 3
}

success() {
  message "$1" 2
}

error() {
	message "Error: $1" 1 1>&2
}

if [ -z "$1" ]
then
      error "You must set a path to the signed object"
      exit 1
fi

if [ -z "$CODESIGN_IDENTITY" ]
then
      error "CODESIGN_IDENTITY environment variable must be set"
      exit 2
fi

status "Signing: $1..."

sign_output=$(codesign \
                --deep \
                --force \
                --verbose \
                --timestamp \
                --options=runtime \
                --keychain "$CODESIGN_KEYCHAIN" \
                -s "$CODESIGN_IDENTITY" \
                "$1" 2>&1)

if [  -$? -ne 0 ]
then
      error "$sign_output"
      exit 3
else
  success "$sign_output"
fi

verify_output=$(codesign \
              --deep \
              --verify \
              --verbose \
              "$1" 2>&1)

if [  -$? -ne 0 ]
then
      error "$verify_output"
      exit 3
else
  success "$verify_output"
fi

status "Signed: $1"
