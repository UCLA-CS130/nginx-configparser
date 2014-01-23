#!/bin/bash

GTEST_DIR=gtest-1.7.0
g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc
ar -rv libgtest.a gtest-all.o
g++ -isystem ${GTEST_DIR}/include -pthread config_parser_test.cc ${GTEST_DIR}/src/gtest_main.cc libgtest.a -o config_parser_test 
