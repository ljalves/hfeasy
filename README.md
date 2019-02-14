# hfeasy
HF-LPx100 firmware for Ankuoo devices

If you like this project please [donate](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=5SMU6YE5XG6JG&source=url)!

# Implemented Features:
* MQTT client
* HTTP config, control and status webpages

After flashing hfeasy, you'll have access to the following web pages:
* Config:
  * http://\<deviceip\>/config
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
* Wifi Plug (http://www.lumitek.cn/en/productsd.php?pid=1095)
![Wifi Plug](http://www.lumitekgroup.com/upload/2015062911265369.jpg)

* Wifi Module (http://www.lumitek.cn/en/productsd.php?pid=1093)
![Wifi Module](http://www.lumitekgroup.com/upload/2015062315503112.jpg)


# Firmware
* [Download here](https://drive.google.com/open?id=1HWl-QiYM2u8lW9TAv1M6Kr9DPkj1_wRG)
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

* Original firmware (in case you want to go back):
  * [Plug](http://lumitek.yunext.com/files/DF_1_5114.bin)
  * [Module](http://lumitek.yunext.com/files/D1_1_V1.5116.bin)
  * Also available on the gdrive folder + factory webpage .bin

# Device info
All devices have a firmware flash webpage at: http://\<deviceip\>/iweb.html

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
