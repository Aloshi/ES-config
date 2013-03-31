#!/bin/bash

#By the way, whatever poor soul is reading this, I'm terrible at Bash scripting.
#-Aloshi

#Package dependencies first, because those are easy
# +cmake for angelscript
sudo apt-get install libsdl1.2-dev libsdl-ttf2.0-dev libsdl-image1.2-dev

#Install to home folder
cd ~/

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
	make
	sudo make install
	cd ../../../../
	rm -rf angelscript
fi
