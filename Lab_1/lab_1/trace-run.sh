#!/bin/bash
export LD_PRELOAD=/usr/local/lib/*
alias tracing_test="stress -c 3 -t 10"
tracing_test
