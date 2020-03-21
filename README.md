# hfeasy
HF-LPx100 firmware for Ankuoo devices

**WARNING:** There are some devices (newer?) that use a different module (the LPB130). They should be easy to identify since the main webpage is different and only allows to configure wifi. Don't flash this firmware or the original webpage binary on these devices as they can become a brick (only recoverable by serial bootloader).
I'll build the firmware image for these devices as soon as I get my hands in one of them.


If you like this project please [donate](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=5SMU6YE5XG6JG&source=url)!

# Implemented Features:
* MQTT client
* HTTP config, control and status webpages

After flashing hfeasy, you'll have access to the following web pages:
* Config:
  * http://\<deviceip\>/config
  * http://\<deviceip\>/config_mqtt
* Status:
  * http://\<deviceip\>/status
* Control:
  * status: http://\<deviceip\>/state
  * on: http://\<deviceip\>/state?sw=1
  * off: http://\<deviceip\>/state?sw=0
* Timers:
  * http://\<deviceip\>/timer

# Recovery mode
  * The device will reset config options and change to AP mode so that you can re-configure it.
    * Module: Toggle the switch 6 or more times within 3 sec
    * Plug: Press and hold the switch for more than 3 sec

# Supported devices (so far...)
* Wifi Plug ([Ankuoo REC](http://www.lumitek.cn/en/productsd.php?pid=1095), [Ankuoo NEO](https://www.ankuoo.com/products/?sort=1))

![Wifi Plug](http://www.lumitekgroup.com/upload/2015062911265369.jpg)

* Wifi Module (http://www.lumitek.cn/en/productsd.php?pid=1093)
![Wifi Module](http://www.lumitekgroup.com/upload/2015062315503112.jpg)

* US Wall Dimmer (AW51011D)
<img src="https://images-na.ssl-images-amazon.com/images/I/413UPchrIfL._SL1000_.jpg" width="400">

# Firmware
* [Download here](https://drive.google.com/open?id=1HWl-QiYM2u8lW9TAv1M6Kr9DPkj1_wRG)
* hfeasy 0v7
  * Changes:
    * MQTT fixes: Publish state when sub and pub topics are different
* hfeasy 0v6
  * Changes:
    * Added WiFi LED config option (under /config)
    * US wall dimmer module is now supported
* hfeasy 0v5
  * Changes:
    * Due to limited webpage size, config was split in 2 pages (/config and /config_mqtt)
    * Added "HTTP auth" config option (sets authentication ON for all webpages)
    * Increased switch debounce time (should fix random turn ons/offs)
    * /state webpage changed to JSON format - if your setup parses the previous webpage output, you need to change it.
* hfeasy 0v4
  * Changes:
    * Added more status (hostname and uptime)
    * Fixed missing HTTP headers
    * More robust switch debouncer for on/off module
* hfeasy 0v3
  * Changes:
    * Added MQTT QoS setting
    * Added module hostname setting
    * Added on/off countdown timers (http://\<ip\>/timer)
* hfeasy 0v2
  * Changes:
    * Added MQTT authentication
    * Added recovery mode for both devices: press the switch 6 times withing 2 sec and the device enters AP mode and resets config
    * Added buzzer driver (for on/off module only). Right now just for fun, try: http://\<ip\>/state?tone=3 (dont go > 21)
* haeasy 0v1
  * Initial release

* Original firmware:
  * Also available on the gdrive folder + factory webpage .bin

# Device info
All devices have a hard-coded firmware flash webpage at: http://\<deviceip\>/iweb.html

This webpage allows to configure the device wifi parameters and flash new firmware.
No matter which firmware you flash, this webpage is always there (make the device almost "unbrickable").

There is another webpage at: http://\<deviceip\>/

In here you can setup the wifi parameters, change the access user/pass and flash new firmware too.
The access to there webpages is password protected.
The user/pass defaults to admin/admin.
If you upgrade to the latest official firmware, the user/pass is changed to admin/Lumlink@100

# TODO
* Add dayly/weekly timers (configurable by webpage)
* ... and suggestions are welcome (open an issue)
