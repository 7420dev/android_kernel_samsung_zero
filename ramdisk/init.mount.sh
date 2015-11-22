#!/res/busybox sh

export PATH=/res/asset:$PATH
export ext4=1

mount -t ext4 -o ro,noatime,nodiratime,noauto_da_alloc,nodiscard,data=ordered,errors=panic /dev/block/platform/15570000.ufs/by-name/SYSTEM /system
mount -t f2fs -o ro,noatime,nodiratime,background_gc=off,nodiscard /dev/block/platform/15570000.ufs/by-name/SYSTEM /system

mount -t ext4 -o noatime,nodiratime,nosuid,nodev,noauto_da_alloc,discard,data=ordered,errors=panic /dev/block/platform/15570000.ufs/by-name/USERDATA /arter97/data || export ext4=0
mount -t f2fs -o noatime,nodiratime,nosuid,nodev,background_gc=on,discard /dev/block/platform/15570000.ufs/by-name/USERDATA /arter97/data

if [ -f /arter97/data/.arter97/btaltrom ] ; then
	export ext4=0
	umount -f /system
	chmod 755 /arter97/data/arter97_secondrom/system
	chmod 771 /arter97/data/arter97_secondrom/data
	chmod 771 /arter97/data/arter97_secondrom/cache
	mount --bind /arter97/data/arter97_secondrom/system /system
	mount --bind /arter97/data/arter97_secondrom/data /data
	mount --bind /arter97/data/arter97_secondrom/cache /cache
	mkdir /arter97/data/.arter97
	mkdir /data/.arter97
	rm -rf /data/.arter97/*
	rm -rf /data/.arter97/.*
	chmod 777 /arter97/data/.arter97
	chmod 777 /data/.arter97
	mount --bind /arter97/data/.arter97 /data/.arter97
	mount --bind -o remount,suid,dev /system
	if [ -f /arter97/data/media/0/.arter97/shared ]; then
		rm -rf /arter97/data/arter97_secondrom/data/media/0/.arter97
		cp -rp /arter97/data/arter97_secondrom/data/media/* /arter97/data/media/
		mount --bind /arter97/data/media /data/media
	fi
	CUR_PATH=$PATH
	export PATH=/sbin:/system/sbin:/system/bin:/system/xbin
	export LD_LIBRARY_PATH=/vendor/lib:/system/lib
	run-parts /arter97/data/arter97_secondrom/init.d
	export PATH=$CUR_PATH
else
	mount --bind /arter97/data /data
	mount -t ext4 -o noatime,nodiratime,nosuid,nodev,noauto_da_alloc,discard,data=ordered,errors=panic /dev/block/platform/15570000.ufs/by-name/CACHE /cache
	mount -t f2fs -o noatime,nodiratime,nosuid,nodev,background_gc=on,discard /dev/block/platform/15570000.ufs/by-name/CACHE /cache
fi

if [[ $ext4 == "1" ]]; then
	sed -i -e 's@# USERDATA@/dev/block/platform/15570000.ufs/by-name/USERDATA	/data	ext4	noatime,nosuid,nodev,noauto_da_alloc,discard,journal_async_commit,data=ordered,errors=panic	wait,check,encryptable=footer@g' /fstab.samsungexynos7420
	umount -f /data
	umount -f /arter97/data
fi

touch /dev/block/mounted
