License & Disclaimer
====================
This software is licensed under the GPLv3, a copy of which is included in the LICENSE.txt file.  

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    

BoneMarrow
==========

ARM Firmware and platform drivers for ZarthCode's VR products.  
Hosted on github at the url: https://github.com/zarthcode/BoneMarrow  
  
Disclaimer
==========

This is pre-release software, meant to be used with pre-release hardware.  Use at your own risk.  
  
  
Windows 8.1 Development Environment Setup
=========================================
  
Visual Studio 2013 Professional (Optional, http://msdn.microsoft.com)  
VisualGDB (http://www.visualgdb.com/)  
Sysprogs GNU Toolchain (GCC 4.8.2 http://gnutoolchains.com/arm-eabi/)  
  
  
Required Hardware
=================
  
MantisVR (EVP4)  
Bluetooth 4.0/LE adapter  
ST-Link/V2 and drivers (http://goo.gl/FjQ2A3)  
Tag-Connect TC2050-ARM2010 (ARM 20-pin to TC2050 Adapter - http://www.tag-connect.com/TC2050-ARM2010)  
Tag-Connect TC2050-IDC-NL (10-pin plug of nails - http://www.tag-connect.com/TC2050-IDC-NL)  

Project Components
==================
  
BoneMarrow.sln - Visual Studio 2013 project solution  
Diagnostic - Basic hardware functional and connectivity testing firmware.  
(TODO)Bootloader - DFU bootloader firmware (Allows update to new firmware without use of st-link.)  
  
  
  
TODO:
Windows setup instructions
Linux setup instructions
Mac setup instructions
Bootloader installation instructions

