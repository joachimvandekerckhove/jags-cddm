SUBDIRS = distributions

jagsmod_LTLIBRARIES = cddm.la

cddm_la_SOURCES = cddm.cc

cddm_la_CPPFLAGS = -I$(includedir)/JAGS -I/usr/jags/lib/ -I/usr/jags/include/JAGS/ -L/usr/jags/lib/

cddm_la_LIBADD = distributions/cddmdist.la
cddm_la_LIBADD += -L/usr/jags/lib/ -ljags -ljrmath

cddm_la_LDFLAGS = -module -avoid-version



