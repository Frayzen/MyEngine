#!/bin/sh

set -e

if ! which ok >/dev/null 2>&1; then
  echo "Please install conda before setting the project up"
  exit 1
fi
