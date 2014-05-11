Linux settings:

Compiler:
-DAPL=0 -D LIN=1 -D IBM=0 

Linker Libraries:
-lphtread -lusb -lm

For standalone application:
Exclude xplane.c from build...


For Plugin build:

add /SDK/CHeaders/XPLM/ as include directory
change Build Artifact under project settings:
Artifact Type: Shared Library
Artifact name: motionplatform
Artifact extension: xpl
Output prefix: 


Windows settings:
Compiler:
-DAPL=0 -D LIN=0 -D IBM=1 

Includes:
-include
-SDK\CHeaders\XPLM

Linker Libraries:
-lib
-SDK\Libraries\Win
-lphtread -lusb -lopengl32 -lglu32 -lXPLM

Artifact Type: Shared Library
Artifact name: motionplatform
Artifact extension: xpl
Output prefix: 




Linux libusb access problem:
http://ubuntu-ky.ubuntuforums.org/showthread.php?t=901891

Basically, when a device is sort of a non-standard device (this is a little hard for me to explain), in particular not a usb file system, the mount rules in /etc/udev/rules.d/40-basic-permissions.rules apply. Now, be aware that the syntax changed going from 7.10 to 8.04, and right now I can't recall the pre-8.04 syntax. To fix my problem, I created a file called 41-cvs-permissions.rules - just be sure the number is above 40 and doesn't conflict with an existing file. Note that I created this file while running as root, so be sure to match it's permissions to those of the 40-basic-permissions.rules file.

My file contains the following - hopefully you'll now see the correlation of the numbers:

# USB devices (usbfs replacement)
SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device",SYSFS{idVendor}=="0dca" , SYSFS{idProduct}=="0027", MODE="0666"
SUBSYSTEM=="usb", ENV{DEVTYPE}=="usb_device",SYSFS{idVendor}=="167b" , SYSFS{idProduct}=="0101", MODE="0666"


Please note that the second usb_device is for another set of CVS cameras have. The idea is to make it either world accessable as I did with MODE="0666", or to set a group id or other such thing.

If you follow my previous post of the lsusb and this file, you will see I'm dealing with the usb device of major id (vendor) of "0dca" and minor id (product) of "0027". It just was by coincidence that it was the first in my lsusb list. These particular devices for me never come up with a name.