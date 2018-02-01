#!/bin/sh

echo 'Generating Doxygen code documentation...'
doxygen --version
apt-cache policy doxygen
doxygen doxygen.conf

if [ -d "html" ] && [ -f "html/index.html" ]; then

    echo 'Uploading documentation to the docs.e5.physik.tu-dortmund.de...'
else
    echo '' >&2
    echo 'Warning: No documentation (html) files have been found!' >&2
    echo 'Warning: Not going to push the documentation to docs.e5.physik.tu-dortmund.de' >&2
    exit 1
fi
