DIRS=nsrpcTest EchoTest

.PHONY: all clean test

all clean test ::
	@for i in $(DIRS) ""; do \
		if [ "x$$i" != "x" ]; then \
			(cd $$i && $(MAKE) $@) || exit 1; \
		fi; \
	done

clean_dep ::
	@for i in $(DIRS) ""; do \
		if [ "x$$i" != "x" ]; then \
			(cd $$i && rm -f *.d); \
		fi; \
	done
