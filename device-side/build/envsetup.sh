function hmm() {
cat <<EOF
Invoke ". build/envsetup.sh" from your shell to add the following functions to your environment:
- lunch:   lunch <product_name>-<build_variant>
- tapas:   tapas [<App1> <App2> ...] [arm|x86|mips|armv5|arm64|x86_64|mips64] [eng|userdebug|user]
- croot:   Changes directory to the top of the tree.
- m:       Makes from the top of the tree.
- mm:      Builds all of the modules in the current directory, but not their dependencies.
- mmm:     Builds all of the modules in the supplied directories, but not their dependencies.
           To limit the modules being built use the syntax: mmm dir/:target1,target2.
- mma:     Builds all of the modules in the current directory, and their dependencies.
- mmma:    Builds all of the modules in the supplied directories, and their dependencies.
- cgrep:   Greps on all local C/C++ files.
- ggrep:   Greps on all local Gradle files.
- jgrep:   Greps on all local Java files.
- resgrep: Greps on all local res/*.xml files.
- sgrep:   Greps on all local source files.
- godir:   Go to the directory containing a file.

Look at the source to view more functions. The complete list is:
EOF
    T=$(gettop)
    local A
    A=""
    for i in `cat $T/build/envsetup.sh | sed -n "/^[ \t]*function /s/function \([a-z_]*\).*/\1/p" | sort | uniq`; do
      A="$A $i"
    done
    echo $A
}

PLATFORM_CHOICES=(nuclear astar octopus tulip azalea sitar cello banjo)
# check to see if the supplied product is one we can build
function check_platform()
{
    for v in ${PLATFORM_CHOICES[@]}
    do
        if [ "$v" = "$1" ]
        then
            return 0
        fi
    done
    return 1
}

# Get the value of a build variable as an absolute path.
function get_abs_build_var()
{
    T=$(gettop)
    if [ ! "$T" ]; then
        echo "Couldn't locate the top of the tree.  Try setting TOP." >&2
        return
    fi
    (\cd $T; CALLED_FROM_SETUP=true BUILD_SYSTEM=build \
      command make --no-print-directory -f build/config.mk dumpvar-abs-$1)
}

# Get the exact value of a build variable.
function get_build_var()
{
    T=$(gettop)
    if [ ! "$T" ]; then
        echo "Couldn't locate the top of the tree.  Try setting TOP." >&2
        return
    fi
    (\cd $T; CALLED_FROM_SETUP=true BUILD_SYSTEM=build \
      command make --no-print-directory -f build/config.mk dumpvar-$1)
}

function check_product()
{
    T=$(gettop)
    if [ ! "$T" ]; then
        echo "Couldn't locate the top of the tree.  Try setting TOP." >&2
        return
    fi
    TARGET_PRODUCT=$1 \
    TARGET_BUILD_VARIANT= \
    TARGET_BUILD_TYPE= \
    TARGET_BUILD_APPS= \
    get_build_var TARGET_DEVICE > /dev/null
}

VARIANT_CHOICES=(tina dragonboard)

# check to see if the supplied variant is valid
function check_variant()
{
    for v in ${VARIANT_CHOICES[@]}
    do
        if [ "$v" = "$1" ]
        then
            return 0
        fi
    done
    return 1
}

function printconfig()
{
    T=$(gettop)
    if [ ! "$T" ]; then
        echo "Couldn't locate the top of the tree.  Try setting TOP." >&2
        return
    fi

    get_build_var report_config
}

function set_stuff_for_environment()
{
    set_sequence_number

    export TINA_BUILD_TOP=$(gettop)
    # With this environment variable new GCC can apply colors to warnings/errors
    export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'
}

function set_sequence_number()
{
    export BUILD_ENV_SEQUENCE_NUMBER=10
}

# Clear this variable.  It will be built up again when the vendorsetup.sh
# files are included at the end of this file.
unset LUNCH_MENU_CHOICES
function add_lunch_combo()
{
    local new_combo=$1
    local c
    for c in ${LUNCH_MENU_CHOICES[@]} ; do
        if [ "$new_combo" = "$c" ] ; then
            return
        fi
    done
    LUNCH_MENU_CHOICES=(${LUNCH_MENU_CHOICES[@]} $new_combo)
}

