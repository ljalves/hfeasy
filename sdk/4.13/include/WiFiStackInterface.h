/* LWIP implementation of NetworkInterfaceAPI
 * Copyright (c) 2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if defined(TARGET_UNO_91H)

#ifndef WIFISTACK_INTERFACE_H
#define WIFISTACK_INTERFACE_H

#include "nsapi.h"
#include "rtos.h"
#include "lwip/netif.h"
#include "rda5991h_wland.h"

// Forward declaration
class NetworkStack;


/** WiFiStackInterface class
 *  Implementation of the NetworkStack for LWIP
 */
class WiFiStackInterface : public NetworkInterface
{
public:
    WiFiStackInterface();

    /** Set a static IP address
     *
     *  Configures this network interface to use a static IP address.
     *  Implicitly disables DHCP, which can be enabled in set_dhcp.
     *  Requires that the network is disconnected.
     *
     *  @param address  Null-terminated representation of the local IP address
     *  @param netmask  Null-terminated representation of the local network mask
     *  @param gateway  Null-terminated representation of the local gateway
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_network(
            const char *ip_address, const char *netmask, const char *gateway);

    /** Enable or disable DHCP on the network
     *
     *  Requires that the network is disconnected
     *
     *  @param dhcp     False to disable dhcp (defaults to enabled)
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_dhcp(bool dhcp);

    /** Set AP IP address
     *
     *  Configures this network interface to use a static IP address.
     *  Implicitly disables DHCP, which can be enabled in set_dhcp.
     *  Requires that the network is disconnected.
     *
     *  @param address      Null-terminated representation of the local IP address
     *  @param netmask      Null-terminated representation of the local network mask
     *  @param gateway      Null-terminated representation of the local gateway
     *  @param dhcp_start   Null-terminated representation of the dhcp pool start
     *  @param dhcp_end     Null-terminated representation of the dhcp pool end
     *  @return         0 on success, negative error code on failure
     */
    virtual nsapi_error_t set_network_ap(const char *ip_address, const char *netmask, const char *gateway,
                const char *dhcp_start, const char *dhcp_end);


    virtual int scan(void);

    /** scan a network
     *  @param ssid     Access Point to scan. could be NULL
     *  @param channel  Channel scan. could be 0
     *  @return         number of scan result, negative on failure
     */
    virtual int scan(const char *ssid, const unsigned char channel);

    /** scan a network
     *  @param ssid     Access Point to scan. could be NULL
     *  @param channel  Channel scan. could be 0
     *  @active, 0:passive scan; 1:active scan.
     *  @scan_time, scan time, unit is senond
     *  @return         number of scan result, negative on failure
     */
    virtual int scan(const char *ssid, const unsigned char channel,
        unsigned char active, unsigned char scan_time);

    /** get scan result
     *  @param bss_list store the scan result
     *  @param num      max number of bss_list
     *  @return         number of scan result, negative on failure
     */

    virtual int scan_result(rda5981_scan_result *bss_list, unsigned int num);

    /** get joined sta info
     *  @param sta_list store the scan result
     *  @param num      max number of bss_list
     *  @return         number of joined sta, negative on failure
     */
    virtual int ap_join_info(rda5981_apsta_info *sta_list, unsigned int num);

    /** Set msg queue handler to driver
     *
     *  Driver will send msg to this queue, like disconnect.
     *
     *  @return         void
     */
    virtual void set_msg_queue(void *queue);

    /** init wifi interface
     *
     *  @return         void
     */
    virtual void init();

    /**
     * Set station mode link loss times. It is effective in sleep mode, too.
     * The default value is 50.
     * if you want to change it, you should call this founction before connect to an ap.
     * You couldn't change it after first time scan or connect.
     * @link_loss_time, wifi station will consider ap is no available if we could not receive beacon @link_loss_time times.
     * the actual time you can receive mac disconnect event is (link_loss_time * beacon_period)
     */
    virtual void set_sta_link_loss_time(unsigned char time);

    /**
     * Set station mode beacon listen interval. It is only effective in sleep mode.
     * The default value is 1.
     * If you want to change it, you should call this founction before connect to an ap.
     * You couldn't change it after first time scan or connect.
     * @interval, wifi station will wake up to receive beacon every @interval beacon period, when it is in sleep mode.
     * the actual time you can receive mac disconnect event in sleep mode is (listen_interval * link_loss_time * beacon_period)
     */
    virtual void set_sta_listen_interval(unsigned char interval);

