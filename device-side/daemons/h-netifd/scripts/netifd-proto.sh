NETIFD_MAIN_DIR="${NETIFD_MAIN_DIR:-/lib/netifd}"
PROTO_DEFAULT_OPTIONS="defaultroute peerdns metric"

. /usr/share/libubox/jshn.sh
. $NETIFD_MAIN_DIR/utils.sh

proto_config_add_int() {
	config_add_int "$@"
}

proto_config_add_string() {
	config_add_string "$@"
}

proto_config_add_boolean() {
	config_add_boolean "$@"
}

proto_config_add_array() {
	config_add_array "$@"
}

proto_config_add_defaults() {
	proto_config_add_boolean "defaultroute"
	proto_config_add_boolean "peerdns"
	proto_config_add_int "metric"
}

proto_add_dynamic_defaults() {
	[ -n "$defaultroute" ] && json_add_boolean defaultroute "$defaultroute"
	[ -n "$peerdns" ] && json_add_boolean peerdns "$peerdns"
	[ -n "$metric" ] && json_add_int metric "$metric"
}

_proto_do_teardown() {
	json_load "$data"
	eval "proto_$1_teardown \"$interface\" \"$ifname\""
}

_proto_do_renew() {
	json_load "$data"
	eval "proto_$1_renew \"$interface\" \"$ifname\""
}

_proto_do_setup() {
	json_load "$data"
	_EXPORT_VAR=0
	_EXPORT_VARS=
	eval "proto_$1_setup \"$interface\" \"$ifname\""
}

proto_init_update() {
	local ifname="$1"
	local up="$2"
	local external="$3"

	PROTO_KEEP=0
	PROTO_INIT=1
	PROTO_TUNNEL_OPEN=
	PROTO_IPADDR=
	PROTO_IP6ADDR=
	PROTO_ROUTE=
	PROTO_ROUTE6=
	PROTO_PREFIX6=
	PROTO_DNS=
	PROTO_DNS_SEARCH=
	json_init
	json_add_int action 0
	[ -n "$ifname" -a "*" != "$ifname" ] && json_add_string "ifname" "$ifname"
	json_add_boolean "link-up" "$up"
	[ -n "$3" ] && json_add_boolean "address-external" "$external"
}

proto_set_keep() {
	PROTO_KEEP="$1"
}

proto_close_nested() {
	[ -n "$PROTO_NESTED_OPEN" ] && json_close_object
	PROTO_NESTED_OPEN=
}

proto_add_nested() {
	PROTO_NESTED_OPEN=1
	json_add_object "$1"
}

proto_add_tunnel() {
	proto_add_nested "tunnel"
}

proto_close_tunnel() {
	proto_close_nested
}

proto_add_data() {
	proto_add_nested "data"
}

proto_close_data() {
	proto_close_nested
}

proto_add_dns_server() {
	local address="$1"

	append PROTO_DNS "$address"
}

proto_add_dns_search() {
	local address="$1"

	append PROTO_DNS_SEARCH "$address"
}

proto_add_ipv4_address() {
	local address="$1"
	local mask="$2"
	local broadcast="$3"
	local ptp="$4"

	append PROTO_IPADDR "$address/$mask/$broadcast/$ptp"
}

proto_add_ipv6_address() {
	local address="$1"
	local mask="$2"
	local preferred="$3"
	local valid="$4"
	local offlink="$5"
	local class="$6"

	append PROTO_IP6ADDR "$address/$mask/$preferred/$valid/$offlink/$class"
}

proto_add_ipv4_route() {
	local target="$1"
	local mask="$2"
	local gw="$3"
	local source="$4"
	local metric="$5"

	append PROTO_ROUTE "$target/$mask/$gw/$metric///$source"
}

proto_add_ipv6_route() {
	local target="$1"
	local mask="$2"
	local gw="$3"
	local metric="$4"
	local valid="$5"
	local source="$6"
	local table="$7"

	append PROTO_ROUTE6 "$target/$mask/$gw/$metric/$valid/$table/$source"
}

proto_add_ipv6_prefix() {
	local prefix="$1"
	local valid="$2"
	local preferred="$3"

	if [ -z "$valid" ]; then
		append PROTO_PREFIX6 "$prefix"
	else
		[ -z "$preferred" ] && preferred="$valid"
		append PROTO_PREFIX6 "$prefix,$valid,$preferred"
	fi
}

_proto_push_ipv4_addr() {
	local str="$1"
	local address mask broadcast ptp

	address="${str%%/*}"
	str="${str#*/}"
	mask="${str%%/*}"
	str="${str#*/}"
	broadcast="${str%%/*}"
	str="${str#*/}"
	ptp="$str"

	json_add_object ""
	json_add_string ipaddr "$address"
	[ -n "$mask" ] && json_add_string mask "$mask"
	[ -n "$broadcast" ] && json_add_string broadcast "$broadcast"
	[ -n "$ptp" ] && json_add_string ptp "$ptp"
	json_close_object
}

_proto_push_ipv6_addr() {
	local str="$1"
	local address mask preferred valid offlink

	address="${str%%/*}"
	str="${str#*/}"
	mask="${str%%/*}"
	str="${str#*/}"
	preferred="${str%%/*}"
	str="${str#*/}"
	valid="${str%%/*}"
	str="${str#*/}"
	offlink="${str%%/*}"
	str="${str#*/}"
	class="${str%%/*}"

	json_add_object ""
	json_add_string ipaddr "$address"
	[ -n "$mask" ] && json_add_string mask "$mask"
	[ -n "$preferred" ] && json_add_int preferred "$preferred"
	[ -n "$valid" ] && json_add_int valid "$valid"
	[ -n "$offlink" ] && json_add_boolean offlink "$offlink"
	[ -n "$class" ] && json_add_string class "$class"
	json_close_object
}

