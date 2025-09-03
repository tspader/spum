export SP_PROJECT_ROOT=$(realpath ../)
export SP_CLI_SOURCE="${SP_PROJECT_ROOT}/source/cli"
export SP_CLI_BUILD="${SP_PROJECT_ROOT}/build/cli"
export SP_CLI_GENERATOR="Unix Makefiles"

mkdir -p $SP_CLI_BUILD

cmake \
  -G "${SP_CLI_GENERATOR}" \
  -S "${SP_CLI_SOURCE}" \
  -B "${SP_CLI_BUILD}" \
  -DSP_PROJECT_ROOT="${SP_PROJECT_ROOT}" \
  -DCMAKE_BUILD_TYPE=DEBUG \

cmake --build "${SP_CLI_BUILD}"