# add the default one here

function print_lunch_menu()
{
    local uname=$(uname)
    echo
    echo "You're building on" $uname
    echo
    echo "Lunch menu... pick a combo:"

    local i=1
    local choice
    for choice in ${LUNCH_MENU_CHOICES[@]}
    do
        echo "     $i. $choice"
        i=$(($i+1))
    done

    echo
}

function lunch()
{
    local answer

    if [ "$1" ] ; then
        answer=$1
    else
        print_lunch_menu
        echo -n "Which would you like?"
        read answer
    fi

    local selection=

    if [ -z "$answer" ]
    then
        selection=astar_parrot-tina
    elif (echo -n $answer | grep -q -e "^[0-9][0-9]*$")
    then
        if [ $answer -le ${#LUNCH_MENU_CHOICES[@]} ]
        then
            selection=${LUNCH_MENU_CHOICES[$(($answer-1))]}
        fi
    elif (echo -n $answer | grep -q -e "^[^\-][^\-]*-[^\-][^\-]*$")
    then
        selection=$answer
    fi

    if [ -z "$selection" ]
    then
        echo
        echo "Invalid lunch combo: $answer"
        return 1
    fi

    local platform=$(echo -n $selection | sed -e "s/_.*$//")
    check_platform $platform

    if [ $? -ne 0 ]
    then
        echo
        echo "** Don't have a platform spec for: '$platform'"
        echo "** Must be one of ${PLATFORM_CHOICES[@]}"
        echo "** Do you have the right repo manifest?"
        platform=
    fi
    local product=$(echo -n $selection | sed -e "s/-.*$//")
    check_product $product
    if [ $? -ne 0 ]
    then
        echo
        echo "** Don't have a product spec for: '$product'"
        echo "** Do you have the right repo manifest?"
        product=
    fi

    local variant=$(echo -n $selection | sed -e "s/^[^\-]*-//")
    check_variant $variant
    if [ $? -ne 0 ]
    then
        echo
        echo "** Invalid variant: '$variant'"
        echo "** Must be one of ${VARIANT_CHOICES[@]}"
        variant=
    fi

    if [ -z "$product" -o -z "$variant" -o -z "$platform" ]
    then
        echo
        return 1
    fi

    export TARGET_PRODUCT=$product
    export TARGET_PLATFORM=$platform
    export TARGET_BOARD=$(get_build_var TARGET_DEVICE)
    export TARGET_BUILD_VARIANT=$variant
    export TARGET_BUILD_TYPE=release

    rm -rf tmp
    echo

    set_stuff_for_environment
    printconfig
}

function get_chip()
{
	local chip=
	if [ "x$TARGET_PLATFORM" = "xsitar" ]; then
		chip=sun3iw1p1
	elif [ "x$TARGET_PLATFORM" = "xnuclear" ]; then
		chip=sun5i
	elif [ "x$TARGET_PLATFORM" = "xastar" ]; then
		chip=sun8iw5p1
	elif [ "x$TARGET_PLATFORM" = "xoctopus" ]; then
		chip=sun8iw6p1
	elif [ "x$TARGET_PLATFORM" = "xbanjo" ]; then
		chip=sun8iw8p1
	elif [ "x$TARGET_PLATFORM" = "xcello" ]; then
		chip=sun8iw10p1
	elif [ "x$TARGET_PLATFORM" = "xazalea" ]; then
		chip=sun8iw11p1
	elif [ "x$TARGET_PLATFORM" = "xtulip" ]; then
		chip=sun50iw1p1
	fi
	echo $chip
}

# Build brandy(uboot,boot0,fes) if you want.
function mboot()
{
	local T=$(gettop)
	local chip=
	echo $TARGET_PRODUCT $TARGET_PLATFORM $TARGET_BOARD
	if [ -z "$TARGET_BOARD" -o -z "$TARGET_PLATFORM" ]; then
		echo "Please use lunch to select a target board before build boot."
		return
	fi

	chip=$(get_chip)
	if [ "x$chip" = "x" ]; then
		echo "platform($TARGET_PLATFORM) not support"
		return
	fi

	cd $T/lichee/brandy/
	./build.sh -p $chip
	if [ $? -ne 0 ]; then
		echo "mboot stop for build error in brandy, Please check!"
		return
	fi
	cd - 1>/dev/null
	echo "mboot success!"
}
# Tab completion for lunch.
function _lunch()
{
    local cur prev opts
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    COMPREPLY=( $(compgen -W "${LUNCH_MENU_CHOICES[*]}" -- ${cur}) )
    return 0
}
complete -F _lunch lunch

function gettop
{
    local TOPFILE=build/envsetup.sh
    if [ -n "$TOP" -a -f "$TOP/$TOPFILE" ] ; then
        # The following circumlocution ensures we remove symlinks from TOP.
        (cd $TOP; PWD= /bin/pwd)
    else
        if [ -f $TOPFILE ] ; then
            # The following circumlocution (repeated below as well) ensures
            # that we record the true directory name and not one that is
            # faked up with symlink names.
            PWD= /bin/pwd
        else
            local HERE=$PWD
            T=
            while [ \( ! \( -f $TOPFILE \) \) -a \( $PWD != "/" \) ]; do
                \cd ..
                T=`PWD= /bin/pwd -P`
            done
            \cd $HERE
            if [ -f "$T/$TOPFILE" ]; then
                echo $T
            fi
        fi
    fi
}

function mm() {
	local T=$(gettop)
	$T/scripts/mm.sh $T $*
}

function mlibc() {
	local T=$(gettop)
	$T/scripts/mlibc.sh $T $*
}

function make_img_md5(){
    #$1: target image
    md5sum $1 | awk '{print $1}' > $1.md5
}
function print_red(){
    echo -e '\033[0;31;1m'
    echo $1
    echo -e '\033[0m'
}
function make_ota_image(){
    local T=$(gettop)
    local chip=sunxi
    local need_usr=0
    local make_ota_fail=0
    [ x$CHIP = x"sun5i" ] && chip=sun5i
    local BIN_DIR=$T/out/${TARGET_BOARD}
    local OTA_DIR=$BIN_DIR/ota
    mkdir -p $OTA_DIR
    print_red "build ota package"
    grep "CONFIG_SUNXI_SMALL_STORAGE_OTA=y" $T/.config && need_usr=1
    #target image
    target_list=($BIN_DIR/boot.img $BIN_DIR/rootfs.img $BIN_DIR/usr.img)
    if [ $need_usr -eq 0 ];then
        target_list=($BIN_DIR/boot.img $BIN_DIR/rootfs.img)
    fi
    [ -n $1 ] && [ x$1 = x"--force" ] && rm -rf $target_list
    for i in $target_list[@]; do
        if [ ! -f $i ]; then
            print_red "$i is not exsit! rebuild the image."
            make -j16
            if [ $? -ne 0 ]
            then
                print_red "make $img file! make_ota_image fail!"
                make_ota_fail=1
            fi
            break
        fi
    done

    rm -rf $OTA_DIR/target_sys
    mkdir -p $OTA_DIR/target_sys
    cp $BIN_DIR/boot.img $OTA_DIR/target_sys/
    make_img_md5 $OTA_DIR/target_sys/boot.img

    cp $BIN_DIR/rootfs.img $OTA_DIR/target_sys/
    make_img_md5 $OTA_DIR/target_sys/rootfs.img
    if [ $need_usr -eq 1 ];then
        cp $BIN_DIR/usr.img $OTA_DIR/target_sys/
        make_img_md5 $OTA_DIR/target_sys/usr.img
        rm -rf $OTA_DIR/usr_sys
        mkdir -p $OTA_DIR/usr_sys
        cp $BIN_DIR/usr.img $OTA_DIR/usr_sys/
        make_img_md5 $OTA_DIR/usr_sys/usr.img
    fi
    #upgrade image
    mv $T/.config $OTA_DIR/.config.old

    grep -v -e CONFIG_TARGET_ROOTFS_INITRAMFS  $T/target/linux/${TARGET_BOARD}/defconfig_ota > .config
    echo 'CONFIG_TARGET_ROOTFS_INITRAMFS=y' >> .config
	echo 'CONFIG_TARGET_AW_OTA_INITRAMFS=y' >> .config
	echo 'CONFIG_TARGET_INITRAMFS_COMPRESSION_XZ=y' >> .config
	cp .config $T/target/linux/${TARGET_BOARD}/defconfig
    #refresh_ota_env
    make V=s -j16
    if [ $? -ne 0 ]
    then
        print_red "make_ota_image fail!"
        make_ota_fail=1
    fi

    cp $OTA_DIR/.config.old $T/.config
    cp $OTA_DIR/.config.old $T/target/linux/${TARGET_BOARD}/defconfig

    rm -rf $OTA_DIR/ramdisk_sys
    mkdir -p $OTA_DIR/ramdisk_sys

    cp $BIN_DIR/boot_initramfs.img $OTA_DIR/ramdisk_sys/
    make_img_md5 $OTA_DIR/ramdisk_sys/boot_initramfs.img

    if [ $need_usr -eq 1 ];then
        cd $OTA_DIR && \
            tar -zcvf target_sys.tar.gz target_sys && \
            tar -zcvf ramdisk_sys.tar.gz ramdisk_sys && \
            tar -zcvf usr_sys.tar.gz usr_sys && \
            cd $T
    else
        cd $OTA_DIR && \
            tar -zcvf target_sys.tar.gz target_sys && \
            tar -zcvf ramdisk_sys.tar.gz ramdisk_sys && \
            cd $T
    fi
    #refresh_ota_env
    if [ $make_ota_fail -ne 0 ];then
        print_red "build ota packag fail!"
    else
        print_red "build ota packag finish!"
    fi
}

usage()
{
	printf "Usage: pack [-cCHIP] [-pPLATFORM] [-bBOARD] [-d] [-s] [-h]
	-c CHIP (default: $chip)
	-p PLATFORM (default: $platform)
	-b BOARD (default: $board)
	-d pack firmware with debug info output to card0
	-s pack firmware with signature
	-m pack dump firmware
	-h print this help message
"
}

function pack() {
	local T=$(gettop)
	local chip=sun5i
	local platform=$(get_build_var TARGET_BUILD_VARIANT)
	local board_platform=$(get_build_var TARGET_BOARD_PLATFORM)
	local board=$(get_build_var TARGET_BOARD)
	local debug=uart0
	local sigmode=none
	local securemode=none
	local mode=normal
	unset OPTIND
	while getopts "dsvmh" arg
	do
		case $arg in
			d)
				debug=card0
				;;
			s)
				sigmode=secure
				;;
			v)
				securemode=secure
				;;
			m)
				mode=dump
				;;
			h)
				usage
				return 0
				;;
			?)
			return 1
			;;
		esac
	done

	chip=$(get_chip)
	if [ "x$chip" = "x" ]; then
		echo "platform($TARGET_PLATFORM) not support"
		return
	fi

	$T/scripts/pack_img.sh -c $chip -p $platform -b $board \
		-d $debug -s $sigmode -m $mode -v $securemode -t $T
}

