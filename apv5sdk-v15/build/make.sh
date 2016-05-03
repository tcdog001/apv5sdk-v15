#!/bin/bash
#wangyu add for v5 mkfs.jffs2 shared library
if [ `env|grep -o -e 'LD_LIBRARY_PATH'` ];then
#       export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/emul/ia32-linux/usr/lib
        export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/lib32
else
#       export LD_LIBRARY_PATH=/emul/ia32-linux/usr/lib
        export LD_LIBRARY_PATH=/usr/lib32
fi

make_help()
{
echo "./make.sh  [Product]  [Os_type]  [Make_type] ..."
echo "           APV5       Double=d   Make_all =1"
echo "           APV6       Single=s   Make_wifi=2"
echo "           AZT                   Make_wtpd=3"
echo "           XH9344                Make_eth =4"
echo "           XJ9344                Make_kernel =5"
echo "                                 Make_wifi_wtpd_eth =6"
echo "                                 custom {kernel_build driver_build enet_build wtpd smalltool hostapd auteos capwap_split_fast ...}"
echo "For example:"
echo "           1. make.sh apv5 d 2"
echo "           2. make.sh AZT s custom smalltool auteos"
}

############################检查参数个数################################
if [ $# -lt 3 ];then
	make_help
	echo " "
	#echo "Error: The command needs 3 argument(s) at least..."
	echo -e "\033[31;49;1mError: The command needs 3 argument(s) at least...\033[39;49;0m"
	exit
fi
########################################################################

############################获取所有参数################################
NUMBER=$#
i=1
while [ $i -le $NUMBER ]
do
a[$i]=$1                  #将数组a[i]赋值为$1,即取得到第一个参数并将值存入a[1]
#echo ${a[$i]}
((i++))                   #数组后移一位,变为a[2]
shift                     #使用shift命令将参数后移一位,即此时的$1为第二个参数
done
########################################################################

##############################检查并赋值产品类型########################
case ${a[1]} in
  'apv5'|'APV5')
  command='make BOARD_TYPE=db12x BUILD_TYPE=jffs2 ETH_CONFIG=_f1e ';;
  'apv6'|'apv6')
  command='make BOARD_TYPE=amcc BUILD_TYPE=jffs2 ETH_CONFIG=_f1e ';;
  'azt'|'AZT')
  command='make BOARD_TYPE=db12x BUILD_TYPE=jffs2 ETH_CONFIG=_f1e AZT=1 ';;
  'xj9344'|'XJ9344')
  command='make BOARD_TYPE=db12x BUILD_TYPE=jffs2 ETH_CONFIG=_f1e AQ_TYPE=XJ9344 ';;
  'xh9344'|'XH9344')
  command='make BOARD_TYPE=db12x BUILD_TYPE=jffs2 ETH_CONFIG=_f1e AQ_TYPE=XH9344 ';;
  *)
  make_help
  echo " "
  #echo "Error: Invalid Product type! Please check param.";;
  echo -e "\033[31;49;1mError: Invalid Product type! Please check params.\033[39;49;0m"
  exit;;
esac
########################################################################

##############################检查并赋值单双系统########################
case ${a[2]} in
  'd'|'D')
  case ${a[1]} in
   'xj9344'|'XJ9344'|'xh9344'|'XH9344')  #XH,XJ不支持双系统
    make_help
    echo " "
	#echo "Error: XJ9344 or XH9344 just can be used SINGLE_OS. "
	echo -e "\033[31;49;1mError: XJ9344 or XH9344 just can be used SINGLE_OS.\033[39;49;0m"
	exit
  esac
  command=$command'OS_TYPE=DOUBLE_OS ';;
  's'|'S')
  command=$command'OS_TYPE=SINGLE_OS ';;
  *)
  make_help
  echo " "
  #echo "Error: Invalid os type! Please check param.";;
  echo -e "\033[31;49;1mError: Invalid os type! Please check param.\033[39;49;0m";;
esac
########################################################################

##############################赋值编译模块名称##########################
case ${a[3]} in
  '1') #make all
	echo $command
	$command;;
  '2') #make wifi
	command=$command'driver_build upgrade_tar_build'
	echo $command
	$command;;
  '3') #make wtpd
	command=$command'wtpd upgrade_tar_build'
	echo $command
	$command;;
  '4') #make eth
	command=$command'enet_build upgrade_tar_build'
	echo $command
	$command;;
  '5') #make kernel
	command=$command'kernel_build upgrade_tar_build'
	echo $command
	$command;;
  '6') #make wifi ethnet wtpd capwap
	command=$command'driver_build enet_build capwap_split_fast wtpd upgrade_tar_build'
	echo $command
	$command;;
  'custom') #user custom param
	if [ $NUMBER -lt 4 ];then #check params
	make_help
	echo " "
	#echo "Error: The command needs 4 argument(s) at least..."
	echo -e "\033[31;49;1mError: The command needs 4 argument(s) at least...\033[39;49;0m"
	exit
	fi
	j=1
	for((j=1;j<=i-3;j++))  #get custom params
	do
	command=$command' '${a[$j+3]}
	done
	command=$command' upgrade_tar_build'
	echo $command
	$command;;
  *)
  make_help
  echo " "
  #echo "Error: Invalid make type! Please check param.";;
  echo -e "\033[31;49;1mError: Invalid make type! Please check param.\033[39;49;0m";;
esac
########################################################################

