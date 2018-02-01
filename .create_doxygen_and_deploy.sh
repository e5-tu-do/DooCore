#!/bin/sh

echo 'Generating Doxygen code documentation...'
# doxygen doxygen.conf
echo 'This is the encrypted user:'
echo $FOO

if [ -d "html" ] && [ -f "html/index.html" ]; then

    echo 'Uploading documentation to docs.e5.physik.tu-dortmund.de...'
else
    echo '' >&2
    echo 'Warning: No documentation (html) files have been found!' >&2
    echo 'Warning: Not going to push the documentation to docs.e5.physik.tu-dortmund.de' >&2
    exit 1
fi
