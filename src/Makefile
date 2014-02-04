#####################################################################
# GNU Makefile Template for multiple project                        #
# Author: Duc Minh Tran                                             #
# Email: 901sttAtgmailDotcom                                        #
#####################################################################


MAKEFILE_FLAGS   :=


.PHONY: all
all: build_app

.PHONY:	test
test:  build_lib
	$(MAKE) -C unittest $(MAKEFILE_FLAGS)


.PHONY: clean
clean:  MAKEFILE_FLAGS := clean
clean:  build_lib
clean:  build_app
	$(MAKE) -C unittest $(MAKEFILE_FLAGS)
	@rm -rvf build
	@rm -rvf lib

.PHONY:	build_lib
build_lib:
	$(MAKE) -C utf8small $(MAKEFILE_FLAGS)
	$(MAKE) -C engine $(MAKEFILE_FLAGS)

.PHONY: build_app
build_app: build_lib
build_app:
	$(MAKE) -C excution $(MAKEFILE_FLAGS)

