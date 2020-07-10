mend_tests = executable('mend_tests',
  'mend.c',
  dependencies: deps,
  link_with: lib)

test('mend_init', mend_tests, args : ['mend_init'])
test('mend_cleanup', mend_tests, args : ['mend_cleanup'])
