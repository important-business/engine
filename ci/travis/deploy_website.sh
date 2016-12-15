#!/bin/bash
set -e # Exit with nonzero exit code if anything fails

DoDeploy () {
    deploydir="build-output/${REPO_VER}/${TRAVIS_OS_NAME}-${CXX}"
    echo "Deploying to ${deploydir}"
    mkdir -p "${deploydir}"
    cp -r ${ENGINEDIR}/build/coverage* "${deploydir}"
    ln -sf "${deploydir}" "build-output/latest"

}

REPO="https://github.com/important-business/engine-site.git"
DESTDIR=~/engine-site
SSH_REPO=${REPO/https:\/\/github.com\//git@github.com:}
REPO_VER=`git rev-parse --verify HEAD`

git clone "$REPO" "$DESTDIR"
ENGINEDIR="$(pwd)"
cd "${DESTDIR}"
DoDeploy

git config user.name "Travis CI"
git config user.email "Travis@nomail.com"

# Commit the "changes", i.e. the new version.
# The delta will show diffs between new and old versions.
git add .
git commit -m "Deploy to GitHub Pages: ${REPO_VER}"

# # Get the deploy key by using Travis's stored variables to decrypt deploy_key.enc
ENCRYPTED_KEY_VAR="encrypted_${DEPLOY_KEY_ENCRYPTION_LABEL}_key"
ENCRYPTED_IV_VAR="encrypted_${DEPLOY_KEY_ENCRYPTION_LABEL}_iv"
ENCRYPTED_KEY=${!ENCRYPTED_KEY_VAR}
ENCRYPTED_IV=${!ENCRYPTED_IV_VAR}

DEPLOY_KEY_ENC="${ENGINEDIR}/ci/travis/keys/deploy_key.enc"
DEPLOY_KEY="${HOME}/deploy_key"
openssl aes-256-cbc -K $ENCRYPTED_KEY -iv $ENCRYPTED_IV -in ${DEPLOY_KEY_ENC} -out ${DEPLOY_KEY} -d
chmod 600 ${DEPLOY_KEY}
eval `ssh-agent -s`
ssh-add ${DEPLOY_KEY}

# # Now that we're all set up, we can push.
git push $SSH_REPO HEAD