#!/bin/bash
lttng create
lttng enable-event --kernel sched_*
lttng enable-event --userspace --all
lttng start
