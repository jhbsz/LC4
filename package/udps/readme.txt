1. Copy udps test engine into user/ folder.
2. Open /user/Makefile add compiler condition.

   dir_$(CONFIG_USER_UDPS) += udps

3. Open config/config.in to added menuconfig parameter to support 
   compiler.
   bool 'udps'	CONFIG_USER_UDPS

4. To do make menuconfig
   
   Kernel/Library/Defaults Selection --->
   [*] Customize Vendor/User Settings
   Network Applications  --->
   [*] udps

5. Start at internet.c function initInternet.

   Using system command run udps test engine.

   system("udps &");

 



