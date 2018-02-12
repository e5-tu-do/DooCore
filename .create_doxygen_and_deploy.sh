#!/bin/sh

set -e # exit with nonzero exit code if anything fails

echo 'Generating Doxygen code documentation...'
echo 'Using the following version of doxygen:'
doxygen --version
doxygen doxygen.conf

if [ -d "html" ] && [ -f "html/index.html" ];
then
	# add remote ssh-key to key-storage
	# first add remote host to known hosts
	echo ''
	echo 'known hosts before:'
	cat ~/.ssh/known_hosts
	ssh-keygen -R $DEPLOY_HOST
	echo ''
	echo 'known hosts between:'
	cat ~/.ssh/known_hosts
	ssh-keyscan -t rsa -p 10023 $DEPLOY_HOST 2> /dev/null | sort -u - ~/.ssh/known_hosts -o ~/.ssh/known_hosts
	echo ''
	echo 'known hosts after:'
	cat ~/.ssh/known_hosts
	echo ''
	echo 'checking directory .ssh'
	ls ~/.ssh/
	echo ''
	# decrypt private shh key
	openssl aes-256-cbc -K $encrypted_26877b322e98_key -iv $encrypted_26877b322e98_iv -in id_rsa_doocore.enc -out id_rsa_doocore -d
	# start ssh-agent and add the key
	eval "$(ssh-agent -s)"
	chmod 600 id_rsa_doocore
	ssh-add id_rsa_doocore

	echo "Uploading.."
	# upload site
	rsync -rq --delete --exclude=".*" -e "ssh -p 10023" html/ $DEPLOY_USER@$DEPLOY_HOST:$DEPLOY_PATH
else
    echo '' >&2
    echo 'Warning: No documentation (html) files have been found!' >&2
    echo 'Warning: Not going to push the documentation to docs.e5.physik.tu-dortmund.de' >&2
    exit 1
fi