function createkeys()
{
	local T=$(gettop)
	local board=$(get_build_var TARGET_BOARD)
	$T/scripts/createkeys -b $board -t $T
}

function minstall()
{
	make $1install $*
	echo "make package"
}

function mclean()
{
	make $1clean $2
	echo "make clean"
}

function croot()
{
    T=$(gettop)
    if [ "$T" ]; then
        \cd $(gettop)
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}

function cboot()
{
    T=$(gettop)
    U=u-boot-2011.09
    if [ "$TARGET_PLATFORM" = "tulip" -o "$TARGET_PLATFORM" = "azalea" -o "$TARGET_PLATFORM" = "sitar" -o "$TARGET_PLATFORM" = "cello" ]; then
	    U=u-boot-2014.07
    fi
    if [ "$T" ]; then
        \cd $(gettop)/lichee/brandy/$U/
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}

function ckernel()
{
    T=$(gettop)
    K=linux-3.4
    if [ "$TARGET_PLATFORM" = "azalea" -o "$TARGET_PLATFORM" = "sitar" -o "$TARGET_PLATFORM" = "cello" ]; then
	    K=linux-3.10
	fi
	if [ "$TARGET_PLATFORM" = "tulip" ];then
		K=linux-4.4
	fi
    if [ "$T" ]; then
        \cd $(gettop)/lichee/$K/
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}

