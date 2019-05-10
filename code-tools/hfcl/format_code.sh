#!/bin/bash

sed -i 's/\t/    /g' `find . -name '*.cc'`
sed -i 's/\t/    /g' `find . -name '*.h'`

sed -i 's/\s\+$//g' `find . -name '*.cc'`
sed -i 's/\s\+$//g' `find . -name '*.h'`

exit 0
