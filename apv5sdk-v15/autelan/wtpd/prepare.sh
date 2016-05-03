#! /bin/bash
pushd $1
cvs CVSROOT=:pserver:qbuilder:qbuilder@192.168.1.18/rdoc login
cvs -d :pserver:qbuilder:qbuilder@192.168.1.18/rdoc checkout wcpss
popd
