export SP_PROJECT_ROOT=$(realpath ../)
export SP_SERVER_SOURCE="${SP_PROJECT_ROOT}/source/server"
export SP_SERVER_BUILD="${SP_PROJECT_ROOT}/build/server"
export SP_SERVER_GENERATOR="Unix Makefiles"

mkdir -p $SP_SERVER_BUILD

cmake \
  -G "${SP_SERVER_GENERATOR}" \
  -S "${SP_SERVER_SOURCE}" \
  -B "${SP_SERVER_BUILD}" \
  -DSP_PROJECT_ROOT="${SP_PROJECT_ROOT}" \
  -DCMAKE_BUILD_TYPE=DEBUG \

cmake --build "${SP_SERVER_BUILD}"