#!/bin/sh
#
# Driver script for autoconf for HIVM project.
#
# Rerun the script any time you add or remove source code files and/or
# directories. The source code files are specified in files
# Makefile.am (one per source code directory).
#
# After running this script, run
#
# % ./configure
# % make -f Makefile
#
# NOTE: remove all files named `makefile' from source-code
# directories. Otherwise, autoconf gets confused and displays
# unhelpful message "No rule to make target `all-am'.".
#
aclocal
autoconf
automake --add-missing --copy
./configure
