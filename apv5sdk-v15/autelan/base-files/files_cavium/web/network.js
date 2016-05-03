function ip_c2i(ip)
{
	var ipAddr = ip;
	var numbers=new Object();
	var lastPtr=0;
	var ptr;
	var dotCount=0;
	var f_ip = "";

	for ( i=0; i<ipAddr.length; i++ )
	{
		var ch=ipAddr.charAt( i );
		if ( ch=='.' ) dotCount++;
	}

	for ( i=0; i<dotCount; i++ )
	{
		ptr=ipAddr.indexOf( '.', lastPtr );
		numbers[ i ]=ipAddr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	if(dotCount == 0)
		return f_ip;
	if(dotCount != 0)
		numbers[dotCount]=ipAddr.substring( lastPtr );
	
	for(i=0; i<=dotCount; i++)
	{
		f_ip += parseInt_dec(numbers[i]);
		if(i!= dotCount)
		{
			f_ip += "."
		}
	}
	return (f_ip);
}

function parseInt_dec(value)
{
	return parseInt(value,10);	
}

var SPACE=' ';
var TAB='\t';
function strim_delimiter(str, delimiter1, delimiter2)
{
	var str1 = str;
	var str2;
	var y = str1.length;
	var x= 0;

	while((x<y) && ((str1.charAt(x) == delimiter1) || (str1.charAt(x) == delimiter2)))
	{
		x++;
	}
	str1 = str1.substring(x, y);
	//alert("str1 is "+ str1);
	x = 0;
	y = str1.length;
	while((y>0) && ((str1.charAt(y-1) == delimiter1) || (str1.charAt(y -1) == delimiter2)))
	{
		y--;
	}
	str1 = str1.substring(0,y);
	//alert("str1 is "+ str1);
	return str1;
}

function aplit_str(obj1, obj2)
{

	var str = new Object;
	var lastPtr=0;
	var ptr = 0;
	var str1;
	var str2;
	str1 = obj1.value;
	str2 = obj2.value;
	
	ptr=str1.indexOf(SPACE, lastPtr );
	if(ptr == -1)
		ptr=str1.indexOf(TAB, lastPtr );
	if(ptr == -1)
	{
		str[0]=str1.substring(lastPtr);	
		str[1]="";
	}
	else
	{
		str[0]=str1.substring( lastPtr, ptr );
		lastPtr=ptr+1;
		str[1]=str1.substring( lastPtr );
	}
	obj1.value = str[0];
	obj2.value = str[1];
	//alert("obj1.value  is "+obj1.value + "obj2.value is " + obj2.value);
}

function regInput(obj, reg, inputStr)
{
	var docSel	= document.selection.createRange();
	if (docSel.parentElement().tagName != "INPUT")	return false;
	oSel = docSel.duplicate();
	oSel.text = "";
	var srcRange	= obj.createTextRange();
	oSel.setEndPoint("StartToStart", srcRange);
	var str = oSel.text + inputStr + srcRange.text.substr(oSel.text.length);
	if(str == 0) return false;	
	return reg.test(str);
	
}
function regInput1(obj, reg, inputStr)
{
	var docSel	= document.selection.createRange();
	if (docSel.parentElement().tagName != "INPUT")	return false;
	oSel = docSel.duplicate();
	oSel.text = "";
	var srcRange	= obj.createTextRange();
	oSel.setEndPoint("StartToStart", srcRange);
	var str = oSel.text + inputStr + srcRange.text.substr(oSel.text.length);
	if((str >= 255) || (str == 0))
	{
		//alert("输入值 " + str + "大于或等于255");
		return false;
	}
	
	return reg.test(str);
}

function regInput2(obj, reg, inputStr)
{
	var docSel	= document.selection.createRange();
	if (docSel.parentElement().tagName != "INPUT")	return false;
	oSel = docSel.duplicate();
	oSel.text = "";
	var srcRange	= obj.createTextRange();
	oSel.setEndPoint("StartToStart", srcRange);
	var str = oSel.text + inputStr + srcRange.text.substr(oSel.text.length);
	return reg.test(str);
}

function getsubnetfrom(LANip, LANmask)
{
	var strsubnetfrom= "";
		
	var ipnum;
	var masknum;
	var subnetfromnum;
	
	ipnum = Iptonum(LANip);
	masknum = Iptonum(LANmask);

	subnetfromnum = (ipnum & masknum);
	subnetfromnum += 1;

	strsubnetfrom = numtoIp(subnetfromnum);
	//alert(strsubnetfrom);
	return strsubnetfrom;
	
}

function getsubnetto(LANip, LANmask)
{
	var strsubnetto= "";
		
	var ipnum;
	var masknum;
	var subnettonum;
	
	ipnum = Iptonum(LANip);
	masknum = Iptonum(LANmask);

	subnettonum = (ipnum)|(~masknum);
	subnettonum -= 1;

	strsubnetto= numtoIp(subnettonum);
	//alert(strsubnetto);
	return strsubnetto;
}

function numtoIp(element1)
{
	var strip= "";
	var ip_obj = new Object();
	for( var i=0; i< 4; i++)
	{
		ip_obj[i]=0;
	}
	for(var i= 3; i >= 0; i-- )
	{
		ip_obj[i] = (element1 & 0xff);
		element1 = element1 >> 8;
	}

	strip = ip_obj[0]+"."+ ip_obj[1]+"."+ip_obj[2]+"."+ip_obj[3];
	return strip;
}

function get_ip_head(element1, num)
{
	var Addr = element1;
	var numbers = new Object();
	var lastPtr=0;
	var ptr = 0;
	var ip_head = "";
	if(Addr == "") return ip_head;
	for ( i=0; i<3; i++ )
	{
		ptr=Addr.indexOf( '.', lastPtr );
		if((ptr + i) == -1) return ip_head;
		numbers[ i ]=Addr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers[ 3 ]=Addr.substring( lastPtr );
	for( var i=0; i< num; i++ )
	{
		ip_head += numbers[i] + ".";
	}
	return ip_head;
}
function get_ip_tail(element1, num)
{
	var Addr = element1;
	var numbers = new Object();
	var lastPtr=0;
	var ptr = 0;
	var ip_head = "";
	if(Addr == "") return ip_head;
	for ( i=0; i<3; i++ )
	{
		ptr=Addr.indexOf( '.', lastPtr );
		if((ptr + i) == -1) return ip_head;
		numbers[ i ]=Addr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers[ 3 ]=Addr.substring( lastPtr );
	for( var i=num; i< 4; i++ )
	{
		ip_head += numbers[i];
	}
	return ip_head;
}
function format_ip(ip)
{
	var ipAddr = ip.value;
	var numbers=new Object();
	var lastPtr=0;
	var ptr;
	for ( i=0; i<3; i++ )
	{
		ptr=ipAddr.indexOf( '.', lastPtr );
		numbers[ i ]=ipAddr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers[ 3 ]=ipAddr.substring( lastPtr );
	if(numbers[ 3 ] != 1)
	{
		alert("开始IP地址必须是本网段的第一个地址\n\n       如X.X.X.1");
	}
	numbers[ 3 ] = 1;
	var f_ip = numbers[0]+"." + numbers[1]+"." + numbers[2]+"." + numbers[3];
	//alert(f_ip);
	return (f_ip);
}
var A_MASK = "255.0.0.0";
var B_MASK = "255.255.0.0";
var C_MASK = "255.255.255.0";

function get_mask(element)
{
	var number;
	var ipvalue = element;
	var i = 0;
	var lastPtr=0;
	var ptr;

	ptr=ipvalue.indexOf( '.', lastPtr );
	numbers=ipvalue.substring( lastPtr, ptr );
	if((numbers >= 1) && (numbers < 127))
	{
		return A_MASK;
	}
	else if((numbers >= 128) && (numbers < 192))
	{
		return B_MASK;
	}
	else if((numbers >= 192) && (numbers < 224))
	{
		return C_MASK;
	}
}
function range_power_2(ip1,ip2)
{
	var num_ip1 = 0;
	var num_ip2 = 0;
	var rang_ips = 0;

	num_ip1 = Iptonum(ip1);
	num_ip2 = Iptonum(ip2);
	
	rang_ips = (num_ip2 - num_ip1) + 2;
	//alert("out  if " + rang_ips);
	var power = rang_ips && !(rang_ips & (rang_ips - 1));
	//alert(power);
	if(power)
	{
		//alert("in if " + rang_ips);
		return true;
	}
	rang_ips = (num_ip2 - num_ip1) + 1;
		
	switch(rang_ips)
	{
		case 0xfe:
		case 0xfffe:
		case 0xfffffe:
		//alert(rang_ips);
		return true;
	}
	
	return false 
}
function checkNetAddr( element, errMsg ) {
    for ( var i=0; i<element.value.length; i++ )
    {
        var ch=element.value.charAt( i );
        if ( !( ( ch>='0' && ch<='9' ) || ch=='.' ) )
            return checkDomainName( element, errMsg );
    }

    return checkIPAddr( element, errMsg );
}
function checkSSID(element, errMsg)
{
    
	
	if ( element.value.length==0)
    {
        element.focus();
        element.select();
        alert("SSID不能为空");
        return false;

    }
    else if(!isSSIDValid(element.value))
    {   
        element.focus();
        element.select();
        //alert( errMsg );

        return false;
    }
    return true;

}
function checkMASK( element, errMsg ) {

    var tmp_Mask;
    if ( element.value.length==0)
    {
        element.focus();
        element.select();
        alert("Mask地址为空");
        return false;

    }
    if(!ismaskValid( element.value ))
    {
        element.focus();
        element.select();
        //alert( errMsg );

        return false;
    }
    return true;
}

function checkIPAddr( element, errMsg ) 
{

    if (element.value.length==0)
    {
        element.focus();
        element.select();
        alert(errMsg+"IP地址为空");
        return false;

    }
    else if(!isIPAddrValid( element.value ) )
    {
        element.focus();
        element.select();
        //alert( errMsg );

        return false;
    }

    return true;
}

function checkGW( element, errMsg ) 
{
	
    if ( element.value.length==0)
    {
        element.focus();
        element.select();
        alert("网关地址为空");
        return false;

    }
    if(!isIPAddrValid( element.value ) )
    {
        element.focus();
        element.select();
        //alert( errMsg );

        return false;
    }

    return true;
}
function checkIPAddr_weas( element, errMsg ) 
{

    if ( element.value.length==0)
    {
        element.focus();
        element.select();
        alert("IP地址为空");
        return false;

    }
    else if(!isIPAddrValid_weas( element.value ) )
    {
        element.focus();
        element.select();
        //alert( errMsg );

        return false;
    }

    return true;
}
function isIPAddrValid_weas( ipAddr ) {
    var dotCount=0;
    var exp_str="\n"+"\n"+"输入值请参考以下格式:192.168.1.1";

    // only digits and '.' allowed
	for ( i=0; i<ipAddr.length; i++ )
	{
		var ch=ipAddr.charAt( i );
		if ( ( ch<'0' || ch>'9' ) && ch!='.' ) 
		{
			var err_msg="第'"+(i+1)+"'个输入错误"+exp_str;
			alert(err_msg) ;
			return false;
		}

		if ( ch=='.' ) dotCount++;
	}

	// only 3 dots
	if ( dotCount!=3 ) 
	{
		var err_msg="";
		if(dotCount < 3)
		{
			err_msg="输入不完整,只有'"+(dotCount+1)+"'个小节"+exp_str;
		}
		else 
		{
			err_msg="输入超过4个小节"+exp_str;
		}
		alert(err_msg);
		return false;
	}


	var numbers=new Object();
	var lastPtr=0;
	var ptr;
	for ( i=0; i<3; i++ )
	{
		ptr=ipAddr.indexOf( '.', lastPtr );
		numbers[ i ]=ipAddr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers[ 3 ]=ipAddr.substring( lastPtr );

	for ( i=0; i<4; i++ )
	{
		if ( numbers[ i ].length==0 ) 
		{
			var err_msg="第'"+(i+1)+"'小节为空"+exp_str;
			alert(err_msg);
			return false;
			
		}
		if ( numbers[ i ] > 255 ) 
		{
			var ip_tem_len=i+1;
			var out_err_msg="第"+ip_tem_len+"小节输入大于或等于255"+exp_str;
			alert(out_err_msg);	
			return false;
		}
	}
	
	if((numbers[ 0 ] <1) || (numbers[ 0 ] >223))
	{
		var out_err_msg="第一节介于'1'~'223'之间"+exp_str;
		alert(out_err_msg);
		return false;
	}/*
        if(numbers[ 0 ] == 127)
        {
                var out_err_msg="127保留用于本地还回地址,请选择其他值"+exp_str;
                alert(out_err_msg);
                return false;
        }
	*/
	

// no 255.255.255.255
	if ( numbers[ 0 ]==0 && numbers[ 1 ]==0 && numbers[ 2 ]==0 && numbers[ 3 ]==0 ) 	
		{
			alert("输入不能全为零");
			return false;
		}
	if ( numbers[ 0 ]==255 && numbers[ 1 ]==255 && numbers[ 2 ]==255 && numbers[ 3 ]==255 ) 	
		{
			 alert("输入不能全为255");
			return false;
		}
	return true;

}


function check_sn( element, errMsg ) {
    if ( element.value.length==0)
    {
        element.focus();
        element.select();
        alert("序列号为空");
        return false;
    }
	if(element.value.length > 32)
	{
	    element.focus();
        element.select();
		alert("序列号长度不能超过32个字符");
		return false;
     
	}
    if(!isSNValid( element.value ) )
    {
        element.focus();
        element.select();
	    return false;
    }
    return true;
}
function check_number(period)
{
	    for ( i=0; i<period.length; i++ )
        {
				
                var ch=period.charAt( i );
                if (( ch<'0') || (ch>'9' ))
                {
                        var err_msg="第'"+(i+1)+"'个输入值不是数字" +"\n";
                        alert(err_msg) ;
                        return false;
                }
		}
		return true;
}
function isSNValid( SN )
{
        for ( i=0; i<SN.length; i++ )
        {
				var exp_str = "输入请参考以下格式‘32a4b5c’";
                var ch=SN.charAt( i );
                if ((( ch<'0') || (ch>'9' )) && 
					((ch >'f' )||(ch <'a'))  && 
					((ch >'F' )||(ch <'A')))
                {
                        var err_msg="第'"+(i+1)+"'个输入值不是16进制数字" +"\n"+ exp_str;
                        alert(err_msg) ;
                        return false;
                }

        }
	return true;
}


function checkPort( element, errMsg ) {
    if ( element.value.length==0)
    {
	element.focus();
        element.select();
	alert("端口号空");
	return false;
    } 
    if(!isIPPortValid( element.value ) )
    {
        element.focus();
        element.select();
        return false;
    }
    return true;
}
function   unicode_length(str)   
{   
	ulen   =   str.length;
	for   (i=0;i<str.length;i++)   
	{   
		if(str.charCodeAt(i)>255)   ulen++;   
	}   
	return   ulen;   
}

function isSSIDValid(SSID)
{
	if(unicode_length(SSID) > 32)
	{
		alert("SSID不能超过32个字符");
		return false;
	}

	var case_SSID = ""
	var bad_ssid = "ANY";
	case_SSID = SSID.toUpperCase();
	//alert("case_SSID ="+case_SSID+"bad_SSID = "+bad_ssid);
	
	if(case_SSID === bad_ssid)
	{
		alert("SSID 不能设为 'any aNy aNY anY Any AnY ANy ANY'其中之一");
		return false;
	}
	for ( i=0; i<SSID.length; i++ )
        {
                var ch=SSID.charAt( i );
                if ( ( ch ==' ' )||
					 ( ch =='?' )||
					 ( ch =='"' )||
					 ( ch ==';' )||
					 ( ch.charCodeAt() == 0x09))
                {
                        var err_msg="第'"+(i+1)+"'个输入"+(ch)+"错误";
                        alert(err_msg) ;
                        return false;
                }
        }

	return true;
}
function isIPPortValid( Port )
{
	for ( i=0; i<Port.length; i++ )
        {
                var ch=Port.charAt( i );
                if ( ( ch<'0') || (ch>'9' )) 
                {
                        var err_msg="第'"+(i+1)+"'个输入值不是数字";
                        alert(err_msg) ;
                        return false;
                }

        }
	if((Port < 1024) || (Port >65535))
	{
		var err_msg="端口值介于'1024'~'65535'之间";
		alert(err_msg);
		return false;
	}
        if(Port == 9001)
        {
                var err_msg="9001端口已经被系统占用,请输入其它值";
                alert(err_msg);
		return false;
        }

	return true;
	
}

function checkDomainName( element, errMsg ) {
    if ( element.value.length>0 && !isDomainNameValid( element.value ) )
    {
        element.focus();
        element.select();
        alert( errMsg );

        return false;
    }
    return true;
}

function checkMacAddr( element, errMsg ) {
    if ( element.value.length>0 && !isMacAddrValid( element.value ) )
    {
        element.focus();
        element.select();
        //alert( errMsg );

        return false;
    }
    return true;
}
function checkIpMask(element1,element2)
{
    var exp_str="\n"+"\n"+"输入请参考以下格式:192.168.1.1";
	var Addr =element1.value;
    var Mask   =element2.value;
	var numbers_ip=new Object();
	var numbers_mask = new Object();
    var lastPtr=0;
    var ptr;
    for ( i=0; i<3; i++ )
    {
		ptr=Addr.indexOf( '.', lastPtr );
		numbers_ip[ i ]=Addr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers_ip[ 3 ]=Addr.substring( lastPtr );
	lastPtr=0;
	ptr=0;

	for ( i=0; i<3; i++ )
    {
		ptr=Mask.indexOf( '.', lastPtr );
		numbers_mask[ i ]=Mask.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers_mask[ 3 ]=Mask.substring( lastPtr );
	
	for(i=0; i<4; i++ )
	{
		//alert(numbers_ip[i]);
		//alert(numbers_mask[i])
	} 
	var tmp_host_0 = 0;
	var tmp_host_1 = 0;
	var tmp_net_1 = 0;
	for(i=0; i<4;i++)
	{
		if((numbers_ip[i] | numbers_mask[i]) == numbers_mask[i])
		{
			tmp_host_0++;
		}
		if((numbers_ip[i] | numbers_mask[i]) == 0xff)
		{
			tmp_host_1++;
		}
		if((numbers_ip[i] & numbers_mask[i]) == 0)
		{
			tmp_net_1++;
		}
	}
	//alert(tmp_host_0);
	//alert(tmp_host_1);
	//alert(tmp_net_1);	
	if(tmp_host_0 == 4)
	{
		var out_err_msg="  IP地址主机部分全为零"+exp_str;
		alert(out_err_msg);
        element1.focus();
        element1.select();
		
		return false;
	}
	if(tmp_host_1 == 4)
	{
		var out_err_msg="  IP地址主机部分全为1"+exp_str;
		alert(out_err_msg);
        element1.focus();
		element1.select();

		return false;	
	}
	if(tmp_net_1 == 4)
	{
		var out_err_msg="  IP地址网络部分全为零"+exp_str;
		element1.focus();
        	element1.select();
		alert(out_err_msg);
		return false;
	}
	return true;
}

function checkIpMask_gw(element1,element2,element3)
{
	var exp_str="\n"+"\n"+"输入请参考以下格式:192.168.1.1";
	var ipAddr =element3.value;
	var Mask   =element2.value;
	var gw_Addr = element1.value;
	var numbers_ip=new Object();
	var numbers_mask = new Object();
	var numbers_gw = new Object();
	

	var lastPtr=0;
	var ptr = 0;

	for ( i=0; i<3; i++ )
	{
		ptr=ipAddr.indexOf( '.', lastPtr );
		numbers_ip[ i ]=ipAddr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers_ip[ 3 ]=ipAddr.substring( lastPtr );
	lastPtr=0;
	ptr=0;

	for ( i=0; i<3; i++ )
	{
		ptr=Mask.indexOf( '.', lastPtr );
		numbers_mask[ i ]=Mask.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers_mask[ 3 ]=Mask.substring( lastPtr );
	lastPtr=0;
	ptr=0;
	for ( i=0; i<3; i++ )
	{
		ptr=gw_Addr.indexOf( '.', lastPtr );
		numbers_gw[ i ]=gw_Addr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers_gw[ 3 ]=gw_Addr.substring( lastPtr );
	for(i=0; i<4; i++ )
	{
		//alert(numbers_ip[i]);
		//alert(numbers_mask[i])
	} 
	var tmp_host_0 = 0;
	var tmp_host_1 = 0;
	var tmp_net_1 = 0;
	for(i=0; i<4;i++)
	{
		if((numbers_gw[i] | numbers_mask[i]) == numbers_mask[i])
		{
			tmp_host_0++;
		}
		if((numbers_gw[i] | numbers_mask[i]) == 0xff)
		{
			tmp_host_1++;
		}
		if((numbers_gw[i] & numbers_mask[i]) == 0)
		{
			tmp_net_1++;
		}
	}
	//alert(tmp_host_0);
	//alert(tmp_host_1);
	//alert(tmp_net_1);	
	if(tmp_host_0 == 4)
	{
		var out_err_msg="  网关地址主机部分全为零"+exp_str;
		alert(out_err_msg);
        element1.focus();
        element1.select();
		
		return false;
	}
	if(tmp_host_1 == 4)
	{
		var out_err_msg="  网关地址主机部分全为1"+exp_str;
		alert(out_err_msg);
        	element1.focus();
		element1.select();

		return false;	
	}
	if(tmp_net_1 == 4)
	{
		var out_err_msg="  网关地址网络部分全为0"+exp_str;
		element1.focus();
        	element1.select();
		alert(out_err_msg);
		return false;
	}
	var gw_long = 0;
	var ip_long = 0;
	var mask_long = 0;
	var num = 0;
	for(var i = 0; i < 4; i++)
	{
		num = str_to_num(numbers_gw[i]);
		gw_long = gw_long*256 + num;
		
	}
	
	
	num = 0;
	for(var i = 0; i < 4; i++)
	{
		num = str_to_num(numbers_ip[i]);
		ip_long = ip_long*256 + num;
		
	}
	
	
	num = 0;
	for(var i = 0; i < 4; i++)
	{
		num = str_to_num(numbers_mask[i]);
		mask_long = mask_long*256 + num;
	}
	
	var gw_and_mask = 0;
	var ip_and_mask = 0;
	/*
	alert("gw_long = "+ gw_long);
	alert("ip_long ="+ ip_long);
	alert("mask_long= " + mask_long);
	alert("gw_and_mask = "+((gw_long & mask_long)+0xffffffff));
	alert("ip_and_mask = "+((ip_long & mask_long)+0xffffffff));
	*/
	gw_and_mask =(gw_long & mask_long)+0xffffffff;
	ip_and_mask =(ip_long & mask_long)+0xffffffff;
	/*	
	alert(gw_and_mask);
	alert(ip_and_mask);
	*/
	if(gw_and_mask != ip_and_mask)
	{
		alert("网关地址和IP地址不在同一子网");
		return false;
	}
	return true;
}

function str_to_num(str)
{
	var i = 0;
	var len = str.length;
	var num = 0;
	//alert("in str_to_num str = "+ str);
	for(i = 0; i < len; i++)
	{
		num = num*10 + (str.charCodeAt(i) - 48);
	}
	//alert("in str_to_num num = "+ num);
	return num;
}

function check2IpMask(element1,element2,element3)
{
	var exp_str="\n"+"\n"+"输入请参考以下格式:192.168.1.1";
	var ipAddr =element3;
	var Mask   =element2;
	var gw_Addr = element1;
	//alert(ipAddr);alert(Mask);alert(gw_Addr);
	var numbers_ip=new Object();
	var numbers_mask = new Object();
	var numbers_gw = new Object();
	

	var lastPtr=0;
	var ptr = 0;

	for ( i=0; i<3; i++ )
	{
		ptr=ipAddr.indexOf( '.', lastPtr );
		numbers_ip[ i ]=ipAddr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers_ip[ 3 ]=ipAddr.substring( lastPtr );
	lastPtr=0;
	ptr=0;

	for ( i=0; i<3; i++ )
	{
		ptr=Mask.indexOf( '.', lastPtr );
		numbers_mask[ i ]=Mask.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers_mask[ 3 ]=Mask.substring( lastPtr );
	lastPtr=0;
	ptr=0;
	for ( i=0; i<3; i++ )
	{
		ptr=gw_Addr.indexOf( '.', lastPtr );
		numbers_gw[ i ]=gw_Addr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers_gw[ 3 ]=gw_Addr.substring( lastPtr );
	/*
	for(i=0; i<4; i++ )
	{
		//alert(numbers_ip[i]);
		//alert(numbers_mask[i])
	} 
	*/
	var gw_long = 0;
	var ip_long = 0;
	var mask_long = 0;
	var num = 0;
	for(var i = 0; i < 4; i++)
	{
		num = str_to_num(numbers_gw[i]);
		gw_long = gw_long*256 + num;
		
	}
	
	
	num = 0;
	for(var i = 0; i < 4; i++)
	{
		num = str_to_num(numbers_ip[i]);
		ip_long = ip_long*256 + num;
		
	}
	
	
	num = 0;
	for(var i = 0; i < 4; i++)
	{
		num = str_to_num(numbers_mask[i]);
		mask_long = mask_long*256 + num;
	}
	
	var gw_and_mask = 0;
	var ip_and_mask = 0;
	/*
	alert("gw_long = "+ gw_long);
	alert("ip_long ="+ ip_long);
	alert("mask_long= " + mask_long);
	alert("gw_and_mask = "+((gw_long & mask_long)+0xFFFFFFFF));
	alert("ip_and_mask = "+((ip_long & mask_long)+0xFFFFFFFF));
	*/
	gw_and_mask =(gw_long & mask_long) + 0xFFFFFFFF;
	ip_and_mask =(ip_long & mask_long)+ 0xFFFFFFFF;
		
	//alert(gw_and_mask);
	//alert(ip_and_mask);
	
	if(gw_and_mask != ip_and_mask)
	{
		//alert(errMsg+ "IP地址不再同一子网");
		return false;
	}
	return true;
}
function Iptonum(element1)
{
	var Addr = element1;
	var numbers = new Object();
	var lastPtr=0;
	var ptr = 0;
	if(Addr == "") return 0;
	for ( i=0; i<3; i++ )
	{
		ptr=Addr.indexOf( '.', lastPtr );
		numbers[ i ]=Addr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers[ 3 ]=Addr.substring( lastPtr );
	
	var num_long = 0;
	
	var num = 0;
	for(var i = 0; i < 4; i++)
	{
		num = str_to_num(numbers[i]);
		num_long = num_long*256 + num;
		
	}
	//alert("num_long= "+ num_long);
	return num_long;
}
function compare_ip(ip1,ip2)
{
	var num_ip1 = 0;
	var num_ip2 = 0;
	
	num_ip1 = Iptonum(ip1);
	num_ip2 = Iptonum(ip2);
	//alert("num_ip1= "+num_ip1);
	//alert("num_ip2= "+num_ip2);
	if(num_ip1 > num_ip2 ) 	return (1);
	else if(num_ip1 == num_ip2) return (0);
	else if(num_ip1 < num_ip2) return (-1);
	
}
function isIPAddrValid( ipAddr ) {
    var dotCount=0;
    var exp_str="\n"+"\n"+"输入请参考以下格式:192.168.1.1";

    // only digits and '.' allowed
	for ( i=0; i<ipAddr.length; i++ )
	{
		var ch=ipAddr.charAt( i );
		if ( ( ch<'0' || ch>'9' ) && ch!='.' ) 
		{
			var err_msg="第'"+(i+1)+"'个输入值错误"+exp_str;
			alert(err_msg) ;
			return false;
		}

		if ( ch=='.' ) dotCount++;
	}

	// only 3 dots
	if ( dotCount!=3 ) 
	{
		var err_msg="";
		if(dotCount < 3)
		{
			err_msg="输入不完整,只有'"+(dotCount+1)+"'个小节"+exp_str;
		}
		else 
		{
			err_msg="输入超过4个小节"+exp_str;
		}
		alert(err_msg);
		return false;
	}


	var numbers=new Object();
	var lastPtr=0;
	var ptr;
	for ( i=0; i<3; i++ )
	{
		ptr=ipAddr.indexOf( '.', lastPtr );
		numbers[ i ]=ipAddr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
	}
	numbers[ 3 ]=ipAddr.substring( lastPtr );

	for ( i=0; i<4; i++ )
	{
		if ( numbers[ i ].length==0 ) 
		{
			var err_msg="第'"+(i+1)+"'小节为空"+exp_str;
			alert(err_msg);
			return false;
			
		}
		if ( numbers[ i ] > 255 ) 
		{
			var ip_tem_len=i+1;
			var out_err_msg="第"+ip_tem_len+"小节输入大于或等于255"+exp_str;
			alert(out_err_msg);	
			return false;
		}
	}
	
	if((numbers[ 0 ] <1) || (numbers[ 0 ] >223))
	{
		var out_err_msg="第一节介于'1'~'223'之间"+exp_str;
		alert(out_err_msg);
		return false;
	}
        if(numbers[ 0 ] == 127)
        {
                var out_err_msg="127保留用于本地还回地址,请选择其他值"+exp_str;
                alert(out_err_msg);
                return false;
        }
	
	

// no 255.255.255.255
	if ( numbers[ 0 ]==0 && numbers[ 1 ]==0 && numbers[ 2 ]==0 && numbers[ 3 ]==0 ) 	
		{
			alert("输入不能全为零");
			return false;
		}
	if ( numbers[ 0 ]==255 && numbers[ 1 ]==255 && numbers[ 2 ]==255 && numbers[ 3 ]==255 ) 	
		{
			 alert("输入不能全为255");
			return false;
		}
	return true;

}

function get_MOD(num1,num2)
{
	var tmp_num = 0;
	tmp_num = ((num1 - (num1 % num2)) / num2);
	return tmp_num;
}	
function ismaskValid( ipAddr ) 
{
    var dotCount=0;
    var exp_str="\n"+"\n"+"输入请参考以下格式:255.255.255.0";
    // only digits and '.' allowed
	for ( i=0; i<ipAddr.length; i++ )
	{
		var ch=ipAddr.charAt( i );
		if ( ( ch<'0' || ch>'9' ) && ch!='.' ) 
		{
			var err_msg="第'"+(i+1)+"'个输入值错误"+exp_str;
			alert(err_msg) ;
			return false;
		}

		if ( ch=='.' ) dotCount++;
	}

	// only 3 dots
	if ( dotCount!=3 ) 
	{
		var err_msg="";
		if(dotCount < 3)
		{
			err_msg="输入不完整,只有'"+(dotCount+1)+"'个小节"+exp_str;
		}
		else 
		{
			err_msg="输入超过4个小节"+exp_str;
		}
		alert(err_msg);
		return false;
	}

	
	var numbers=new Object();
	var lastPtr=0;
	var ptr;
	for ( i=0; i<3; i++ )
	{
		ptr=ipAddr.indexOf( '.', lastPtr );
		numbers[ i ]=ipAddr.substring( lastPtr, ptr );
		lastPtr=ptr+1;
		
	}
	numbers[ 3 ]=ipAddr.substring( lastPtr );
	
	if(numbers[ 0 ] == 128)
	{
		var out_err_msg = "无效的子网掩码"+exp_str;
		alert(out_err_msg);
		return false;
	}	
	for ( i=0; i<4; i++ )
	{
		if ( numbers[ i ].length==0 ) 
		{
			var err_msg="第'"+(i+1)+"'小节为空"+exp_str;
			alert(err_msg);
			return false;
			
		}
		
		if ( numbers[ i ]>255 ) 
		{
			var ip_tem_len=i+1;
			var out_err_msg="第"+ip_tem_len+"小节输入大于255"+exp_str;
			alert(out_err_msg);	
			return false;
		}
	}
	var  tmp_value =new Object;
	var  mask_bit = "mask_bit=";
	
	for(var i = 0;i < 32; i++)
	{
		tmp_value[i] = 0;
	}
	
	for(var j = 0; j < 4; j++)
	{
	
		for (var i = 0; i < 8; i++)
		{	
			tmp_value[31-(j*8+i)] = (numbers[3-j] >>i ) & (0x01);
			//mask_bit = mask_bit + tmp_value[31-(j*8+i)];
 		}
		//alert(mask_bit);
	}
	
	var tmp_str = "";
	var plus0 = 33;
	var plus1 = 0;
	
	if (tmp_value[0] == 0)
	{
		
		var out_err_msg="第1小节输入错误,子网掩码必须连续"+exp_str;	
		alert(out_err_msg);
		return false;
	}
	tmp_str = tmp_str + tmp_value[0];

	for(var ii = 0; ii < 32; ii++)
	{
		tmp_str = tmp_str + tmp_value[ii];
		
		if(tmp_value[ii] == 0)
		{	
			plus0 = ii;
			//alert(ii);
		}
		
		if(tmp_value[ii] == 1)
		{
			plus1 = ii;
			
			if(plus1 > plus0)
			{
				var out_err_msg="第"+(get_MOD(ii,8) + 1)+"小节输入错误,子网掩码必须连续"+exp_str;	
				//alert(tmp_str);
				alert(out_err_msg);
				return false;
			}
			
		}
		
	}
	if(plus1 + 1 == 32)
	{
		alert("输入不能全为255");
		//alert(tmp_str);
		return false;

	}
	return true;
}

function isDomainNameValid( domainName ) {
if ( domainName.charAt( domainName.length-1 )=='.' ) return false;

var letterExist=false;
for ( var i=0; i<domainName.length; i++ )
{
var ch=domainName.charAt( i );
if ( ch>='a' && ch<='z' )
{
    letterExist=true; break;
}
}
if ( !letterExist ) return false;

var ptr=0;
while ( ptr<domainName.length )
{
var dotPtr=domainName.indexOf( '.', ptr );
if ( dotPtr==-1 ) dotPtr=domainName.length;

// JPNIC worships 1 character subdomain
if ( dotPtr==ptr ) return false;
//	if( dotPtr==ptr+1 || dotPtr==ptr ) return false;

var subdomain=domainName.substring( ptr, dotPtr );
var ch=subdomain.charAt( 0 );

if ( !( ch>='a' && ch<='z' ) && !( ch>='0' && ch<='9' ) ) return false;
if ( subdomain.indexOf( '--' )!=-1 ) return false;
for ( i=0; i<subdomain.length; i++ )
{
    var ch=subdomain.charAt( i );
    if ( !( ( ch>='a' && ch<='z' ) || ( ch>='0' && ch<='9' ) || ch=='-' ) ) return false;
}

ptr=dotPtr+1;
}

return true;
}

/*检查MAC地址的有效性*/
function isMacAddrValid( macAddr ) 
{
var exp_str="\n"+"\n"+"输入请参照以下格式: 00:0B:C0:50:23:20";
var special_addr = "00:00:00:00:00:00";

if ( macAddr.length!=17 ) 
{
	var in_mac_len=macAddr.length;
	//var exp_str="输入请参照以下格式: 00-90-4B-00-58-70";
	var out_msg="你输入了"+in_mac_len+"个字符"+exp_str;	
	alert(out_msg);
	return false;
}
	
	
    for ( var i=0; i<17; i++ )
    {
        var ch=macAddr.charAt( i );

        if ( ( ( i-2 )%3 )==0 )
        {
            //if ( (ch!=':') && (ch!='-')) 
			if((ch !=':'))
			{
				var sep_pos=( i-2 )/3+1; 
				var err_msg="第"+sep_pos+"个分割符不是 ':'"+exp_str;
				alert(err_msg);
				return false;
			}
        } else if ( !( ( ch>='0' && ch<='9' ) || ( ch>='A' && ch<='F' ) || ( ch>='a' && ch<='f' ) ) ) 
		{
			var err_msg="输入字符'"+ch+"'错误"+exp_str;
			alert(err_msg);
			return false;
		}
    }
	if(macAddr == special_addr)
	{
		var err_msg = "不能输入全零MAC地址" + exp_str;
		alert(err_msg);
		return false;
	}
	var MAC_num=new Object();
	var MAC0;
	var lastPtr=0;
	var ptr;
	var len = 0;
	for(var i = 0; i < 17; i++)
	{
		MAC_num[i] = 0;
	}
	ptr=macAddr.indexOf( ':', lastPtr );
	MAC0 = macAddr.substring( lastPtr, ptr );
	
	
	len = MAC0.length;
	var a_a = "a";
	var a_A = "A";
	var a_0 = "0";
	for(var i = 0; i < len; i++)
	{
		if(MAC0.charAt(i)>'a')
		{
			MAC_num[i] = MAC0.charCodeAt(i) - 97 + 10;
		}
		else if(MAC0.charAt(i)>'A')
		{
			MAC_num[i] = MAC0.charCodeAt(i) - 65 + 10;
		}
		else if(MAC0.charAt(i)>'0')
		{
			MAC_num[i] = MAC0.charCodeAt(i) - 48;
		}

		//alert("MAC_num="+(MAC_num[i]));
	}
	MAC0 =0;
	
	for(var i = 0; i < len ; i++)
	{
		MAC0 = MAC0*16 + MAC_num[i];
	}
	
	//alert("第二位="+(MAC0 & 0x01));
    
	if((MAC0 & 0x01) != 0)
	{
		alert("不能输入广播MAC地址！");
		return false;
	}
	var ddd="第一位="+ (MAC0 & 0x02);
	//alert(ddd);
	if((MAC0 & 0x02) != 0) 
	{
		alert("MAC地址非法！");
		return false;
	}
	
	return true;
}


/*
 * A JavaScript implementation of the RSA Data Security, Inc. MD5 Message
 * Digest Algorithm, as defined in RFC 1321.
 * Copyright (C) Paul Johnston 1999 - 2000.
 * Updated by Greg Holt 2000 - 2001.
 * See http://pajhome.org.uk/site/legal.html for details.
 */

/*
 * Convert a 32-bit number to a hex string with ls-byte first
 */
var hex_chr = "0123456789abcdef";
function rhex(num)
{
  str = "";
  for(j = 0; j <= 3; j++)
    str += hex_chr.charAt((num >> (j * 8 + 4)) & 0x0F) +
           hex_chr.charAt((num >> (j * 8)) & 0x0F);
  return str;
}

/*
 * Convert a string to a sequence of 16-word blocks, stored as an array.
 * Append padding bits and the length, as described in the MD5 standard.
 */
function str2blks_MD5(str)
{
  nblk = ((str.length + 8) >> 6) + 1;
  blks = new Array(nblk * 16);
  for(i = 0; i < nblk * 16; i++) blks[i] = 0;
  for(i = 0; i < str.length; i++)
    blks[i >> 2] |= str.charCodeAt(i) << ((i % 4) * 8);
  blks[i >> 2] |= 0x80 << ((i % 4) * 8);
  blks[nblk * 16 - 2] = str.length * 8;
  return blks;
}

/*
 * Add integers, wrapping at 2^32. This uses 16-bit operations internally 
 * to work around bugs in some JS interpreters.
 */
function add(x, y)
{
  var lsw = (x & 0xFFFF) + (y & 0xFFFF);
  var msw = (x >> 16) + (y >> 16) + (lsw >> 16);
  return (msw << 16) | (lsw & 0xFFFF);
}

/*
 * Bitwise rotate a 32-bit number to the left
 */
function rol(num, cnt)
{
  return (num << cnt) | (num >>> (32 - cnt));
}

/*
 * These functions implement the basic operation for each round of the
 * algorithm.
 */
function cmn(q, a, b, x, s, t)
{
  return add(rol(add(add(a, q), add(x, t)), s), b);
}
function ff(a, b, c, d, x, s, t)
{
  return cmn((b & c) | ((~b) & d), a, b, x, s, t);
}
function gg(a, b, c, d, x, s, t)
{
  return cmn((b & d) | (c & (~d)), a, b, x, s, t);
}
function hh(a, b, c, d, x, s, t)
{
  return cmn(b ^ c ^ d, a, b, x, s, t);
}
function ii(a, b, c, d, x, s, t)
{
  return cmn(c ^ (b | (~d)), a, b, x, s, t);
}

/*
 * Take a string and return the hex representation of its MD5.
  MD5签名算法，对输入的启动密钥进行签名，生成 16字节密钥
 */
function MD5(str)
{
  x = str2blks_MD5(str);
  var a =  1732584193;
  var b = -271733879;
  var c = -1732584194;
  var d =  271733878;
 
  for(i = 0; i < x.length; i += 16)
  {
    var olda = a;
    var oldb = b;
    var oldc = c;
    var oldd = d;

    a = ff(a, b, c, d, x[i+ 0], 7 , -680876936);
    d = ff(d, a, b, c, x[i+ 1], 12, -389564586);
    c = ff(c, d, a, b, x[i+ 2], 17,  606105819);
    b = ff(b, c, d, a, x[i+ 3], 22, -1044525330);
    a = ff(a, b, c, d, x[i+ 4], 7 , -176418897);
    d = ff(d, a, b, c, x[i+ 5], 12,  1200080426);
    c = ff(c, d, a, b, x[i+ 6], 17, -1473231341);
    b = ff(b, c, d, a, x[i+ 7], 22, -45705983);
    a = ff(a, b, c, d, x[i+ 8], 7 ,  1770035416);
    d = ff(d, a, b, c, x[i+ 9], 12, -1958414417);
    c = ff(c, d, a, b, x[i+10], 17, -42063);
    b = ff(b, c, d, a, x[i+11], 22, -1990404162);
    a = ff(a, b, c, d, x[i+12], 7 ,  1804603682);
    d = ff(d, a, b, c, x[i+13], 12, -40341101);
    c = ff(c, d, a, b, x[i+14], 17, -1502002290);
    b = ff(b, c, d, a, x[i+15], 22,  1236535329);    

    a = gg(a, b, c, d, x[i+ 1], 5 , -165796510);
    d = gg(d, a, b, c, x[i+ 6], 9 , -1069501632);
    c = gg(c, d, a, b, x[i+11], 14,  643717713);
    b = gg(b, c, d, a, x[i+ 0], 20, -373897302);
    a = gg(a, b, c, d, x[i+ 5], 5 , -701558691);
    d = gg(d, a, b, c, x[i+10], 9 ,  38016083);
    c = gg(c, d, a, b, x[i+15], 14, -660478335);
    b = gg(b, c, d, a, x[i+ 4], 20, -405537848);
    a = gg(a, b, c, d, x[i+ 9], 5 ,  568446438);
    d = gg(d, a, b, c, x[i+14], 9 , -1019803690);
    c = gg(c, d, a, b, x[i+ 3], 14, -187363961);
    b = gg(b, c, d, a, x[i+ 8], 20,  1163531501);
    a = gg(a, b, c, d, x[i+13], 5 , -1444681467);
    d = gg(d, a, b, c, x[i+ 2], 9 , -51403784);
    c = gg(c, d, a, b, x[i+ 7], 14,  1735328473);
    b = gg(b, c, d, a, x[i+12], 20, -1926607734);
    
    a = hh(a, b, c, d, x[i+ 5], 4 , -378558);
    d = hh(d, a, b, c, x[i+ 8], 11, -2022574463);
    c = hh(c, d, a, b, x[i+11], 16,  1839030562);
    b = hh(b, c, d, a, x[i+14], 23, -35309556);
    a = hh(a, b, c, d, x[i+ 1], 4 , -1530992060);
    d = hh(d, a, b, c, x[i+ 4], 11,  1272893353);
    c = hh(c, d, a, b, x[i+ 7], 16, -155497632);
    b = hh(b, c, d, a, x[i+10], 23, -1094730640);
    a = hh(a, b, c, d, x[i+13], 4 ,  681279174);
    d = hh(d, a, b, c, x[i+ 0], 11, -358537222);
    c = hh(c, d, a, b, x[i+ 3], 16, -722521979);
    b = hh(b, c, d, a, x[i+ 6], 23,  76029189);
    a = hh(a, b, c, d, x[i+ 9], 4 , -640364487);
    d = hh(d, a, b, c, x[i+12], 11, -421815835);
    c = hh(c, d, a, b, x[i+15], 16,  530742520);
    b = hh(b, c, d, a, x[i+ 2], 23, -995338651);

    a = ii(a, b, c, d, x[i+ 0], 6 , -198630844);
    d = ii(d, a, b, c, x[i+ 7], 10,  1126891415);
    c = ii(c, d, a, b, x[i+14], 15, -1416354905);
    b = ii(b, c, d, a, x[i+ 5], 21, -57434055);
    a = ii(a, b, c, d, x[i+12], 6 ,  1700485571);
    d = ii(d, a, b, c, x[i+ 3], 10, -1894986606);
    c = ii(c, d, a, b, x[i+10], 15, -1051523);
    b = ii(b, c, d, a, x[i+ 1], 21, -2054922799);
    a = ii(a, b, c, d, x[i+ 8], 6 ,  1873313359);
    d = ii(d, a, b, c, x[i+15], 10, -30611744);
    c = ii(c, d, a, b, x[i+ 6], 15, -1560198380);
    b = ii(b, c, d, a, x[i+13], 21,  1309151649);
    a = ii(a, b, c, d, x[i+ 4], 6 , -145523070);
    d = ii(d, a, b, c, x[i+11], 10, -1120210379);
    c = ii(c, d, a, b, x[i+ 2], 15,  718787259);
    b = ii(b, c, d, a, x[i+ 9], 21, -343485551);

    a = add(a, olda);
    b = add(b, oldb);
    c = add(c, oldc);
    d = add(d, oldd);
  }
  return rhex(a) + rhex(b) + rhex(c) + rhex(d);
}
/*启动密钥长度检查*/
function checkkey( element, errMsg ) 
{

    if(!iskeyValid( element.value ) )
    {
        element.focus();
        element.select();
        return false;
    }
    return true;
}
/* 启动密钥有效性检查*/
function iskeyValid(key)
{
	for ( i=0; i<key.length; i++ )
        {
				var exp_str = "请输入启动密钥";//输入规则：数字0～9,字母a～z或A～Z";
                var ch=key.charAt( i );
                if ((( ch<'0') || (ch>'9' )) && 
					((ch >'z' )||(ch <'a'))  && 
					((ch >'Z' )||(ch <'A')))
                {
                        var err_msg = exp_str;
                        alert(err_msg) ;
                        return false;
                }
        }
	return true;
}

