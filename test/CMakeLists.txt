# proj test sub module

add_test_exe    (test_uscheme test_uscheme.cpp)
test_link_libs  (test_uscheme uscheme)
create_test     (test_uscheme)
# force additional dep to build scheme
add_dependencies(test_uscheme scheme)

add_test_exe    (test_uscheme_stream test_uscheme_stream.cpp)
test_link_libs  (test_uscheme_stream uscheme)
create_test     (test_uscheme_stream)
