#!/bin/bash
git fetch f2fs

find . -not -path '*/\.*' -name '*f2fs*' | grep -v 'update_f2fs\|recovery\|ramdisk' | while read f2fs; do rm -rf $f2fs; git checkout f2fs/linux-3.10 -- $f2fs; done

rm -rf fs/f2fs
git checkout f2fs/linux-3.10 -- fs/f2fs

git cherry-pick --no-commit ed44568d272a322d5dc272ceed5259b58b203ebc
git cherry-pick --no-commit 5562ccf9c1b3506f7aa7f961071582adde06a6d2
git status
