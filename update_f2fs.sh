#!/bin/bash
git fetch f2fs

find . -not -path '*/\.*' -name '*f2fs*' | grep -v 'update_f2fs\|recovery\|ramdisk' | while read f2fs; do rm -rf $f2fs; git checkout f2fs/linux-3.10 -- $f2fs; done

rm -rf fs/f2fs
git checkout f2fs/linux-3.10 -- fs/f2fs

git cherry-pick --no-commit 95d50dbdeb056fed87bf7e96f843acae279d82fb
git cherry-pick --no-commit b10ff1a6525382be951d60709b991b5533326404
git status
