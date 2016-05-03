#!/usr/bin/haserl --upload-limit=1024 
<%in /usr/lib/web/libweb.sh %>

<%
    #get form
    local txpower="$FORM_txpower"
    local trans_rate="$FORM_rate"
    local trans_ratea="$FORM_ratea"
    local trans_rate_n_ht20ds="$FORM_rate_n_ht20ds"
    local trans_rate_n_ht40ds_s="$FORM_rate_n_ht40ds_s"
    local trans_rate_n_ht40ds="$FORM_rate_n_ht40ds"
    local channel="$FORM_channel"
    local channela="$FORM_channela"
    local psize="$FORM_psize"
    local pnum="$FORM_pnumber"
    local henable="${FORM_henable:-no}"
    local txoffset="$FORM_txoffset"
    local essid="$FORM_essid"
    local p_test="${FORM_test:-no}"  
    local txantenna="$FORM_tx"
    local extoffset="$FORM_extoffset"
    local cwmmode="$FORM_cwmmode"
    local shortgi="${FORM_shortgi:-0}"
    
    local wifi_name="$FORM_wifiname_x"
    local wifi_name_s="$FORM_wifiname"
    if [ "$wifi_name" == "wifi0" ];then
        tmpmode0="$DEVMODE_wifi0"
       if [ "$tmpmode0" == "bg" ];then
            mode="$FORM_mode"
       fi
       if [ "$tmpmode0" == "abg" ];then
            mode="$FORM_modea"
       fi     
       if [ "$tmpmode0" == "n" ];then
            mode="$FORM_modeng"
       fi     
       if [ "$tmpmode0" == "an" ];then
            mode="$FORM_moden"
       fi
    fi
    if [ "$wifi_name" == "wifi1" ];then   
        tmpmode1="$DEVMODE_wifi1"
       if [ "$tmpmode1" == "bg" ];then
            mode="$FORM_mode"
       fi     
       if [ "$tmpmode1" == "abg" ];then
            mode="$FORM_modea"
       fi     
       if [ "$tmpmode0" == "n" ];then
            mode="$FORM_modeng"
       fi     
       if [ "$tmpmode1" == "an" ];then
            mode="$FORM_moden"
       fi     
    fi
    

   
    #command path
    #SETTXPOWER=/usr/sbin/settxpower
    PACKET=/usr/sbin/packet
    #MYDAEMON=/usr/sbin/mydaemon
    IWLIST=/usr/sbin/iwlist

    #file path
    AP_MODE_FILE=/proc/sys/dev/wifi0/thinap
    SEND_SWITCH_FILE=/proc/sys/net/ath0/transmit_switch
    SEND_RATE_FILE=/proc/sys/net/ath0/transmit_rate
    TX_OFFSET_FILE=/jffs/tx-offset

    
    #function

    #
    #set_power( txpower )
    #
    set_power()
    {
    	local tx_offset=0
    	if [ -f /jffs/tx-offset ];then
    		tx_offset=`cat  /jffs/tx-offset`
    	fi
    	local txpower=$(($1-${tx_offset}))
    	if [ ${txpower} -lt 1 ];then
    		txpower=1
    	fi
    	${IWCONFIG} ath0 txpower ${txpower}
    	return $?
    }


case "$FORM_SERVLET" in

