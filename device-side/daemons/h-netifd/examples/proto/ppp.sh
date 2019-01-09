#!/bin/sh
NETIFD_MAIN_DIR=../../scripts
. $NETIFD_MAIN_DIR/netifd-proto.sh

init_proto "$@"

ppp_generic_init_config() {
	proto_config_add_string "username"
	proto_config_add_string "password"
	proto_config_add_int "keepalive"
}

proto_ppp_init_config() {
	no_device=1
	available=1
	ppp_generic_init_config
}

proto_ppp_setup() {
	echo "ppp_setup($1): $2"
}

proto_ppp_teardown() {
	return
}

add_protocol ppp

proto_pppoe_init_config() {
	ppp_generic_init_config
}

proto_pppoe_setup() {
	local interface="$1"
	local device="$2"

	json_get_var username username
	json_get_var password password
	echo "pppoe_setup($interface, $device), username=$username, password=$password"
	proto_init_update pppoe-$interface 1
	proto_set_keep 1
	proto_add_ipv4_address "192.168.2.1" 32
	proto_add_dns_server "192.168.2.2"
	proto_add_ipv4_route "0.0.0.0" 0 192.168.2.2
	proto_add_data
	json_add_string "ppp-type" "pppoe"
	proto_close_data
	proto_send_update "$interface"

	proto_init_update pppoe-$interface 1
	proto_set_keep 1
	proto_add_ipv6_address "fe80::2" 64
	proto_add_ipv6_route "::0" 0 "fe80::1"
	proto_add_data
	json_add_string "ppp-type" "pppoe"
	proto_close_data
	proto_send_update "$interface"

	proto_run_command "$interface" sleep 30
}

proto_pppoe_teardown() {
	[ "$ERROR" = 9 ] && {
		proto_notify_error "$interface" PROCESS_KILLED
		proto_block_restart "$interface"
	}
	proto_kill_command "$interface"
	return
}

add_protocol pppoe
