alias_tests = executable('alias_tests',
  'alias.c',
  dependencies: deps,
  link_with: lib)

test('add alias', alias_tests, args : ['pass1'])
test('remove alias', alias_tests, args : ['pass2'])
test('get alias', alias_tests, args : ['fail'])
