export SP_PROJECT_ROOT=$(realpath ../)
export SP_CLIENT_SOURCE="${SP_PROJECT_ROOT}/source/client"
export SP_CLIENT_BUILD="${SP_PROJECT_ROOT}/build/client"
export SP_CLIENT_GENERATOR="Unix Makefiles"

mkdir -p $SP_CLIENT_BUILD

emcmake cmake \
  -G "${SP_CLIENT_GENERATOR}" \
  -S "${SP_CLIENT_SOURCE}" \
  -B "${SP_CLIENT_BUILD}" \
  -DSP_PROJECT_ROOT="${SP_PROJECT_ROOT}" \
  -DCMAKE_BUILD_TYPE=DEBUG 

cmake --build "${SP_CLIENT_BUILD}"