#!/bin/bash

psql postgresql://postgres@db -c 'CREATE DATABASE mend'
psql postgresql://postgres@db/mend -f sql/tables.sql
psql postgresql://postgres@db/mend -f test/tables.sql
ninja -C .build test
