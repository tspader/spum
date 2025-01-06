source ./build-utils.sh

export SP_PROJECT_ROOT=$(realpath ../)
export SP_SERVER_SOURCE="${SP_PROJECT_ROOT}/source/server"
if is_windows; then
  export SP_SERVER_BUILD="${SP_PROJECT_ROOT}/build/win-x86_64/server"
  export SP_SERVER_GENERATOR="MinGW Makefiles"
else
  export SP_SERVER_BUILD="${SP_PROJECT_ROOT}/build/linux-x86_64/server"
  export SP_SERVER_GENERATOR="Unix Makefiles"
fi

mkdir -p $SP_SERVER_BUILD

cmake \
  -G "${SP_SERVER_GENERATOR}" \
  -S "${SP_SERVER_SOURCE}" \
  -B "${SP_SERVER_BUILD}" \
  -DSP_PROJECT_ROOT="${SP_PROJECT_ROOT}"

cmake --build "${SP_SERVER_BUILD}"