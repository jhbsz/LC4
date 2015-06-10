subdir=utils qid


build:
	@for f in ${subdir} ; \
	do \
	    ${MAKE} -C $$f ${BUILD_MODE}; \
		CFLAGS="$(CFLAGS)" LDFLAGS="$(LDFLAGS)"; \
	done

unit:
	@for f in ${subdir} ; \
	do \
	    ${MAKE} -C $$f ${BUILD_MODE}; \
	done


clean:
	@for f in ${subdir} ; \
	do \
	    ${MAKE} -C $$f clean ; \
	done
