EXTRA_DIST = $(doxygen_input) Doxyfile mainpage.dox

doxygen_input = mainpage.dox

if DOC

html_dir = $(pkgdatadir)/html
#html_DATA = html/index.html

#install-data-hook:
#	cp -pR html/* $(DESTDIR)$(html_dir)

# Automake's "distcheck" is sensitive to having files left over
# after "make uninstall", so we have to clean up the install hook.
uninstall-local:
	rm -rf $(DESTDIR)$(html_dir)

dox: html/index.html

else

# We repeat the three targets in both the "if" and "else" clauses
# of the conditional, because the generated makefile will contain
# references to the targets (target "install" depends on target
# "install-datahook", for example), and some make programs get upset
# if no target exists.

#install-data-hook:
uninstall-local:
dox:

endif


#all-local: dox

html/index.html: Doxyfile $(doxygen_input)
	"@DOXYGEN@"

# Make tarfile to distribute the HTML documentation.
doc-dist: dox
	tar -czf $(PACKAGE)-docs-$(VERSION).tar.gz -C html .

clean-local:
	$(RM) -r html
