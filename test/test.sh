#!/bin/bash

psql -U postgres postgresql://db -c 'CREATE DATABASE mend'
psql -U postgres postgresql://db -f sql/tables.sql
ninja -C .build test
