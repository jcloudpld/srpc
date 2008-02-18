DIRS=src test

.PHONY: all clean test clean_dep

all clean test ::
	@for i in $(DIRS) ""; do \
		if [ "x$$i" != "x" ]; then \
			(cd ../../$$i && $(MAKE) $@) || exit 1; \
		fi; \
	done

clean_dep ::
	@for i in $(DIRS) ""; do \
		if [ "x$$i" != "x" ]; then \
			(cd ../../$$i && rm -f *.d); \
		fi; \
	done