"·¢°ü")

 #write files
   if [ -f /tmp/wifi_name ];then
      wifi_name=`cat /tmp/wifi_name 2>/dev/null`
   else
      wifi_name="wifi0"
      echo "wifi0" > /tmp/wifi_name
   fi	
   echo "$essid" > /tmp/essid_wifi
  #  echo "$wifi_name" > /tmp/wifi_name
    mkdir /tmp/"$wifi_name"
    echo "$psize" > /tmp/psize
    echo "$pnum" > /tmp/pnumber
    echo "$p_test" > /tmp/p_test
    if [ "$wifi_name" == "wifi0" ];then
       echo "$mode" > /tmp/"$wifi_name"/mode_wifi0
       echo "$txpower" > /tmp/"$wifi_name"/txpower_wifi0
       echo "$trans_rate" > /tmp/"$wifi_name"/rate_wifi0
       echo "$channel" > /tmp/"$wifi_name"/channel_wifi0
       echo "$trans_ratea" > /tmp/"$wifi_name"/ratea_wifi0
       echo "$channela" > /tmp/"$wifi_name"/channela_wifi0
       echo "$txantenna" > /tmp/"$wifi_name"/txantenna_wifi0
       echo "$trans_rate_n_ht20ds" > /tmp/"$wifi_name"/rate_n_ht20ds_wifi0
       echo "$trans_rate_n_ht40ds_s" > /tmp/"$wifi_name"/rate_n_ht40ds_s_wifi0
       echo "$trans_rate_n_ht40ds" > /tmp/"$wifi_name"/rate_n_ht40ds_wifi0
       echo "$cwmmode" > /tmp/"$wifi_name"/cwmmode_wifi0
       echo "$extoffset" > /tmp/"$wifi_name"/extoffset_wifi0
       echo "$shortgi" > /tmp/"$wifi_name"/shortgi_wifi0    
    fi
    if [ "$wifi_name" == "wifi1" ];then
       echo "$mode" > /tmp/"$wifi_name"/mode_wifi1
       echo "$txpower" > /tmp/"$wifi_name"/txpower_wifi1
       echo "$trans_rate" > /tmp/"$wifi_name"/rate_wifi1
       echo "$channel" > /tmp/"$wifi_name"/channel_wifi1
       echo "$trans_ratea" > /tmp/"$wifi_name"/ratea_wifi1
       echo "$channela" > /tmp/"$wifi_name"/channela_wifi1
       echo "$txantenna" > /tmp/"$wifi_name"/txantenna_wifi1
       echo "$trans_rate_n_ht20ds" > /tmp/"$wifi_name"/rate_n_ht20ds_wifi1
       echo "$trans_rate_n_ht40ds_s" > /tmp/"$wifi_name"/rate_n_ht40ds_s_wifi1
       echo "$trans_rate_n_ht40ds" > /tmp/"$wifi_name"/rate_n_ht40ds_wifi1
       echo "$cwmmode" > /tmp/"$wifi_name"/cwmmode_wifi1
       echo "$extoffset" > /tmp/"$wifi_name"/extoffset_wifi1
       echo "$shortgi" > /tmp/"$wifi_name"/shortgi_wifi1
    fi
    
    ${KILLALL}  packet > /dev/null 2>&1
    #create ap interface and init
    RES=0
  
    echo 0 > "${AP_MODE_FILE}" 
    RES=$(($?+${RES}))
    ${WLANCONFIG} ath0 destroy > /dev/null 2>&1
    if [ "$p_test" == "no" ];then
    	 echo "$DEVICE_MODEL" | grep -q 'N'
       if [ $? -eq 0 ];then
       		${WLANCONFIG} ath0 create wlandev ${wifi_name} wlanmode ap > /dev/null 2>&1
       else
       		${WLANCONFIG} ath0 create wlandev ${wifi_name} wlanmode monitor > /dev/null 2>&1
       fi
    else
       ${WLANCONFIG} ath0 create wlandev ${wifi_name} wlanmode ap > /dev/null 2>&1
    fi

    ${IWCONFIG} ath0 essid ${essid} > /dev/null 2>&1
    
    if [ ${mode} != "11ng" ]&&[ ${mode} != "11na" ];then
    	${SETANTENNA} ${wifi_name} diversity 0 > /dev/null 2>&1
    	${SETANTENNA} ${wifi_name} tx ${txantenna} > /dev/null 2>&1
    fi

#    ${KILLALL} wtpd_check > /dev/null 2>&1
    /usr/sbin/stop_monitor >/dev/null 2>&1
   
    
    if [ ${henable} == "yes" ];then
       echo ${txoffset} > ${TX_OFFSET_FILE}
    fi
    if [ "$mode" == "11a" ];then
       ${IWCONFIG} ath0 channel 0 > /dev/null 2>&1
       ${IWPRIV} ath0 pureg 0 > /dev/null 2>&1
       ${IWPRIV} ath0 mode ${mode} > /dev/null 2>&1
       ${IWCONFIG} ath0 channel ${channela} > /dev/null 2>&1
    fi
    if [ "$mode" == "11g" ];then
    	 ${IWCONFIG} ath0 channel 0 > /dev/null 2>&1
    	 ${IWPRIV} ath0 mode ${mode} > /dev/null 2>&1
       ${IWCONFIG} ath0 channel ${channel} > /dev/null 2>&1
    fi
    
    if [ "$mode" == "11na" ];then
       ${IWCONFIG} ath0 channel 0 > /dev/null 2>&1
  
       if [ "$cwmmode" == "0" ];then
          $IWPRIV ath0 mode 11NAHT20 > /dev/null 2>&1
       fi
       if [ "$cwmmode" != "0" -a "$extoffset" == "0" ];then
          $IWPRIV ath0 mode 11NAHT20 > /dev/null 2>&1
       fi
       if [ "$cwmmode" != "0" -a "$extoffset" == "1" ];then
          $IWPRIV ath0 mode 11NAHT40PLUS > /dev/null 2>&1
       fi
       if [ "$cwmmode" != "0" -a "$extoffset" == "-1" ];then
          $IWPRIV ath0 mode 11NAHT40MINUS > /dev/null 2>&1
       fi    	 
       
       ${IWPRIV} ath0 cwmmode ${cwmmode} > /dev/null 2>&1
