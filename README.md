License & Disclaimer
====================
BoneMarrow is licensed under the GPLv3, a copy of which is included in the LICENSE.txt file.  

    BoneMarrow is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
  
    BoneMarrow is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with BoneMarrow.  If not, see <http://www.gnu.org/licenses/>.
    

BoneMarrow
==========

BoneMarrow is the ARM Firmware (and platform drivers) for ZarthCode's VR products.  Namely our VR glove and it's accessories. In a nutshell, it takes a large volume of orientation data and uses it to create a skeletal model, which can be analyzed for gestures, or sent in a raw format over usb or bluetooth.  Additionally, this software can interface with externally connected accessories.

Hosted on github at the url: https://github.com/zarthcode/BoneMarrow  
  
  
Windows 8.1 Development Environment Setup
=========================================
This is what we're using...

  
Visual Studio 2013 Professional (Optional, http://msdn.microsoft.com)  
VisualGDB (Optional, http://www.visualgdb.com/)  

GCC-arm-none-eabi: Launchpad GNU Tools for ARM Embedded Processors (https://launchpad.net/gcc-arm-embedded/+download)

  
Required Hardware
=================
  
MantisVR (EVP4 or later)  
Bluetooth 4.0/LE adapter  
ST-Link/V2 and drivers (http://goo.gl/FjQ2A3)  
Tag-Connect TC2050-ARM2010 (ARM 20-pin to TC2050 Adapter - http://www.tag-connect.com/TC2050-ARM2010)  
Tag-Connect TC2050-IDC-NL (10-pin plug of nails - http://www.tag-connect.com/TC2050-IDC-NL)  

Project Components
==================
  
BoneMarrow.sln - Visual Studio 2013 project solution  
Diagnostic - Basic hardware functional and connectivity testing firmware.  
Glove - VR 
(TODO)Bootloader - DFU bootloader firmware (Allows update to new firmware without use of st-link.)  
  
  
  
TODO:
Windows setup instructions
Linux setup instructions
Mac setup instructions
Bootloader installation instructions

