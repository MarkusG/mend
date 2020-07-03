mend_page = configure_file(
  input: 'mend.1.in',
  output: 'mend.1',
  configuration: version_conf
)

menddb_page = configure_file(
  input: 'menddb.3.in',
  output: 'menddb.3',
  configuration: version_conf
)

install_man(mend_page)
install_man(menddb_page)