#       ${IWPRIV} ath0 extoffset ${extoffset} > /dev/null 2>&1
       ${IWPRIV} ath0 shortgi ${shortgi} > /dev/null 2>&1
       ${IWPRIV} ${wifi_name} txchainmask 1 > /dev/null 2>&1
       
       ${IWCONFIG} ath0 channel ${channela} > /dev/null 2>&1
    fi
    
    if [ "$mode" == "11ng" ];then
       ${IWCONFIG} ath0 channel 0 > /dev/null 2>&1 
       
       if [ "$cwmmode" == "0" ];then
          $IWPRIV ath0 mode 11NGHT20 > /dev/null 2>&1
       fi
       if [ "$cwmmode" != "0" -a "$extoffset" == "0" ];then
          $IWPRIV ath0 mode 11NGHT20 > /dev/null 2>&1
       fi
       if [ "$cwmmode" != "0" -a "$extoffset" == "1" ];then
          $IWPRIV ath0 mode 11NGHT40PLUS > /dev/null 2>&1
       fi
       if [ "$cwmmode" != "0" -a "$extoffset" == "-1" ];then
          $IWPRIV ath0 mode 11NGHT40MINUS > /dev/null 2>&1
       fi    	 
       
       ${IWPRIV} ath0 cwmmode ${cwmmode} > /dev/null 2>&1
#       ${IWPRIV} ath0 extoffset ${extoffset} > /dev/null 2>&1
       ${IWPRIV} ath0 shortgi ${shortgi} > /dev/null 2>&1
       ${IWPRIV} ${wifi_name} txchainmask 1 > /dev/null 2>&1
       
       ${IWCONFIG} ath0 channel ${channel} > /dev/null 2>&1
    fi
    
    ${IFCONFIG} ath0 up > /dev/null 2>&1
    
    if [ ${wifi_name} == "wifi0" ];then
       set_power ${txpower} 1>/dev/null 2>&1
    fi
    if [ ${wifi_name} == "wifi1" ];then
       ${IWCONFIG} ath0 txpower ${txpower} 1>/dev/null 2>&1
    fi
    
    ${IFCONFIG} ath0 down > /dev/null 2>&1

  if [ "$p_test" == "no" ];then
   if [ "$mode" == "11a" ];then
    ${IWCONFIG} ath0 rate ${trans_ratea}M 1>/dev/null 2>&1
   fi
   if [ "$mode" == "11g" ];then
    ${IWCONFIG} ath0 rate ${trans_rate}M 1>/dev/null 2>&1
   fi
   
   ${IFCONFIG} ath0 up > /dev/null 2>&1
   ${IFCONFIG} ath0 down > /dev/null 2>&1
   sleep 3
 else  
   if [ "$mode" == "11a" ];then
    ${IWCONFIG} ath0 rate ${trans_ratea}M 1>/dev/null 2>&1
   fi
   if [ "$mode" == "11g" ];then
    ${IWCONFIG} ath0 rate ${trans_rate}M 1>/dev/null 2>&1
   fi
 fi
    ${IFCONFIG} ath0 up > /dev/null 2>&1

    #start transmit
    if [ -f ${TX_OFFSET_FILE} ];then
    	amplifier=`cat ${TX_OFFSET_FILE}`
    else
    	amplifier="no amplifier"
    fi
  
  if [ "$p_test" == "no" ];then
    ${MYDAEMON} ${PACKET}  ${pnum} ${psize} ath0 > /dev/null 2>&1
  else
    $BRCTL addif $BRLAN ath0 > /dev/null 2>&1
  fi
    
    cgi_exec a50435de5dc4cacf33479630b043bc84.htm
    
  ;;

"Í£Ö¹")
    ${KILLALL}  packet > /dev/null 2>&1
    cgi_exec a50435de5dc4cacf33479630b043bc84.htm
    ;;
"WIFISpec")
   echo "$wifi_name_s" > /tmp/wifi_name
   cgi_exec a50435de5dc4cacf33479630b043bc84.htm
    ;;
esac
%>