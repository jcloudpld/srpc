AUTOMAKE_OPTIONS = 1.9 foreign dist-bzip2 dist-zip

SUBDIRS = doc include src test benchmark examples

EXTRA_DIST = \
	build \
	CHANGES.txt \
	DEPENDENCIES.txt \
	INSTALL.txt \
	README.txt \
	THANKS.txt

dist-hook:
	find $(distdir) -name .svn | xargs rm -rf

.PHONY: release snapshot dox doc-dist 

release:
	rm -rf .deps */.deps
	$(MAKE) distcheck

snapshot:
	$(MAKE) dist distdir=$(PACKAGE)-`date +%Y-%m-%d` 

dox:
	$(MAKE) -C doc dox

doc-dist:
	$(MAKE) -C doc doc-dist
	mv -f doc/$(PACKAGE)-docs-$(VERSION).tar.gz . 