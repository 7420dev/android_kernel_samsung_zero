#!/bin/bash
git fetch f2fs

CURBRANCH=$(git rev-parse --abbrev-ref HEAD)

git branch -D f2fs-temp 2>/dev/null
git checkout -b f2fs-temp
git reset --hard f2fs/master
git revert --no-edit 5f2c4179e129bdc47870a81a65d0aff85aa18293 2>/dev/null
git commit -a --no-edit
git revert --no-edit 6e77137b363b8d866ac29c5a0c95e953614fb2d8 2>/dev/null
git commit -a --no-edit
git revert --no-edit 680baacbca69d18a6d7315374ad83d05ac9c0977 2>/dev/null
git commit -a --no-edit

git checkout $CURBRANCH

find . -not -path '*/\.*' -name '*f2fs*' | grep -v 'update_f2fs\|recovery\|ramdisk' | while read f2fs; do rm -rf $f2fs; git checkout f2fs-temp -- $f2fs; done

rm -rf fs/f2fs
git checkout f2fs-temp -- fs/f2fs

sed -i -e 's/wb.dirty_exceeded/dirty_exceeded/g' fs/f2fs/*
git add .

git branch -D f2fs-temp

git log --oneline f2fs/linux-3.10 | head -1 | awk '{print $1}' | while read patch; do git cherry-pick --no-commit $patch; done
git cherry-pick --no-commit 95d50dbdeb056fed87bf7e96f843acae279d82fb
git cherry-pick --no-commit b10ff1a6525382be951d60709b991b5533326404
git status
