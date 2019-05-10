N="
"

append() {
	local var="$1"
	local value="$2"
	local sep="${3:- }"

	eval "export -- \"$var=\${$var:+\${$var}\${value:+\$sep}}\$value\""
}

add_default_handler() {
	case "$(type $1 2>/dev/null)" in
		*function*) return;;
		*) eval "$1() { return; }"
	esac
}

set_default() {
	local __s_var="$1"
	local __s_val="$2"
	eval "export -- \"$__s_var=\${$__s_var:-\$__s_val}\""
}

_config_add_generic() {
	local type="$1"; shift

	for name in "$@"; do
		json_add_array ""
		json_add_string "" "$name"
		json_add_int "" "$type"
		json_close_array
	done
}

config_add_int() {
	_config_add_generic 5 "$@"
}

config_add_array() {
	_config_add_generic 1 "$@"
}

config_add_string() {
	_config_add_generic 3 "$@"
}

config_add_boolean() {
	_config_add_generic 7 "$@"
}
