#!/bin/bash

set -e

script_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$script_dir/.."
rm "./installed" -rf
