EXTRA_DIST=jags.nsi AdvUninstLog.nsh MultiUser64.nsh README.WIN

clean-local:
	rm -rf inst 

installer: 
	makensis -nocd -DVERSION=${PACKAGE_VERSION} \
        -DMAJOR=${JAGS_MAJOR} -DJAGSINC=$(srcdir) \
	-DLICENSE=$(top_srcdir)/COPYING $(srcdir)/jags.nsi

