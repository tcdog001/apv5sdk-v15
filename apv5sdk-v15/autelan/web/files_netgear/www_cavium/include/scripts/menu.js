var menuObject;
Object.extend(Array.prototype, {
	removeItem: function(search) {
		for(var i=0;i<this.length;i++) {
			if (this[i] == search) {
				return this.splice(i,1);
			}
		}
	}
});

var page_map_menu={
    "LAN Setting"            :"lansetup.htm",
    "DNS Server"        :"dnsserver.htm",
    "NTP Server"            :"ntpserver.htm",
    "WirelessInfo"        :"wlansetup.htm",
    "WirelessSet"    :"wireless_setting.htm",
    "Security"            :"security.htm",
    "RADIUS"        :"radiusserver.htm",
    "MacFilter"            :"macfilter.htm",
    "WDS"        :"wds.htm",
    "RogueAPDetect"    :"rogueap_detection.htm",
    "Advance"            :"advanceset.htm",
    "Intelligent"        :"auto_adjust.htm",
    "Wired Interface"            :"wired_if.htm",
    "Wireless Interface"        :"wireless_if.htm",
    "3G"    :"wan_3g_settings.htm",
    "Route Setting"            :"route_forward.htm",
    "STP"        :"stp.htm",
    "VLAN"            :"vlan.htm",
    "DHCP Snooping"        :"dhcp_snooping.htm",
    "DHCP Server"    :"dhcpserver.htm",
    "Firewall"            :"firewall.htm",
    "Admin Security"        :"admin_security.htm",
    "Access Security"            :"netfilter.htm",
    "WMM Settings"        :"qos.htm",
    "Traffic Limit"    :"qos_queue.htm",
    "Locator Service"            :"services.htm",
    "FirmUpgrade"        :"firmwareupgrade1.htm",
    "WorkMode"            :"workmode.htm",
    "Backup"        :"backsetup.htm",
    "Diagnostics"    :"diagnostics.htm",
    "Reboot"        :"operate.htm",
    "ChangePassword"            :"change_password.htm",
    "RemoteAccess"        :"adminaccess.htm",
    "Syslog"    :"syslog.htm",
    "Tr069"            :"tr069.htm",
    "System"        :"systemstatus.htm",
    "Lan"            :"lanstatus.htm",
    "Wlan"        :"wlanstatus.htm",
    "StationList"    :"stationlist.htm",
    "StatisticInfo"    :"statistics_info.htm",
    "Help"    :"help.htm",
    "Logout"    :"logout.cgi"
};


