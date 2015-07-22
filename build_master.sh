#!/bin/bash
if [ ! "${1}" = "skip" ] ; then
	./build_clean.sh
	./build_kernel.sh CC='$(CROSS_COMPILE)gcc' "$@"
	./build_recovery.sh skip CC='$(CROSS_COMPILE)gcc' "$@"
fi

if [ -e boot.img ] ; then
	rm arter97-kernel-"$(cat version)".zip 2>/dev/null
	cp boot.img kernelzip/boot.img
	cd kernelzip/
	7z a -mx9 arter97-kernel-"$(cat ../version)"-tmp.zip *
	zipalign -v 4 arter97-kernel-"$(cat ../version)"-tmp.zip ../arter97-kernel-"$(cat ../version)".zip
	rm arter97-kernel-"$(cat ../version)"-tmp.zip
	cd ..
	ls -al arter97-kernel-"$(cat version)".zip
	rm kernelzip/boot.img
fi

if [ -e recovery.img ] ; then
	rm arter97-recovery-"$(cat version)"-philz_touch_"$(cat version_recovery | awk '{print $1}')".zip 2>/dev/null
	cp recovery.img recoveryzip/
	cd recoveryzip/
	cp ../kernelzip/META-INF/com/google/android/update-binary META-INF/com/google/android/update-binary
	sed -i -e s/PHILZ_VERSION/$(cat ../version_recovery | awk '{print $1}')/g -e s/CWM_VERSION/$(cat ../version_recovery | awk '{print $2 }')/g META-INF/com/google/android/updater-script
	7z a -mx9 arter97-recovery-"$(cat ../version)"-philz_touch_"$(cat ../version_recovery | awk '{print $1}')"-tmp.zip *
	zipalign -v 4 arter97-recovery-"$(cat ../version)"-philz_touch_"$(cat ../version_recovery | awk '{print $1}')"-tmp.zip ../arter97-recovery-"$(cat ../version)"-philz_touch_"$(cat ../version_recovery | awk '{print $1}')".zip
	rm arter97-recovery-"$(cat ../version)"-philz_touch_"$(cat ../version_recovery | awk '{print $1}')"-tmp.zip
	sed -i -e s/$(cat ../version_recovery | awk '{print $1}')/PHILZ_VERSION/g -e s/$(cat ../version_recovery | awk '{print $2 }')/CWM_VERSION/g META-INF/com/google/android/updater-script
	cd ..
	ls -al arter97-recovery-"$(cat version)"-philz_touch_"$(cat version_recovery | awk '{print $1}')".zip
	rm recoveryzip/META-INF/com/google/android/update-binary
	rm recoveryzip/recovery.img
fi
