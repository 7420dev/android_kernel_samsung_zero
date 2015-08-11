#!/bin/bash
git fetch f2fs

find . -not -path '*/\.*' -name '*f2fs*' | grep -v 'update_f2fs\|recovery\|ramdisk' | while read f2fs; do rm -rf $f2fs; git checkout 6282adbf932c226f76e1b83e074448c79976fe75 -- $f2fs; done

rm -rf fs/f2fs
git checkout 6282adbf932c226f76e1b83e074448c79976fe75 -- fs/f2fs

git cherry-pick --no-commit ede50acb557df1ef707295efa33a85b1da8bba35
git cherry-pick --no-commit 5d8c190f0beb972f78e2904755d3940c9f6babbf
git cherry-pick --no-commit 925f2aaaf04e2ff29f4d10f35e68c54b9feacc3c
git status
