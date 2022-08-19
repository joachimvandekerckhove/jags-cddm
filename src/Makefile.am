SUBDIRS = distributions

jagsmod_LTLIBRARIES = cddm.la

cddm_la_SOURCES = cddm.cc

cddm_la_CPPFLAGS = -I$(includedir)/JAGS

cddm_la_LIBADD = distributions/cddmdist.la
cddm_la_LIBADD += -ljags -ljrmath

cddm_la_LDFLAGS = -module -avoid-version



