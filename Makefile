BUILD_DIR:=build

.PHONY: all clean build ${BUILD_DIR}/Makefile derp

all: format project test

clean:
	@([ -d ${BUILD_DIR} ] && make --directory=${BUILD_DIR} clean && rm -rf ${BUILD_DIR}/*) || echo Nothing to clean

project: ${BUILD_DIR}/Makefile
	@make --directory=${BUILD_DIR}

test: ${BUILD_DIR}/Makefile
	@make --directory=${BUILD_DIR} test ARGS='-V'

format:
	@make --directory=${BUILD_DIR} clangformat

lint:
	@make --directory=${BUILD_DIR} lint

${BUILD_DIR}/Makefile:
	@[ -d ${BUILD_DIR} ] || mkdir -p ${BUILD_DIR}
	@[ -f ${BUILD_DIR}/Makefile ] || (cd ${BUILD_DIR} && cmake ${CMAKE_OPTS} ..)
