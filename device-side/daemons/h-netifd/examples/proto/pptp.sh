#!/bin/sh
NETIFD_MAIN_DIR=../../scripts
. $NETIFD_MAIN_DIR/netifd-proto.sh

init_proto "$@"

proto_pptp_init_config() {
	no_device=1
	available=1

	proto_config_add_string "username"
	proto_config_add_string "password"
	proto_config_add_string server
}

proto_pptp_setup() {
	local interface="$1"
	local device="$2"

	json_get_var server server
	proto_add_host_dependency "$interface" "$server"

	json_get_var username username
	json_get_var password password
	echo "pptp_setup($interface), username=$username, password=$password"
	proto_init_update "pptp-$interface" 1
	proto_set_keep 1
	proto_add_ipv4_address "192.168.9.1" 32
	proto_add_dns_server "192.168.9.2"
	proto_add_ipv4_route "0.0.0.0" 0 192.168.9.2
	proto_add_data
	json_add_string "ppp-type" "pptp"
	proto_close_data
	proto_send_update "$interface"

	proto_run_command "$interface" sleep 30
}

proto_pptp_teardown() {
	return
}

add_protocol pptp

