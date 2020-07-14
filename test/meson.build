mend_tests = executable('mend_tests',
  'mend.c',
  dependencies: deps,
  link_with: lib)

test('mend_init', mend_tests, args : ['mend_init'])
test('mend_cleanup', mend_tests, args : ['mend_cleanup'])

entity_tests = executable('entity_tests',
  'entity.c',
  dependencies: deps,
  link_with: lib)

test('mend_new_entity', entity_tests, args: ['mend_new_entity'])
test('mend_get_entities', entity_tests, args: ['mend_get_entities'])
test('mend_get_entity by name', entity_tests, args: ['mend_get_entity_by_name'])
test('mend_get_entity by UUID', entity_tests, args: ['mend_get_entity_by_uuid'])
test('mend_remove_entity by name', entity_tests, args: ['mend_remove_entity_by_name'])
test('mend_remove_entity by UUID', entity_tests, args: ['mend_remove_entity_by_uuid'])
test('mend_uid_from_name', entity_tests, args: ['mend_uid_from_name'])

alias_tests = executable('alias_tests',
  'alias.c',
  dependencies: deps,
  link_with: lib)

test('mend_new_alias', alias_tests, args: ['mend_new_alias'])
test('mend_get_alias by uuid', alias_tests, args: ['mend_get_alias_by_uuid'])
test('mend_get_aliases', alias_tests, args: ['mend_get_aliases'])
test('mend_get_alias by value', alias_tests, args: ['mend_get_alias_by_value'])
test('mend_remove_alias by uuid', alias_tests, args: ['mend_remove_alias_by_uuid'])
test('mend_remove_alias by value', alias_tests, args: ['mend_remove_alias_by_value'])

note_tests = executable('note_tests',
  'note.c',
  dependencies: deps,
  link_with: lib)

test('mend_new_note', note_tests, args: ['mend_new_note'])
test('mend_get_note', note_tests, args: ['mend_get_note'])
test('mend_get_notes', note_tests, args: ['mend_get_notes'])
test('mend_edit_note', note_tests, args: ['mend_edit_note'])
test('mend_remove_note', note_tests, args: ['mend_remove_note'])

relation_tests = executable('relation_tests',
  'relation.c',
  dependencies: deps,
  link_with: lib)

test('mend_new_relation', relation_tests, args: ['mend_new_relation'])
test('mend_get_relation', relation_tests, args: ['mend_get_relation'])
test('mend_get_related_entities', relation_tests, args: ['mend_get_related_entities'])
test('mend_edit_relation', relation_tests, args: ['mend_edit_relation'])
test('mend_remove_relation', relation_tests, args: ['mend_remove_relation'])
