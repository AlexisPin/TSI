message("Building tools library")

file(GLOB_RECURSE source_files "*.cpp")
file(GLOB_RECURSE header_files "*.h" "*.hpp")

message(" Source files")
foreach(line IN LISTS source_files)
  message("  ${line}")
endforeach()

message(" Header files")
foreach(line IN LISTS header_files)
  message("  ${line}")
endforeach()

add_library(tools ${source_files} ${header_files})
