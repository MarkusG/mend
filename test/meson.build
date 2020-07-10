mend_tests = executable('mend_tests',
  'mend.c',
  dependencies: deps,
  link_with: lib)

test('mend_init', mend_tests, args : ['init'])
test('mend_cleanup', mend_tests, args : ['cleanup'])
