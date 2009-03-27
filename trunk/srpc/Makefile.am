AUTOMAKE_OPTIONS = 1.9 foreign dist-bzip2 dist-zip

SUBDIRS = include src test benchmark examples

EXTRA_DIST = \
	build \
	doc \
	CHANGES.txt \
	DEPENDENCIES.txt \
	INSTALL.txt \
	README.txt \
	THANKS.txt

dist-hook:
	find $(distdir) -name .svn | xargs rm -rf

release:
	rm -rf .deps */.deps
	$(MAKE) distcheck

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)-`date +%Y-%m-%d` 