    /** Start the interface
     *
     *  Attempts to connect to a WiFi network. Requires ssid and passphrase to be set.
     *  If passphrase is invalid, NSAPI_ERROR_AUTH_ERROR is returned.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int connect(const char *ssid, const char *pass, const char *bssid = NULL, nsapi_security_t security = NSAPI_SECURITY_NONE, uint8_t channel = 0);
    virtual nsapi_error_t connect();


    /** Restart the interface
     *
     *  Attempts to reconnect to a WiFi network. Ssid and passphrase has been stored.
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int reconnect();


    /** Start a Access Point
     *
     *  @param ssid     Access Point name
     *  @param pass     Access Point password
     *  @param channel  Access Point channel
     *  @param mode     Access Point hidden mode
     *  @return         0 on success, negative error code on failure
     */
    virtual int start_ap(const char *ssid, const char *pass, int channel);
    virtual int start_ap(const char *ssid, const char *pass, int channel, char mode);

    /** Stop a Access Point
     *
     *  @return         0 on success, negative error code on failure
     */
    virtual int stop_ap(unsigned char state);


	virtual int restart_ap(int channel);
    /** Stop the interface
     *  @return             0 on success, negative on failure
     */
    virtual int disconnect();

    /** Get the internally stored MAC address
     *  @return             MAC address of the interface
     */
    virtual const char *get_mac_address();

    /** Get the internally stored softap MAC address
     *  @return             MAC address of the softap interface
     */
    virtual const char *get_mac_address_ap();

    /** Get the internally stored IP address
     *  @return             IP address of the interface or null if not yet connected
     */
    virtual const char *get_ip_address();

    /** Get the local network mask
     *
     *  @return         Null-terminated representation of the local network mask
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_netmask();

    /** Get the local gateways
     *
     *  @return         Null-terminated representation of the local gateway
     *                  or null if no network mask has been recieved
     */
    virtual const char *get_gateway();

    /** Get the internally stored AP IP address
     *  @return             IP address of the AP interface or null if not yet connected
     */
    virtual const char *get_ip_address_ap();

    /** Get the internally stored AP netmask address
     *  @return             IP address of the AP interface or null if not yet connected
     */
    virtual const char *get_netmask_ap();

    /** Get the internally stored AP Gateway address
     *  @return             IP address of the AP interface or null if not yet connected
     */
    virtual const char *get_gateway_ap();

    /** Get the internally stored AP dhcp start address
     *  @return             IP address of the AP interface or null if not yet connected
     */
    virtual const char *get_dhcp_start_ap();

    /** Get the internally stored AP dhcp end address
     *  @return             IP address of the AP interface or null if not yet connected
     */
    virtual const char *get_dhcp_end_ap();

    /** Gets the current BSSID if sta has been connnected
     *
     *  @return         string of AP's MAC
     */
    virtual const char *get_BSSID();

    /** Gets the current current for active connection
     *
     *  @return         Current channel
     */
    virtual int8_t get_channel();

    /** Gets the current radio signal strength for active connection
     *
     *  @return         Connection strength in dBm (negative value)
     */
    virtual int8_t get_rssi();

    /** Update the current radio signal strength for active connection
     *
     *  @return    NONE
     */
    virtual void update_rssi();

    /** Gets the current active connection access name
     *
     *  @return         Name of the active connection access
     */
    virtual const char *get_ssid();

/*
    const char *get_sm_host_ip_address();
    const char *get_sm_ssid();
    const char *get_sm_passwd();
*/
protected:
    int send_smartconfig_response_to_host();
    char hex_to_number(char a);
    /** Provide access to the underlying stack
     *
     *  @return The underlying network stack
     */
    virtual NetworkStack * get_stack();

    char SSID[32+1];
    char passwd[64+1];
    char BSSID[NSAPI_MAC_BYTES];
    char sm_host_ip_addr[NSAPI_IPv4_SIZE];
    bool _dhcp;
    char _ip_address[IPADDR_STRLEN_MAX];
    char _netmask[NSAPI_IPv4_SIZE];
    char _gateway[NSAPI_IPv4_SIZE];
    char _ip_address_ap[NSAPI_IPv4_SIZE];
    char _netmask_ap[NSAPI_IPv4_SIZE];
    char _gateway_ap[NSAPI_IPv4_SIZE];
    char _dhcp_start[NSAPI_IPv4_SIZE];
    char _dhcp_end[NSAPI_IPv4_SIZE];
    rda5981_scan_result _bss;
	bool _apactive;
	char _ap_ssid[32+1];
	char _ap_password[64+1];
	char _ap_channel;
	char _ap_mode;
	char _is_connected;
};

#endif

#endif