var menuClass = Class.create({
	data: $H({
		'BasicSetting': $H({
			"LAN Setting": [],
			"DNS Server": [],
			"NTP Server": []
		}),
		'Wireless': $H({
			"WirelessInfo": [],
			"WirelessSet": [],
			"Security": [],
			"RADIUS": [],
			"MacFilter": [],
			"WDS": [],
			"RogueAPDetect": [],
			"Advance": [],
			"Intelligent": []
		}),
		'Interface': $H({
			"Wired Interface": [],
			"Wireless Interface": [],
			"3G": []
		}),
		'Network': $H({
			"Route Setting": [],
			"STP": [],
			"VLAN": [],
			"DHCP Snooping": [],
			"DHCP Server": [],
			"Firewall": []
		}),
		'AccessControl': $H({
			"Admin Security": [],
			"Access Security": []
		}),
		'QoS': $H({
			"WMM Settings": [],
			"Traffic Limit": []
		}),
		'Service': $H({
			"Locator Service": []
		}),
		'Adminstration': $H({
			"FirmUpgrade": [],
			"WorkMode": [],
			"Backup": [],
			"Diagnostics": [],
			"Reboot": [],
			"ChangePassword": [],
			"RemoteAccess": [],
			"Syslog": [],
			"Tr069": []
		}),
		'Status': $H({
			"System": [],
			"Lan": [],
			"Wlan": [],
			"StationList": [],
			"StatisticInfo": []
		}),
		'Help': $H({}),
		'Logout': $H({})
	}),
	initialize: function () {
		//this.processConfig();
		this.pointer = {first: 0, second: 0, third: 0, fourth: 0};
		this.currentData = { first: null, second: null, third: null, fourth: [] };
		this.initialLoad = true;
		this.getFirstLevelData();
		//this.updateMenu('first',0);
	},
	processConfig: function() {
		if (!config.MBSSID.status && !config.HTTPREDIRECT.status && !config.SYSLOGD.status && !config.ETHERNET_CONFIG.status) {
			this.data.get("Configuration").get("System").unset("Advanced");
			this.data.get("Monitoring").unset("Logs");
		}
		else {
			if (!config.MBSSID.status) {
				this.data.get("Configuration").get("System").get("Advanced").removeItem("General");
			}
			if (!config.HTTPREDIRECT.status) {
				this.data.get("Configuration").get("System").get("Advanced").removeItem("Hotspot");
			}
			if (!config.SYSLOGD.status) {
				this.data.get("Configuration").get("System").get("Advanced").removeItem("Syslog");
				this.data.get("Monitoring").unset("Logs");
			}
			if (!config.ETHERNET_CONFIG.status) {
				this.data.get("Configuration").get("System").get("Advanced").removeItem("Ethernet");
			}
		}
		if (!config.DHCPSERVER.status) {
			this.data.get("Configuration").get("IP").unset("DHCP Server Settings");
		}
		if (!config.WMM.status) {
			this.data.get("Configuration").get("Wireless").get("Basic").removeItem("QoS Settings");
			this.data.get("Configuration").get("Wireless").get("Advanced").removeItem("QoS Settings");
		}

		if (!config.MACACL.status) {
			this.data.get("Configuration").get("Security").get("Advanced").removeItem("MAC Authentication");
		}
		if (!config.ROGUEAP.status) {
			this.data.get("Configuration").get("Security").get("Advanced").removeItem("Rogue AP");
			this.data.get("Monitoring").unset("Rogue AP");
		}
		if (!config.SNMP.status && ((!config.SSH.status && !config.TELNET.status) || (!config.CLI.status)))  {
			this.data.get("Maintenance").unset("Remote Management");
		}
		else {
			if (!config.SNMP.status) {
				this.data.get("Maintenance").get("Remote Management").unset("SNMP");
			}
			if (((!config.SSH.status && !config.TELNET.status) || (!config.CLI.status))) {
				this.data.get("Maintenance").get("Remote Management").unset("Remote Console");
			}
		}
	},
	updateItem: function(item, value, key) {
		var key = (key == undefined)?0:key;
		switch(item) {
			case 'first':
				this.currentData.first = value;
				break;
			case 'second':
				this.currentData.second = value;
				break;
			case 'third':
				//this.currentData.third = value;
				break;
			case 'fourth':
				//this.currentData.fourth[key] = value;
				break;
		}
	},
	updatePointer: function(level, value) {
		switch(level) {
			case 'first':
				this.pointer.first = value;
				break;
			case 'second':
				this.pointer.second = value;
				break;
			case 'third':
				this.pointer.third = value;
				break;
			case 'fourth':
				this.pointer.fourth = value;
				break;
		}
	},
	getLevel: function(level,id) {
		switch(level) {
			case 'first':
				return this.currentData.first[id];
				break;
			case 'second':
				return this.currentData.second[id];
				break;
			case 'third':
				//return this.currentData.third[id];
				break;
			case 'fourth':
				//return this.currentData.fourth[id];
				break;
		}
		return this.first[id];
	},
	getPointer: function(level) {
		switch(level) {
			case 'first':
				return this.pointer.first;
				break;
			case 'second':
				return this.pointer.second;
				break;
			case 'third':
				return this.pointer.third;
				break;
			case 'fourth':
				return this.pointer.fourth;
				break;
		}
	},
	resetPointer: function(level) {
		switch(level) {
			case 'first':
				this.updatePointer('second',0);
				break;
			case 'second':
				this.updatePointer('third',0);
				break;
			case 'third':
				this.updatePointer('fourth',0);
				break;
		}
	},
	getFirstLevelData:  function() {
		this.updateItem('first',this.data.keys());
		this.getSecondLevelData()
	},
	getSecondLevelData: function() {
		var dataValues = this.data.values();
		for (var i=0; i<dataValues.length;i++) {
			if (i == this.getPointer('first')) {
				this.updateItem('second',dataValues[i].keys());	
				//alert('Second Level['+i+'] Updated with '+dataValues[i].keys());
			}
		}
//		this.getThirdLevelData();
	},
	getThirdLevelData: function() {
		
	},
	getFourthLevelData: function() {
		
	},
	updateMenu: function(level, pointer, start) {
		if (window.top.frames['master']._disableAll != undefined && window.top.frames['master']._disableAll == true) {
			return ;
		}
/*		if (!window.top.frames['header']._initiateMenu) {
			if (window.top.frames['master'].progressBar == undefined || window.top.frames['master'].progressBar.isOpened() == true) {
				//if (!confirm('Page is currently loading!\nAre you sure you want to navigate to this page?'))
					//return;
			}
		}*/
		if (start!=undefined) {
			this.initialLoad = start;
		}
		this.updatePointer(level,pointer);
		switch(level) {
			case 'first':
				this.getFirstLevelData();
				this.updateFirstMenu();
				break;
			case 'second':
				this.getSecondLevelData();
				this.updateSecondMenu();
				break;
			case 'third':
				this.getThirdLevelData();
				if (!this.initialLoad) {
					this.updateThirdMenu();
				}
				else {
					loadThird = setTimeout(loadThirdMenu, 50);
				}
				break;
		}
		if (!this.initialLoad) {
			showMasterPage(this.getLevel('first',this.getPointer('first')),this.getLevel('second',this.getPointer('second')));
			//showPage('',this.getLevel('third',this.getPointer('third')),this.getLevel('second',this.getPointer('second')),this.getLevel('first',this.getPointer('first')),[this.getPointer('third'),0],false);
		}
		return;
	},
	updateFirstMenu: function() {
		var primaryTabs = $('primaryNav').immediateDescendants();
		for (var x=0; x< primaryTabs.length; x++) {
			if (this.getPointer('first') == x) {
				primaryTabs[x].replace("<LI class='Active'><A href='#' onclick=\"javascript:menuObject.updateMenu('first',"+x+", false);\">"+this.getLevel('first',x)+"</A></LI>");
			}
			else {
				primaryTabs[x].replace("<LI><A href='#' onclick=\"javascript:menuObject.updateMenu('first',"+x+",false);\">"+this.getLevel('first',x)+"</A></LI>");
			}
		}
		if (this.getLevel('first', this.getPointer('first')) == 'Monitoring' || this.getLevel('first', this.getPointer('first')) == 'Support') {
			if (typeof(window.top.frames['action'].$) == 'function' && window.top.frames['action'].$('ButtonsDiv') != undefined)
				window.top.frames['action'].$('standardButtons').hide();
		}
		else {
			if (typeof(window.top.frames['action'].$) == 'function' && window.top.frames['action'].$('ButtonsDiv') != undefined)
				window.top.frames['action'].$('standardButtons').show();
		}
		this.updatePointer('second',0);
		this.getSecondLevelData();
		this.updateSecondMenu();
	},
	updateSecondMenu: function() {
		var secondaryTabs = $('secondaryNav').immediateDescendants();
		for (var x=0; x< secondaryTabs.length; x++) {
			var str = "<LI><A href='javascript:void(0)' onclick=\"menuObject.updateMenu('second',"+x+",false);\" ";
			if (this.getPointer('second') == x) {
				str = str + "class='Active'";
			}
			str = str + ">"+this.getLevel('second',x)+"</A>";
			if (this.currentData.second.length != x+1)
				str =  str + '&nbsp;<img src="images/tab_separator.gif" class="separatorImage">&nbsp;';
			str = str + '</LI>';
			if (this.getLevel('second',x)!=undefined) {
				secondaryTabs[x].show();
				secondaryTabs[x].replace(str);	
			}
			else {
				secondaryTabs[x].hide();
			}
		}
		//this.updatePointer('third',0);
		//alert(this.getPointer('first')+'----'+this.getPointer('second')+'----'+this.getPointer('third')+'----'+this.getPointer('fourth'));

		//this.getThirdLevelData();
		//if (!this.initialLoad) 
		//	this.updateThirdMenu();
	},
	updateThirdMenu: function() {
		
	},
	prepareThirdMenu: function() {
		
		//alert(this.getLevel('first',this.getPointer('first')) + '---' + this.getLevel('second',this.getPointer('second')) + '---' +this.getLevel('third',this.getPointer('third')));
		//alert(window.top.frames['thirdmenu'].$('TreeFrame').innerHTML);
	},
	prepareFourthMenu: function(level, body) {
		
	},
	updateFourthMenu: function(level) {
		
	},
	test: function() {
		alert("First = "+this.pointer.first+"\nSecond = "+this.pointer.second+"\nThird = "+this.pointer.third+"\nFourth = "+this.pointer.fourth);
	}
});

