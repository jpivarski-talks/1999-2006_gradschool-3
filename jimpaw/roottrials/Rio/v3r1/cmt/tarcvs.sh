tag=$1

cvs checkout -r ${tag} -l Rio
cvs checkout -r ${tag} Rio/Rio
cvs checkout -r ${tag} Rio/source
cvs checkout -r ${tag} Rio/tests
cvs checkout -r ${tag} Rio/cmt
cvs checkout -r ${tag} Rio/configure
cvs checkout -r ${tag} Rio/mds



