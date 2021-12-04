#!/bin/sh

CFLAGS="-Wall -Werror -Wextra -I."

clang++ $CFLAGS -D NAMESPACE=ft main.cpp -o ft_containers || exit
clang++ $CFLAGS -D NAMESPACE=std main.cpp -o std_containers || exit

rm .stdpipe .ftpipe
mkfifo .stdpipe .ftpipe
./fifodiff .stdpipe .ftpipe &
 ./std_containers $@ >> .stdpipe | ./ft_containers $@ >> .ftpipe | pkill -TERM fifodiff

#sleep 10
#pkill diff
#pkill ft_containers
#pkill std_containers