menuObject = new menuClass();
//menuObject.updateFirstMenu();

Event.onDOMReady ( function() {
	if (window.top.frames['header']._initiateMenu != undefined && window.top.frames['header']._initiateMenu != false) {
		window.top.frames['header'].menuObject.updateMenu('first', 0, true);
	//	window.top.frames['header'].menuObject.test();
	}
});
function initiateMenu(start)
{
	window.top.frames['header'].menuObject.updateMenu('first',0, true);
	window.top.frames['header'].menuObject.test();
}

function showMasterPage(primary,second)
{
	
	try {
		if (top.master._disableAll != undefined && top.master._disableAll == true) {
			return ;
		}
		if ((window.top.frames['master'].progressBar!= undefined) && (!window.top.frames['master'].progressBar.isOpen)) {
			window.top.frames['master'].progressBar.open();
		}
	
		//setActiveContent(false);
	
		if(primary != "")
		{
			if(primary == "Help" )
			{
				window.top.frames['master'].document.location.href=page_map_menu[primary];
			}
			else if(primary == "Logout")
			{
				window.top.location.href="index.htm";
			}
			else
			{
				window.top.frames['master'].document.location.href=page_map_menu[second];
			}
		}
	}
	catch (e) {
		window.top.frames['master'].document.location.href = 'index.htm';
		//alert('Please wait until page loads!');
	}
	
}

//var menu1 = new menuData();
