#!/bin/sh

reserv_link()
{
    local chkfolder="$1"
    local filterword="$2"
    local newtarget="$3"
    local chktgt
    for chktgt in $(ls $chkfolder); do
        [ -L "$chkfolder/$chktgt" -o -L "$chktgt" ] && {
            [ -n "$(ls -al $chkfolder/$chktgt | grep $filterword)" ] && {
                echo "do reserv for $chkfolder/$chktgt : ln -sf $newtarget $chkfolder/$chktgt"
                ln -sf $newtarget $chkfolder/$chktgt
            }
        }
    done
}

backup_link()
{
    local tpath="$1"
    local tfile
    local tdir
    for linkfs in $(find "$tpath" -type l); do
        tfile=$(basename $linkfs)
        tdir=$(dirname $linkfs)
        echo cd $tdir
        cd $tdir
        targfs="$(ls -al $linkfs | awk '{print $11}')"
        targfsbn="$(basename $targfs)_zzzz"
        echo cp $targfs /tmp/$targfsbn
        cp $targfs /tmp/$targfsbn
        echo ln -s /tmp/$targfsbn ${linkfs}_1
        ln -sf /tmp/$targfsbn ${linkfs}_1
        echo mv ${linkfs}_1 ${linkfs}
        mv -f ${linkfs}_1 ${linkfs}
        echo cd -
        cd -
    done
}

backup_one_file_with_link()
{
    local trycount=10
    local link_result=1
    local source_file="$1"
    local target_file="$2"
    [ ! -f "$source_file" ] && return
    cp $source_file $target_file

    while [ "$trycount" -gt "0" -a "$link_result" != "0" ]; do
        ln -sf $target_file $source_file
        link_result=$?
        [ "$link_result" = "0" ] && break
        sleep 1
        trycount=$(($trycount - 1))
    done
}

# cp /bin/busybox /tmp/
# mv /usr/sbin/nvram /tmp/
# ln -s /tmp/nvram /usr/sbin/nvram
libfiles="libuClibc-0.9.29.so ld-uClibc-0.9.29.so libdl-0.9.29.so libm-0.9.29.so libnsl-0.9.29.so libutil-0.9.29.so"
usrlibfiles="libnvram.so libshared.so"
for libfl in $libfiles; do
    backup_one_file_with_link "/lib/$libfl" "/tmp/$libfl"
done
for libfl in $usrlibfiles; do
    backup_one_file_with_link "/usr/lib/$libfl" "/tmp/$libfl"
done
backup_one_file_with_link "/www/resp_upgrade_success.asp" "/tmp/resp_upgrade_success.asp"
binfiles="busybox"
sbinfiles="ifconfig mtd"
usebinfiles="gpioctl"
usesbinfiles="nvram ezp-i2c upgraded ezpup"
etcfiles="rc.common functions.sh network.sh arch.shi log.sh"
for libfl in $binfiles; do
    backup_one_file_with_link "/bin/$libfl" "/tmp/$libfl"
done
for libfl in $sbinfiles; do
    backup_one_file_with_link "/sbin/$libfl" "/tmp/$libfl"
done
for libfl in $usebinfiles; do
    backup_one_file_with_link "/usr/bin/$libfl" "/tmp/$libfl"
done
for libfl in $usesbinfiles; do
    backup_one_file_with_link "/usr/sbin/$libfl" "/tmp/$libfl"
done
for libfl in $etcfiles; do
    backup_one_file_with_link "/etc/$libfl" "/tmp/$libfl"
done



#backup_one_file_with_link "/usr/sbin/httpd" "/tmp/httpd_cmd"
#reserv_link "/sbin" "busybox" "/tmp/busybox"
#reserv_link "/bin" "busybox" "/tmp/busybox"
#reserv_link "/usr/sbin" "busybox" "/tmp/busybox"
#reserv_link "/usr/bin" "busybox" "/tmp/busybox"
# backup_link "/usr"
#backup_link "/usr/sbin"
#backup_link "/usr/bin"
#backup_link "/bin"
#backup_link "/sbin"
# backup_link "/lib"
backup_link "/etc/init.d"