_proto_push_string() {
	json_add_string "" "$1"
}

_proto_push_route() {
	local str="$1";
	local target="${str%%/*}"
	str="${str#*/}"
	local mask="${str%%/*}"
	str="${str#*/}"
	local gw="${str%%/*}"
	str="${str#*/}"
	local metric="${str%%/*}"
	str="${str#*/}"
	local valid="${str%%/*}"
	str="${str#*/}"
	local table="${str%%/*}"
	str="${str#*/}"
	local source="${str}"

	json_add_object ""
	json_add_string target "$target"
	json_add_string netmask "$mask"
	[ -n "$gw" ] && json_add_string gateway "$gw"
	[ -n "$metric" ] && json_add_int metric "$metric"
	[ -n "$valid" ] && json_add_int valid "$valid"
	[ -n "$source" ] && json_add_string source "$source"
	[ -n "$table" ] && json_add_string table "$table"
	json_close_object
}

_proto_push_array() {
	local name="$1"
	local val="$2"
	local cb="$3"

	[ -n "$val" ] || return 0
	json_add_array "$name"
	for item in $val; do
		eval "$cb \"\$item\""
	done
	json_close_array
}

_proto_notify() {
	local interface="$1"
	local options="$2"
	json_add_string "interface" "$interface"
	ubus $options call network.interface notify_proto "$(json_dump)"
}

proto_send_update() {
	local interface="$1"

	proto_close_nested
	json_add_boolean keep "$PROTO_KEEP"
	_proto_push_array "ipaddr" "$PROTO_IPADDR" _proto_push_ipv4_addr
	_proto_push_array "ip6addr" "$PROTO_IP6ADDR" _proto_push_ipv6_addr
	_proto_push_array "routes" "$PROTO_ROUTE" _proto_push_route
	_proto_push_array "routes6" "$PROTO_ROUTE6" _proto_push_route
	_proto_push_array "ip6prefix" "$PROTO_PREFIX6" _proto_push_string
	_proto_push_array "dns" "$PROTO_DNS" _proto_push_string
	_proto_push_array "dns_search" "$PROTO_DNS_SEARCH" _proto_push_string
	_proto_notify "$interface"
}

proto_export() {
	local var="VAR${_EXPORT_VAR}"
	_EXPORT_VAR="$(($_EXPORT_VAR + 1))"
	export -- "$var=$1"
	append _EXPORT_VARS "$var"
}

proto_run_command() {
	local interface="$1"; shift

	json_init
	json_add_int action 1
	json_add_array command
	while [ $# -gt 0 ]; do
		json_add_string "" "$1"
		shift
	done
	json_close_array
	[ -n "$_EXPORT_VARS" ] && {
		json_add_array env
		for var in $_EXPORT_VARS; do
			eval "json_add_string \"\" \"\${$var}\""
		done
		json_close_array
	}
	_proto_notify "$interface"
}

proto_kill_command() {
	local interface="$1"; shift

	json_init
	json_add_int action 2
	[ -n "$1" ] && json_add_int signal "$1"
	_proto_notify "$interface"
}

proto_notify_error() {
	local interface="$1"; shift

	json_init
	json_add_int action 3
	json_add_array error
	while [ $# -gt 0 ]; do
		json_add_string "" "$1"
		shift
	done
	json_close_array
	_proto_notify "$interface"
}

proto_block_restart() {
	local interface="$1"; shift

	json_init
	json_add_int action 4
	_proto_notify "$interface"
}

proto_set_available() {
	local interface="$1"
	local state="$2"
	json_init
	json_add_int action 5
	json_add_boolean available "$state"
	_proto_notify "$interface"
}

proto_add_host_dependency() {
	local interface="$1"
	local host="$2"
	local ifname="$3"

	# execute in subshell to not taint callers env
	# see tickets #11046, #11545, #11570
	(
		json_init
		json_add_int action 6
		json_add_string host "$host"
		[ -n "$ifname" ] && json_add_string ifname "$ifname"
		_proto_notify "$interface" -S
	)
}

proto_setup_failed() {
	local interface="$1"
	json_init
	json_add_int action 7
	_proto_notify "$interface"
}

init_proto() {
	proto="$1"; shift
	cmd="$1"; shift

	case "$cmd" in
		dump)
			add_protocol() {
				no_device=0
				no_proto_task=0
				available=0
				renew_handler=0
				teardown_on_l3_link_down=0

				add_default_handler "proto_$1_init_config"

				json_init
				json_add_string "name" "$1"
				json_add_array "config"
				eval "proto_$1_init_config"
				json_close_array
				json_add_boolean no-device "$no_device"
				json_add_boolean no-proto-task "$no_proto_task"
				json_add_boolean available "$available"
				json_add_boolean renew-handler "$renew_handler"
				json_add_boolean lasterror "$lasterror"
				json_add_boolean teardown-on-l3-link-down "$teardown_on_l3_link_down"
				json_dump
			}
		;;
		setup|teardown|renew)
			interface="$1"; shift
			data="$1"; shift
			ifname="$1"; shift

			add_protocol() {
				[[ "$proto" == "$1" ]] || return 0

				case "$cmd" in
					setup) _proto_do_setup "$1";;
					teardown) _proto_do_teardown "$1" ;;
					renew) _proto_do_renew "$1" ;;
					*) return 1 ;;
				esac
			}
		;;
	esac
}
