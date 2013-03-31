#!/bin/bash

#By the way, whatever poor soul is reading this, I'm terrible at Bash scripting.
#-Aloshi

#Package dependencies first, because those are easy
sudo apt-get install libsdl1.2-dev libsdl-ttf2.0-dev libsdl-image1.2-dev libboost-filesystem-dev

#Install to home folder
pushd ~/

#install AngelScript
if [ ! -e "/usr/include/angelscript.h" ] && [ ! -e "/usr/local/include/angelscript.h" ]; then
	if [ ! -d "angelscript" ]; then
		mkdir angelscript
	else
		echo "~/angelscript directory exists. Stop."
		exit 0
	fi
	cd angelscript
	wget http://www.angelcode.com/angelscript/sdk/files/angelscript_2.26.1.zip -O angelscript.zip
	unzip angelscript.zip
	cd sdk/angelscript/projects/gnuc

	#Gotta edit the Makefile to build on the raspberry pi
	#this is hardcoded to line 103 which is bad, but my sed skills are nonexistant
	if lscpu | grep -q "armv6l"; then
		echo "Adding as_callfunc_arm_gcc to AS Makefile..."
		sed -i '103i else\n\tOBJ += \$(OBJDIR)\/as_callfunc_arm_gcc.o' makefile
	fi

	make
	sudo make install
	cd $HOME
	rm -rf angelscript
fi

popd
