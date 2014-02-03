
MAKEFILE_FLAGS   :=


.PHONY: all
all: release

.PHONY: debug_test
debug_test:  MAKEFILE_FLAGS := debug
debug_test:  build_lib

.PHONY:	test
test:  debug_test
	$(MAKE) -C unittest $(MAKEFILE_FLAGS)

.PHONY: debug
debug:  MAKEFILE_FLAGS := debug
debug:  build_lib
debug:  build_app

.PHONY:	release
release:	MAKEFILE_FLAGS := release
release:	build_lib
release:  build_app


.PHONY: install
install:  MAKEFILE_FLAGS := release
install:  build_app

.PHONY: clean
clean:  MAKEFILE_FLAGS := clean
clean:  build_lib
clean:  build_app
	$(MAKE) -C unittest $(MAKEFILE_FLAGS)

.PHONY:	build_lib
build_lib:
	$(MAKE) -C utf8small $(MAKEFILE_FLAGS)
	$(MAKE) -C engine $(MAKEFILE_FLAGS)

.PHONY: build_app
build_app:
	$(MAKE) -C excution $(MAKEFILE_FLAGS)

