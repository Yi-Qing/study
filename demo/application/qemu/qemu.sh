#!/bin/env bash

printf -v MACADDR "52:54:%02x:%02x:%02x:%02x" $(( $RANDOM & 0xff)) $(( $RANDOM & 0xff )) $(( $RANDOM & 0xff)) $(( $RANDOM & 0xff ))

function unused_random_port() {
    port=`shuf -i 6550-6650 -n 1`
    lsof -i:${port} &> /dev/null
    if [ $? -eq 0 ];    then
        unused_random_port
    fi
    echo ${port}
}

VM_NAME="xubuntu1804"

qemu-system-x86_64 \
    -name ${VM_NAME} \
    -enable-kvm \
    -machine q35 \
    -smp 4 -cpu host -m 4G \
    -drive file=machine/xubuntu1804,format=qcow2 \
    -boot order=d,menu=on \
    -snapshot \
    -virtfs local,id=shared_folder_dev_0,path=/home/yiqing/.vm/share/debian,security_model=none,mount_tag=shared0 \
    -net nic,macaddr="$MACADDR" \
    -display none \
    -vga qxl \
    -device virtio-serial-pci \
    -device virtserialport,chardev=spicechannel0,name=com.redhat.spice.0 \
    -chardev spicevmc,id=spicechannel0,name=vdagent \
    -spice unix=on,addr=${XDG_RUNTIME_DIR}/qemu/${VM_NAME}/spice.socket,disable-ticketing=on \
    -monitor telnet:127.0.0.1:`unused_random_port`,server,nowait \
#    -daemonize

#    -cdrom "/home/yiqing/.vm/iso/xubuntu-18.04.5-desktop-amd64.iso" \
#    -rtc base=localtime \
