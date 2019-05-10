#!/bin/sh
NETIFD_MAIN_DIR=../../scripts
. $NETIFD_MAIN_DIR/netifd-wireless.sh

init_wireless_driver "$@"

drv_mac80211_init_device_config() {
	# identifiers
	config_add_string macaddr
	config_add_string path
	config_add_string phy

	# config
	config_add_int channel
	config_add_string hwmode
	config_add_array ht_capab

	config_add_int chanbw
}

drv_mac80211_init_iface_config() {
	config_add_string macaddr

	config_add_boolean wds
	config_add_int maxassoc
	config_add_int dtim_period

	config_add_int max_listen_int

	config_add_boolean hidden
	config_add_boolean wmm
}

setup_vif() {
	local name="$1"

	json_select config
	json_get_var ssid ssid
	json_select ..

	wireless_add_vif "$name" "dummy-$ssid"
	/bin/sleep 10 &
	wireless_add_process "$!" /bin/sleep 1
}

drv_mac80211_cleanup() {
	echo "mac80211 cleanup"
}

drv_mac80211_setup() {
	echo "mac80211 setup: $1"
	json_dump
	for_each_interface "sta ap adhoc" setup_vif
	wireless_set_data phy=phy0
	wireless_set_up
}

drv_mac80211_teardown() {
	json_select data
	json_get_var phy phy
	json_select ..
	echo "mac80211 teardown: $1 ($phy)"
	json_dump
}

add_driver mac80211