function cgeneric()
{
    T=$(gettop)
    if [ "$T" ]; then
        \cd $(gettop)/target/linux/generic
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}

function cdevice()
{
    T=$(gettop)
    if [ "$T" ]; then
	    \cd $(gettop)/target/linux/${TARGET_BOARD}
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}

function cconfigs()
{
    T=$(gettop)
    if [ "$T" ]; then
	    \cd $(gettop)/target/linux/${TARGET_BOARD}/configs
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}

function cout()
{
    T=$(gettop)
    if [ "$T" ]; then
	    \cd $(gettop)/out/${TARGET_BOARD}
    else
        echo "Couldn't locate the top of the tree.  Try setting TOP."
    fi
}

function cgrep()
{
    find . -name .repo -prune -o -name .git -prune -o -name out -prune -o -type f \( -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.h' \) -print0 | xargs -0 grep --color -n "$@"
}

function godir ()
{
    if [[ -z "$1" ]]; then
        echo "Usage: godir <regex>"
        return
    fi
    T=$(gettop)
    if [[ ! -f $T/filelist ]]; then
        echo -n "Creating index..."
        (\cd $T; find . -wholename ./out -prune -o -wholename ./.repo -prune -o -type f > filelist)
        echo " Done"
        echo ""
    fi
    local lines
    lines=($(\grep "$1" $T/filelist | sed -e 's/\/[^/]*$//' | sort | uniq))
    if [[ ${#lines[@]} = 0 ]]; then
        echo "Not found"
        return
    fi
    local pathname
    local choice
    if [[ ${#lines[@]} > 1 ]]; then
        while [[ -z "$pathname" ]]; do
            local index=1
            local line
            for line in ${lines[@]}; do
                printf "%6s %s\n" "[$index]" $line
                index=$(($index + 1))
            done
            echo
            echo -n "Select one: "
            unset choice
            read choice
            if [[ $choice -gt ${#lines[@]} || $choice -lt 1 ]]; then
                echo "Invalid choice"
                continue
            fi
            pathname=${lines[$(($choice-1))]}
        done
    else
        pathname=${lines[0]}
    fi
    \cd $T/$pathname
}

# Print colored exit condition
function pez
{
    "$@"
    local retval=$?
    if [ $retval -ne 0 ]
    then
        echo -e "\e[0;31mFAILURE\e[00m"
    else
        echo -e "\e[0;32mSUCCESS\e[00m"
    fi
    return $retval
}

function get_make_command()
{
  echo command make V=s
}

function make()
{
    local start_time=$(date +"%s")
    $(get_make_command) "$@"
    local ret=$?
    local end_time=$(date +"%s")
    local tdiff=$(($end_time-$start_time))
    local hours=$(($tdiff / 3600 ))
    local mins=$((($tdiff % 3600) / 60))
    local secs=$(($tdiff % 60))
    echo
    if [ $ret -eq 0 ] ; then
        echo -n -e "#### make completed successfully "
    else
        echo -n -e "#### make failed to build some targets "
    fi
    if [ $hours -gt 0 ] ; then
        printf "(%02g:%02g:%02g (hh:mm:ss))" $hours $mins $secs
    elif [ $mins -gt 0 ] ; then
        printf "(%02g:%02g (mm:ss))" $mins $secs
    elif [ $secs -gt 0 ] ; then
        printf "(%s seconds)" $secs
    fi
    echo -e " ####"
    echo
    return $ret
}



if [ "x$SHELL" != "x/bin/bash" ]; then
    case `ps -o command -p $$` in
        *bash*)
            ;;
        *)
            echo "WARNING: Only bash is supported, use of other shell would lead to erroneous results"
            ;;
    esac
fi

# Execute the contents of any vendorsetup.sh files we can find.
for f in `test -d target && find -L target -maxdepth 4 -name 'vendorsetup.sh' 2> /dev/null`
do
    echo "including $f"
    . $f
done
unset f
