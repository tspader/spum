function is_windows {
  [[ "$(uname -s)" == "Windows_NT" ]]
}