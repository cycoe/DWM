#!/bin/sh
lock() {
    i3lock-fancy-rapid 5 5
}


text="lock\nlogout\nsuspend\nhibernate\nreboot\nshutdown"
# 此处 echo 加的 -e 参数非常重要，否则 \n 不会被正确解析
choice=$(echo -e $text | rofi -dmenu -p "Select a choice" -config ~/.config/rofi/rofi.config)

case $choice in
    lock)
        lock
        ;;
    logout)
        i3-msg exit
        ;;
    suspend)
        lock && systemctl suspend
        ;;
    hibernate)
        lock && systemctl hibernate
        ;;
    reboot)
        systemctl reboot
        ;;
    shutdown)
        systemctl poweroff
        ;;
    *)
        echo "Usage: $0 {lock|logout|suspend|hibernate|reboot|shutdown}"
        exit 2
esac

exit 0
