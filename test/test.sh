#!/bin/bash

psql -U postgres postgresql://db -c 'CREATE DATABASE mend'
psql -U postgres postgresql://db/mend -f sql/tables.sql
psql -U postgres postgresql://db/mend -f test/tables.sql
ninja -C .build test
