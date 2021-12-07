#!/bin/sh

CFLAGS="-Wall -Werror -Wextra -std=c++98 -I../srcs/containers -I../srcs/utils"


trap "pkill fifodiff; rm .stdpipe .ftpipe" INT

cd monkey

check_last_change()
{
  ret=0
  for file in *.hpp
  do
    tmp="$(stat -c "%Y" $file )"
    : $(( ret = tmp > ret ? tmp : ret ))
  done
  echo $ret
}

if [  ! -x fifodiff ] || [ "$(stat -c %Y fifodiff.cpp)" -ge "$(stat -c %Y fifodiff)" ]
then
  echo "compiling fifodiff..."
  clang++ $CFLAGS fifodiff.cpp -o fifodiff || exit
fi
if [ ! -x ft_containers ] || [ "$(check_last_change)" -ge "$(stat -c %Y ft_containers)" -o "$(stat -c %Y main.cpp)" -ge "$(stat -c %Y ft_containers)" ]
then
  echo "compiling ft... "
  clang++ $CFLAGS -D NAMESPACE=ft main.cpp -o ft_containers || exit
fi
if [  ! -x std_containers  ] || [ "$(stat -c %Y main.cpp)" -ge "$(stat -c %Y std_containers)" ]
then
  echo "compiling std... "
  clang++ $CFLAGS -D NAMESPACE=std main.cpp -o std_containers || exit
fi

rm .stdpipe .ftpipe 2>/dev/null
mkfifo .stdpipe .ftpipe

./fifodiff .stdpipe .ftpipe &
./std_containers $@ >> .stdpipe | ./ft_containers $@ >> .ftpipe &
cat << EOF""
...............................:*FV$V*:...........
..............................*V$$$$****:.........
.............................:***I$$III***:.......
..............................***I$$V***V$$V*:....
.............................***:**F$$$$$$$$$V*...
...........................:::*:::*F$$$$$$$$$$$V:.
...........................:**::::*$$$$$$$$$$$$$$:
.................:***********II****$$$$$$$$$$$$$$I
...............*FI**V$$$$$$$$$$$$V$$$$$$$$$$$$$$$V
.............:III***:::**V$$$$$$$$$$$$$$$$$$$$$$$*
.............:***II*:...*V$$V$$$$$$$$$$$$$$$$$$$$:
............:**..::*....**:*$$$$$$$$$$$$$$$$$$$$$:
:I$$:......:*:..........:****$$$$$$$$$$$$$$$$$$$V.
.FNNM:...................:***I$$$$$$$$$$$$$$$$$$*.
..*MNM*....................:.:$$$$$$$$$$$$$$$$$V..
...*MN$::****:........::******$$$$$$$$$$$$$$$$$:..
....*$**FVVV$*::::.:***********IV$$$$IF$$$$$$$*:..
...:::::*****::::::..::::************:::*****:....
MONKEY IS TESTING YOUR PROJECT...
EOF
wait
rm .stdpipe .ftpipe
pkill fifodiff
#sleep 10
#pkill ft_containers
#pkill std_containers


