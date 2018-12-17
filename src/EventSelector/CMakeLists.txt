set(MTEventSelector_headers_files
  MTEventSelector.h
)

set(MTEventSelector_cxx_files
  MTEventSelector.cxx
)

#---Generate dictionary for MTEventSelector
ROOT_GENERATE_DICTIONARY(G__MTEventSelector
  ${MTEventSelector_headers_files}
LINKDEF MTEventSelector.LinkDef.h)

#---Create a shared library with geneated dictionary for MTEventSelector class
add_library(MTEventSelector SHARED
  ${MTEventSelector_cxx_files}
G__MTEventSelector.cxx)

target_link_libraries(MTEventSelector ${ROOT_LIBRARIES})