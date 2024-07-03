#!/usr/bin/env bash
#
# @file prerelease_ops.sh
# @author Mike S Wang
# @brief Pre-release operations.
#

semantic-release --config .semrelrc.toml --noop version --print 2> /dev/null
semantic-release --config .semrelrc.toml changelog 2> /dev/null
