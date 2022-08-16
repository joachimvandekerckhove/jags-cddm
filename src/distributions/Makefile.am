noinst_LTLIBRARIES = cddmdist.la

cddmdist_la_CPPFLAGS = -I$(top_srcdir)/src -I$(includedir)/JAGS -I/usr/jags/lib/ -I/usr/jags/include/JAGS/ -L/usr/jags/lib/ -lgsl

cddmdist_la_LDFLAGS = -no-undefined -module -avoid-version

cddmdist_la_SOURCES = DCDDM.cc

noinst_HEADERS = DCDDM.h
