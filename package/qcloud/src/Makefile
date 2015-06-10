subdir=libs porter sample

all: build

build:
	@for f in ${subdir} ; \
	do \
		${MAKE} -C $$f \
		CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}"; \
	done

clean:
	@for f in ${subdir} ; \
	do \
		${MAKE} -C $$f clean; \
	done
