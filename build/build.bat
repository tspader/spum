emcc ..\..\source\main.c -o index.html --shell-file ..\..\source\index.html ^
  -DSOKOL_GLES3=1 ^
  -s USE_WEBGL2=1 ^
  -s ALLOW_MEMORY_GROWTH=1 -O2 ^
  -std=c11 ^
  -I ..\..\external\sokol\  -I ..\..\external\doublenickel  -I ..\..\external\gunslinger  -I ..\..\external\handmade -I ..\..\external