/*****************
 * Frames related functions....
 */
var columntype=""
var defaultsetting=""

function getCurrentSetting()
{
	if (document.body)
		return (document.body.cols)? document.body.cols : document.body.rows;
}

function setframevalue(coltype, settingvalue)
{
	if (coltype=="rows")
		document.body.rows=settingvalue;
	else if (coltype=="cols")
		document.body.cols=settingvalue;
}

function resizeFrame(contractsetting)
{
	if (getCurrentSetting()!=defaultsetting)
		setframevalue(columntype, defaultsetting);
	else
		setframevalue(columntype, contractsetting);
}

function init()
{
	if (!document.all && !document.getElementById) return;
	if (document.body!=null){
		columntype=(document.body.cols)? "cols" : "rows";
		defaultsetting=(document.body.cols)? document.body.cols : document.body.rows;
	}
}
/**********
 * End Frames functions
 */


function convertLeaseTime(seconds,format)
{
	var days = parseInt(seconds/86400);
	var hrs = parseInt((seconds-(days*86400))/3600);
	var mnts = parseInt((seconds-((days*86400)+(hrs*3600)))/60)
	if (format == 'days') {
		return days;
	}
	else if (format == 'hours') {
		return hrs;
	}
	else if (format == 'minutes') {
		return mnts;
	}
}

function convertLeaseTime2Seconds(days, hours, minutes)
{
	var days = $('dhcpsLeaseDays').value;
	var hours = $('dhcpsLeaseHours').value;
	var minutes = $('dhcpsLeaseMinutes').value;
	var seconds = ((days*86400)+(hours*3600)+(minutes*60));
	if (seconds < 120) {
		alert('Lease Time cannot be less than 2 minutes!');
		return false;
	}
	else if ((seconds/60) > 143999) {
		alert('Lease Time cannot be more than 143999 minutes!');
		return false;
	}
	$('dhcpsLeaseTime').value = seconds;
}

function tbhdr(tlt,hlpurl){
	str="<table class='tableStyle'><tr><td colspan='2' class='subSectionTabTopLeft spacer80Percent font12BoldBlue'>"+tlt;
	str+="</td><td class='subSectionTabTopRight spacer20Percent'><a href='javascript: void(0);' onclick=\"showHelp('"+tlt+"','"+hlpurl+"');\">";
	str+="<img src='../images/help_icon.gif' width='12' height='12' title='Click for help'/></a></td></tr><tr><td colspan='3' class='subSectionTabTopShadow'></td></tr></table>"
	document.write(str);
}


function checkNum()
{
	var carCode = event.keyCode;
	if ((carCode < 48) || (carCode > 57))
	{
		event.cancelBubble = true;
		event.returnValue = false;
	}
}

function updateStationInfo(event,mac)
{
	if (!mac || mac == -1) {
		alert('Please Select a station!');
		return false;
	}
	row = $(mac);
    for (var i=0;i < row.cells.length; i++)
	{
		if ($('layeredWinTable').rows[i]) {
			if ($('layeredWinTable').rows[i].cells[1]) {
				if ($('layeredWinTable').rows[i].cells[1].childNodes[0]) {
					$($('layeredWinTable').rows[i].cells[1]).update('<table style="width: 90px; border: 1px solid #7FB2E5; background: #FFFFFF; padding: 0px; margin: 0px;"><tr><td style="width: 90px; padding: 1px; margin: 0px;">' + row.cells[(i + 1)].innerHTML.pad(20," ",1) + '</td></tr></table>');
				}
			}
		}
	}
	if (event!=null)
	var evt = getEvent(event);
	return true;
}

function showPopupWindow()
{
	detailsPopup = window.open('','Details','toolbar=0,scrollbars=0,location=0,statusbar=0,menubar=0,resizable=1,width=510px, height=520px');
	detailsPopup.document.body.innerHTML = '';
	detailsPopup.document.write('<html><title>Wireless Station Details</title><head><link rel="stylesheet" href="include/css/style.css" type="text/css">'+
			'<link rel="stylesheet" href="include/css/default.css" type="text/css">' +
'</head><body style="padding: 5px;"><div id="tempDiv">'+$('layeredWindow').innerHTML+'</div></body></html>');
	detailsPopup.focus();
}

function showSurveyPopupWindow()
{
	detailsPopup = window.open('siteSurvey.php','Details','toolbar=no,scrollbars=yes,statusbar=no,menubar=no,resizable=yes,width=510px, height=450px');
	detailsPopup.focus();
}

function str_replace(search, replace, subject) {
    var f = search, r = replace, s = subject;
    var ra = r instanceof Array, sa = s instanceof Array, f = [].concat(f), r = [].concat(r), i = (s = [].concat(s)).length;

    while (j = 0, i--) {
        if (s[i]) {
            while (s[i] = (s[i]+'').split(f[j]).join(ra ? r[j] || "" : r[0]), ++j in f){};
        }
    }
    return sa ? s : s[0];
}

function copyAPDetails(row)
{
    var val = str_replace('&nbsp;',' ',row.getElementsByTagName('td')[1].innerHTML);
    $('wirelessSSID0').value = str_replace('&amp;','&',val);
	//$('wirelessSSID0').value = row.getElementsByTagName('td')[1].innerHTML.replace('&nbsp;',' ').replace('&amp;','&');
	//alert(row.getElementsByTagName('td')[1].innerHTML);
	$('authenticationType').value = row.getElementsByTagName('td')[2].getElementsByTagName('INPUT')[0].value;
	var enc = (isNaN(parseInt(row.getElementsByTagName('td')[3].getElementsByTagName('INPUT')[0].value)) || row.getElementsByTagName('td')[3].getElementsByTagName('INPUT')[0].value == '')?64:row.getElementsByTagName('td')[3].getElementsByTagName('INPUT')[0].value
	$('key_size_11g').value = (isNaN(parseInt(row.getElementsByTagName('td')[3].getElementsByTagName('INPUT')[0].value)) || row.getElementsByTagName('td')[3].getElementsByTagName('INPUT')[0].value == '')?64:row.getElementsByTagName('td')[3].getElementsByTagName('INPUT')[0].value;
	DisplayClientSettings($('authenticationType').value, enc);
	if ($('key_size_11g').value > 8) {
		$('encryption').value=1;
		$('wepKeyType').disabled=false;
		$('wepKeyType').value=$('key_size_11g').value;
	}
	//showPassphrase();
	setActiveContent();
	detailsPopup.close();
}

function showPassphrase()
{
	if ($('key_size_11g').value == 0) {
		//hide all
		$('wpa_row').style.display = "none";
		$('wep_row').style.display = "none";
		$('wpa_row').disabled = true;
		$('wep_row').disabled = true;
	}
	else if ($('key_size_11g').value == 2 || $('key_size_11g').value == 4) {
		//show wpa presharedkey
		$('wep_row').style.display = "none";
		$('wep_row').disabled = true;
		try {
			$('wpa_row').style.display = "table-row";
		}
		catch (e) {
			$('wpa_row').style.display = "block";
		}
		$('wpa_row').disabled = false;
	}
	else {
		//show wep passphrase
		$('wpa_row').style.display = "none";
		$('wpa_row').disabled = true;
		try {
			$('wep_row').style.display = "table-row";
		}
		catch (e) {
			$('wep_row').style.display = "block";
		}
		$('wep_row').disabled = false;
		gen_11g_keys();
	}
}

function DisplayClientSettings(mode,option)
{
	var obj=fetchObjectsByAttribute("mode","TR");
	var flag=false;
	if (mode=='32') {
		var List=new Array('AES');
		var ListVal=new Array('4');
		$('encryption').value='4';
		$('wepKeyType').disabled=true;
	}
	else if (mode=='16') {
		var List=new Array('TKIP');
		var ListVal=new Array('2');
		$('encryption').value='2';
		$('wepKeyType').disabled=true;
	}
	else if (mode=='1') {
		var List=new Array('64 bit WEP','128 bit WEP', '152 bit WEP');
		var ListVal=new Array('64','128','152');
		$('encryption').value='1';
		$('wepKeyType').disabled=false;
		$('wepKeyType').value='64';
	}
	else if (mode!= '0') {
		var List=new Array('None');
		var ListVal=new Array('0');
		$('encryption').value='0';
		$('wepKeyType').disabled=true;
	}
	else {
		var List=new Array('None','64 bit WEP','128 bit WEP', '152 bit WEP');
		var ListVal=new Array('0','64','128','152');
		$('encryption').value='0';
		$('wepKeyType').disabled=true;
		if (option == 64) {
			flag = true;
		}
	}


	var dataEnc = $('key_size_11g');
	var selEnc = dataEnc.value;
	do
		dataEnc.options[0] = null;
	while (dataEnc.length > 0);
	for (var j=0; j<List.length; j++)
	{
		var opt=document.createElement('OPTION');
		opt.text=List[j];
		opt.value=ListVal[j];
		if (selEnc == ListVal[j]) {
			if (opt.value > 8) {
				$('wepKeyType').value=opt.value;
			}
			opt.selected="selected";
		}
//alert(opt.text+'======='+opt.value);
		try {
			dataEnc.add(opt, null); // standards compliant; doesn't work in IE
		}
		catch(ex) {
			dataEnc.add(opt); // IE only
		}
	}
	if (flag) {
		dataEnc.value = 64;
	}
	for (var k=0; k < obj.length; k++) {
		if ((obj[k].getAttribute("mode")==mode || mode == 0) && dataEnc.value != 0) {
			showNenable(obj[k]);
		}
		else {
			hideNdisable(obj[k]);
		}
	}
	showPassphrase();
}


var x = 0;
var intervalId;
function updatePosition(item,pixel) {
	pixel = parseInt(x);
	x = x + 20;
	if (pixel > 200)
		clearInterval(intervalId);
	else
	switch(item) {
		case "left": $("progress").style.left=(pixel) + 'px'; break;
		case "top": $("progress").style.top=(pixel) + 'px'; break;
		case "width": $("progress").style.width=(pixel) + 'px'; break;
		case "height": $("progress").style.height=(pixel) + 'px'; break;
	}
}

function checkActiveSession()
{
	if (parent.parent.frames['master'].progressBar != undefined)
		parent.parent.frames['master'].progressBar.open();
	var oOptions = {
            method: "get",
			asynchronous: false,
			parameters: { checkActiveSession: 'check', id: Math.floor(Math.random()*1000001) },
            onSuccess: function (oXHR, oJson) {
				var response = oXHR.responseText;
				if(response == 'expired') {
					//alert("Session Expired!");
					window.top.location.href = "index.php";
				}
				else {
					//alert('Session Active!');
			    		return true;
				}
				return false;                
            },
            onFailure: function (oXHR, oJson) {
				alert("There was an error with the connection, Please try again!"); 
				parent.parent.frames['master'].progressBar.close();
            }
        };
    var req = new Ajax.Request('checkSession.php', oOptions);
}

function doSubmit(obj, val, changed)
{
	if (window.top.frames['master']._disableAll != undefined && window.top.frames['master']._disableAll == false) {
		top.master._disableAll = true;
	}
	if (changed != undefined) {
		if (changed == false) {
			return;
		}
	}
	else if (val == 'apply') {
		return;
	}
	if (val != 'cancel' && $('sysCountry') != undefined && $('sysCountryRegion') != undefined && $('sysCountry').value != $('sysCountryRegion').value) {
		if (!confirm("Changed Country / Region will be applied only after rebooting!\n\nClick OK to apply the changes and reboot.")) {
			return;
		}
	}
	setActiveContent(false);

	if ( $('firmwareFile') != undefined) {
    	parent.parent.frames['master'].progressBar.open();
		$('Action').disabled = false;
		if (val == 'apply')
			$('Action').value = val;
		document.dataForm.submit();
	}
	else if (val != 'apply') {
		if (val=='login') {
			if (inputForm && inputForm.formLiveValidate()) {
				window.top.frames['master'].progressBar.open();
				window.top.frames['master'].document.dataForm.submit();
			}
			else {
				var evt = getEvent(obj);
				evt.returnValue = false;
				return false;
			}
		}
		else {
			window.top.frames['master'].progressBar.open();
			window.top.frames['master'].document.dataForm.submit();
		}
	}
	else if (inputForm && inputForm.formLiveValidate()) {
		if (($('activeMode')!=undefined) && $('activeMode').value == '') {
			$RD('WirelessMode1').each(function(radio) {
				radio.disabled=true;
			});
		}
		if ($('activeMode')!= undefined)
			eval('var res = (typeof(disableChannelonWDS'+(($('activeMode').value=='3'||$('activeMode').value=='4')?'1':'0')+') == "boolean");');
		else
			var res = false;
        if ($('currentInterfaceNum') != undefined)
            var iface = $('currentInterfaceNum').value;
        else
            var iface = '';
        if (val != 'cancel' && $('maxWirelessClients'+iface) != undefined && $('dbMaxWirelessClients'+iface) != undefined && $('dbMaxWirelessClients'+iface).value != $('maxWirelessClients'+iface).value) {
            if (!confirm("Maximum wireless clients will be applied only after rebooting!\n\nClick OK to apply the changes and reboot.")) {
                return;
            }
        }
        if ((typeof(window.top.frames['master'].addMacWin) == 'object') && (window.top.frames['master'].addMacWin.isOpened() == true)) {
			alert("Please close the layered window before applying the changes!");
			setActiveContent();
		}
		else if ((res) && (!checkWDSStatus('',($('activeMode').value=='3'||$('activeMode').value=='4'||$('activeMode').value=='5')?'1':'0'))) {
		}
		else {
			var flag = true;
			if (($('priRadIpAddr') != undefined) && ($('sndRadIpAddr') != undefined) && ($('priRadIpAddr').value == '') && ($('sndRadIpAddr').value != '')) {
				alert('Primary Server should be configured first!');
			}
			else if (($('priAcntIpAddr') != undefined) && ($('sndAcntIpAddr') != undefined) && ($('priAcntIpAddr').value == '') && ($('sndAcntIpAddr').value != '')) {
				alert('Primary Server should be configured first!');
			}
			else if (($('priAcntIpAddr') != undefined) && ($('authType') != undefined) && $('priRadIpAddr').value == '' && (($('authType').value == '0')) ) {
				alert("Primary Radius server IP should not be empty when Network Authentication set to radius!!");
			}	
			else {
				flag = false;
			}
			if (!flag) {
				for (var i = 0; i < document.dataForm.elements.length; i++) {
					if ((window.top.frames['master'].document.dataForm.elements[i].type != 'hidden')) {
	                    if ((window.top.frames['master'].document.dataForm.elements[i].getAttribute("masked") == "true")||(window.top.frames['master'].document.dataForm.elements[i].getAttribute("masked") == true)) {
	                        if (window.top.frames['master'].document.dataForm.elements[i].getAttribute("validate").indexOf('IpAddress') != -1) {
	                            if (window.top.frames['master'].document.dataForm.elements[i].value == '')
	                                window.top.frames['master'].document.dataForm.elements[i].value = '0.0.0.0';
	                        }
	                        else if (window.top.frames['master'].document.dataForm.elements[i].getAttribute("validate").indexOf('MACAddress') != -1) {
	                            if (window.top.frames['master'].document.dataForm.elements[i].value == '')
	                                window.top.frames['master'].document.dataForm.elements[i].value = '00:00:00:00:00:00';
	                        }
							else {
	                            window.top.frames['master'].document.dataForm.elements[i].disabled = true;
	                        }
	                    }
					}
				}
	            
				window.top.frames['master'].progressBar.open();
				$('Action').disabled = false;
				if (val == 'apply')
					$('Action').value = val;
				window.top.frames['master'].document.dataForm.submit();
			}
		}
	}
}

function activateTab(obj, id, mainId, tabCount, iface)
{
    if (!config.FIVEGHZ.status && id != '1') {
        $('br_head').innerHTML = '5GHz operation not supported for this country!';
        $('errorMessageBlock').show();
    }
    else {
        if (!config.FIVEGHZ.status && $('br_head').innerHTML == '5GHz operation not supported for this country!') {
            $('errorMessageBlock').hide();
        }
        if (mainId == undefined || mainId == '') {
			var mode = $('activeMode').value;
			if (config.TWOGHZ.status && $('modeWlan0') != undefined) {
				var modeWlan0 = $('modeWlan0');
			}
			if (config.FIVEGHZ.status && $('modeWlan1') != undefined) {
				var modeWlan1 = $('modeWlan1');
			}
            if (id == '1') {
				if (config.TWOGHZ.status) {
					$('inlineTab1').className = "Active";
					$('inlineTab1').blur();
				}
				if (config.FIVEGHZ.status) {
					$('inlineTab2').className = "";
				}

				//				alert("Before tab1 Switch:"+"\n"+'activeMode = '+mode+"\nmodeWlan0="+modeWlan0.value+"\nmodeWlan1="+modeWlan1.value);
				var rad = getCheckedRadioById('WirelessMode1');
				if (modeWlan0 != undefined && modeWlan0.value != 3 && modeWlan0.value != 4 && modeWlan0.value != '') {
					if (mode != '' && mode != 3 && mode != 4) {
						setRadioCheckById('WirelessMode1', mode, mode);
						modeWlan0.value = mode;

					}
					else {
						setRadioCheckById('WirelessMode1', modeWlan0.value,modeWlan0.value);
					}
					if (modeWlan0.value == mode)
						$('cb_chkRadio0').checked = true;
					setActiveMode($('cb_chkRadio0'), 'WirelessMode1', false);
					DispChannelList(1, modeWlan0.value,'chkRadio0');
					if (modeWlan0.value==2)
						enable11nFields('show',1);
					else
						enable11nFields('hide',1);

					if (rad != undefined && rad.value > 2) {
						//modeWlan0.value='';
						modeWlan1.value = rad.value;
						rad.checked = false;
					}
				}

				//				alert("After tab1 Switch:\n"+'activeMode = '+mode+"\nmodeWlan0="+modeWlan0.value+"\nmodeWlan1="+modeWlan1.value);

				if (mode != '') {
					if ($('cb_chkRadio0') == undefined) {
						if (mode != '3' && mode != '4') {
							if ($('cb_wdsEnabled') != undefined) {
								if ($('cb_wdsEnabled').checked != false) {
									Form.enable(document.dataForm);
								}
								$('cb_wdsEnabled').disabled = false;
								$('wdsEnabled').disabled = false;
								alert($('enableQoS').enableForm);
							}
							else {
								if ($('enableQoS') != undefined) {
									if ($('enableQoS').enableForm == false) {
										Form.disable(document.dataForm);
									}
									else {
										Form.enable(document.dataForm);
									}
								}
								else {
									if ($('rogueApDetection') != undefined) {
										if ($('rogueApDetection').value == 'wlan1') {
											if ($('cb_rogueApDetection0') != undefined) {
												$('cb_rogueApDetection0').disabled = false;
												$('rogueApDetection0').disabled = false;
												toggleFields($('cb_rogueApDetection0'), false);
											}
											else
												if ($('cb_accessControlMode0') != undefined) {
													$('cb_accessControlMode0').disabled = false;
													$('accessControlMode0').disabled = false;
													toggleFields($('cb_accessControlMode0'), false);
												}
												else
													if ($('cb_wdsEnabled0') != undefined) {
														wdsOnEnable($('wdsEnabled0').checked, '0', true);
														$('cb_wdsEnabled0').disabled = false;
														$('wdsEnabled0').disabled = false;
													}
										}
										else {
											if ($('cb_rogueApDetection1') != undefined) {
												$('cb_rogueApDetection1').disabled = false;
												$('rogueApDetection1').disabled = false;
												toggleFields($('cb_rogueApDetection1'), false);
											}
											else
												if ($('cb_accessControlMode1') != undefined) {
													$('cb_accessControlMode1').disabled = false;
													$('accessControlMode1').disabled = false;
													toggleFields($('cb_accessControlMode1'), false);
												}
												else
													if ($('cb_wdsEnabled1') != undefined) {
														wdsOnEnable($('wdsEnabled1').checked, '1',true);
														$('cb_wdsEnabled1').disabled = false;
														$('wdsEnabled1').disabled = false;
													}
										}
									}
									else {
										if ($('formDisabled') == undefined)
											Form.enable(document.dataForm);
									}
								}
							}
							if ($('formDisabled') == undefined) {
								["refresh","edit"].each(function(buttonId) {
									if (window.top.frames['action'].$(buttonId) != undefined) {
										window.top.frames['action'].$(buttonId).disabled = false;
										window.top.frames['action'].$(buttonId).src = window.top.frames['action'].$(buttonId).src.replace('_off.gif','_on.gif');
									}
								});
							}
						}
						else {
							Form.disable(document.dataForm);
							["refresh","edit"].each(function(buttonId) {
								if (window.top.frames['action'].$(buttonId) != undefined) {
									window.top.frames['action'].$(buttonId).disabled = true;
									window.top.frames['action'].$(buttonId).src = window.top.frames['action'].$(buttonId).src.replace('_on.gif','_off.gif');
								}
							});
						}
					}
					else {
						$('helpURL').value=$('helpURL').value.replace('_a','_g');
					}
				}
			}
			else {
				if (config.TWOGHZ.status) {
					$('inlineTab1').className = "";
				}
				if (config.FIVEGHZ.status) {
					$('inlineTab2').className = "Active";
					$('inlineTab2').blur();
				}

				//			alert("Before tab2 Switch:\n"+'activeMode = '+mode+"\nmodeWlan0="+modeWlan0.value+"\nmodeWlan1="+modeWlan1.value);

				var rad = getCheckedRadioById('WirelessMode1');
				if (modeWlan1 != undefined && modeWlan1.value != 0 && modeWlan1.value != 1 && modeWlan1.value != 2 && modeWlan1.value != '') {
					if (mode != '' && mode != 0 && mode != 1 && mode != 2) {
						setRadioCheckById('WirelessMode1', mode, mode);
						modeWlan1.value = mode;
					}
					else {
						setRadioCheckById('WirelessMode1', modeWlan1.value, modeWlan1.value);
					}
					if (modeWlan1.value == mode)
						$('cb_chkRadio1').checked = true;
					setActiveMode($('cb_chkRadio1'), 'WirelessMode1', false);
					DispChannelList(2,modeWlan1.value,'chkRadio1');
					if (modeWlan1.value==4)
						enable11nFields('show',2);
					else
						enable11nFields('hide',2);

					if (rad != undefined && rad.value < 3) {
						modeWlan0.value = rad.value;
						//modeWlan1.value='';
						rad.checked = false;
					}
				}

				//				alert("After tab2 Switch:\n"+'activeMode = '+mode+"\nmodeWlan0="+modeWlan0.value+"\nmodeWlan1="+modeWlan1.value);

				if (mode != '') {
					if ($('cb_chkRadio1') == undefined) {
						if (mode != '0' && mode != '1' && mode != '2') {
							if ($('cb_wdsEnabled') != undefined) {
								if ($('cb_wdsEnabled').checked) {
									Form.enable(document.dataForm);
								}
								$('cb_wdsEnabled').disabled = false;
								$('wdsEnabled').disabled = false;
							}
							else {
								if ($('enableQoS') != undefined) {
									if ($('enableQoS').enableForm == false) {
										Form.disable(document.dataForm);
									}
									else {
										Form.enable(document.dataForm);
									}
								}
								else {
									if ($('rogueApDetection') != undefined) {
										if ($('rogueApDetection').value == 'wlan1') {
											if ($('cb_rogueApDetection0') != undefined) {
												$('cb_rogueApDetection0').disabled = false;
												$('rogueApDetection0').disabled = false;
												toggleFields($('cb_rogueApDetection0'), false);
											}
											else
												if ($('cb_accessControlMode0') != undefined) {
													$('cb_accessControlMode0').disabled = false;
													$('accessControlMode0').disabled = false;
													toggleFields($('cb_accessControlMode0'), false);
												}
												else
													if ($('cb_wdsEnabled0') != undefined) {
														wdsOnEnable($('wdsEnabled0').checked, '0',true);
														$('cb_wdsEnabled0').disabled = false;
														$('wdsEnabled0').disabled = false;
													}
										}
										else {
											if ($('cb_rogueApDetection1') != undefined) {
												$('cb_rogueApDetection1').disabled = false;
												$('rogueApDetection1').disabled = false;
												toggleFields($('cb_rogueApDetection1'), false);
											}
											else
												if ($('cb_accessControlMode1') != undefined) {
													$('cb_accessControlMode1').disabled = false;
													$('accessControlMode1').disabled = false;
													toggleFields($('cb_accessControlMode1'), false);
												}
												else
													if ($('cb_wdsEnabled1') != undefined) {
														wdsOnEnable($('wdsEnabled1').checked, '1',true);
														$('cb_wdsEnabled1').disabled = false;
														$('wdsEnabled1').disabled = false;
													}
										}
									}
									else {
										if ($('formDisabled') == undefined)
											Form.enable(document.dataForm);
									}
								}
							}
							if ($('formDisabled') == undefined) {
								["refresh","edit"].each(function(buttonId) {
									if (window.top.frames['action'].$(buttonId) != undefined) {
										window.top.frames['action'].$(buttonId).disabled = false;
										window.top.frames['action'].$(buttonId).src = window.top.frames['action'].$(buttonId).src.replace('_off.gif','_on.gif');
									}
								});
							}
						}
						else {
							Form.disable(document.dataForm);
							["refresh","edit"].each(function(buttonId) {
								if (window.top.frames['action'].$(buttonId) != undefined) {
									window.top.frames['action'].$(buttonId).disabled = true;
									window.top.frames['action'].$(buttonId).src = window.top.frames['action'].$(buttonId).src.replace('_on.gif','_off.gif');
								}
							});
						}
					}
					else {
						$('helpURL').value=$('helpURL').value.replace('_g','_a');
					}
				}
			}
		}
		if (iface != undefined && $('wlan'+String(Number(iface))) != undefined) {
			toggleDisplay('wlan'+String(Number(iface)));
		}
        else {
            toggleDisplay('wlan'+id);
        }
        if (Form.findFirstElement(document.dataForm)!= undefined)
			Form.focusFirstElement(document.dataForm);
    }
//		enablePrimaryFields();
}

function enablePrimaryFields()
{
	$('menu1').disabled=false;
	$('menu2').disabled=false;
	$('menu3').disabled=false;
	$('menu4').disabled=false;
	$('mode7').disabled=false;
	$('mode8').disabled=false;
	$('mode9').disabled=false;
}

function toggleMenu(menuId)
{
	var loopList = document.getElementsByTagName('UL');
	var flag = true;
	for (var i = 0; i < loopList.length; i++) {
		if (('div_' + menuId) == loopList[i].id) {
			if ($(loopList[i]).style.display != 'none') {
				$(loopList[i]).style.display='none';
				$('img_' + menuId).src = 'images/arrow_right.gif';
				flag = false;
			}
			else {
				$(loopList[i]).style.display = 'inline';
				$('img_' + menuId).src = 'images/arrow_down.gif';
			}
				$('third_' + menuId).style.color = '#FFA400';

		}
		else {
			$(loopList[i]).style.display='none';
			$('img_' + menuId).src = 'images/arrow_right.gif';
			$(loopList[i].id.replace('div_','third_')).style.color = '#46008F';
		}
	}
	if (flag) {
		$('div_' + menuId + '_1').childNodes[0].style.color = '#FFA400';
		Event.emulateClick($('div_' + menuId + '_1'));
	}
}

function toggleNTPServer(status)
{
	if (status=='1') {
		$('cb_customntp').disabled=false;
		$('ntpservername').disabled=!$('cb_customntp').checked;
	}
	else {
		$('cb_customntp').disabled=true;
		$('ntpservername').disabled=true;
	}
}

function toggleMACCloneAddress(status,id)
{
	var iface = (id == undefined)? $('currentInterfaceNum').value: id;
	if (status=='0') {
		$('macCloneAddr'+iface).disabled=true;
		if ($('activeMode').value != "") {
			$('errorMessageBlock').hide();
		}
	}
	else {
		$('macCloneAddr'+iface).disabled=false;
	}
}

function checkSpecialChars()
{
	/** allow special characters

	if ((event.keyCode > 32 && event.keyCode < 48) || (event.keyCode > 57 && event.keyCode < 65) ||
	(event.keyCode > 90 && event.keyCode < 97)) event.returnValue = false;

	**/
}


function checkInputKeys(secretKey)
{
	var tempStr = secretKey;

	if(tempStr == "")
		return 1;
	for(i=0;i<tempStr.length;i++)
	{
		var subStr = tempStr.charAt(i);
		if (subStr == "*")
		{
			return 0;
		}
	}
	return 1;
}



//dhcpserversettings page functions

function disableDhcps()
{
	graysomething();
}

// Called when DHCP server is being enabled
function enableDhcps()
{
	graysomething();
}
var posit;

function validateIPField(event,obj)
{
	var KeyValue = event.keyCode;
	if (KeyValue == 0x08) {       // BackSpace
		if (posit > 0) {
			posit=posit-1;
		}
		return;
	}
	if (KeyValue == 0x09 ||     // Tab
		KeyValue == 0x2e        // Delete
	) {
		return;
	}
	if ((KeyValue < 0x30 || KeyValue > 0x39) &&
		(KeyValue < 0x60 || KeyValue > 0x69) // numbers
	) {
		event.returnValue = false;
		return;
	}
	//if (posit++ == 3) {
	//	obj.form.elements[getIndex(obj)+1].focus();
	//}
}

function isInteger(s){
    for (var i = 0; i < s.length; i++){
        // Check that current character is number.
        var c = s.charAt(i);
        if (((c < "0") || (c > "9")))
			return false;
    }
    // All characters are numbers.
    return true;
}

function toggleCheckBoxes(target,obj)
{
	list=fetchObjectByAttributeValue('id',target,'INPUT');
    for (var i = 0; i < list.length; i++){
		list[i].checked=obj.checked;
    }
	return true;
}


function graysomething(self,flag)
{
	$('errorMessageBlock').hide()
	var inputs=fetchAllInputFields();
	for (var a = 0; a < inputs.length; a++)
	{
		if (inputs[a].id != self.id) {
			if (!flag)
				inputs[a].disabled = !(!(parseInt(self.value)));
			else
				inputs[a].disabled = !(!(!(parseInt(self.value))));
		}
	}
}

function openAddNewMacWin(event,id)
{
	var evt = getEvent(event);
	//if ($('cb_accessControlMode' + id).checked) {
		addMacWin.open();
		setActiveContent();
		$('new-mac1').focus();
	//}
	return false;
}

function toggleFields(self, flag)
{
	Form.enable(document.dataForm);
	var inputs=fetchAllInputFields();
	var id = ($('activeMode').value == '3' || $('activeMode').value == '4')?1:0;
	for (var a = 0; a < inputs.length; a++)
	{
		if (inputs[a].id != self.id) {
			inputs[a].disabled = !self.checked;
		}
		else if (flag) {
			inputs[a].disabled = !self.checked;
		}
		if (inputs[a].type=='checkbox' || inputs[a].type=='image' || inputs[a].type == 'hidden' || inputs[a].id=='addNewMac'+ id) {
			inputs[a].disabled=false;
		}
		if (inputs[a].type == 'image') {
			if (inputs[a].disabled == false) {
				inputs[a].src = inputs[a].src.replace('_off','_on');
			}
			else {
				inputs[a].src = inputs[a].src.replace('_on','_off');
			}
		}
	}
	if ($('addnewMac') != undefined) {
		$R(1, 7, true).each(function(value) {
			$('new-mac'+value).disabled=false;
		});
		$('addnewMac').disabled=false;
		$('cancel').disabled=false;
	}
}


function prepareURL(fourth, third, second, primary, id, iface, extra)
{
	if (top.master._disableAll != undefined && top.master._disableAll == true) {
		return ;
	}
	if (id && $S(id+'_'+iface) == -1 ) {
		alert("Please select a Profile!");
		return false;
	}
	if ((parent.parent.frames['master'].progressBar!= undefined) && (!parent.parent.frames['master'].progressBar.isOpen)) {
		parent.parent.frames['master'].progressBar.open();
	}

	setActiveContent(false);

	if ($('menu4')!=undefined)
		$('menu4').value=fourth;
	if ($('menu3')!=undefined)
		$('menu3').value=third;
	if ($('menu2')!=undefined)
		$('menu2').value=second;
	if ($('menu1')!=undefined)
		$('menu1').value=primary;
	if (id && $('mode7')!=undefined) {
		$('mode7').value=$S(id+'_'+iface);
		if ($('mode7').value == -1) {
			$('mode7').value=$S(id);
			if ($('mode7').value == -1) {
			}
		}
	}
	if (iface && $('mode8')!=undefined)
		$('mode8').value=iface;
	if (extra && $('mode9')!=undefined)
		$('mode9').value=extra;

	document.dataForm.target="master";
	document.dataForm.action="index.php?page=master";
	document.dataForm.submit();
}

function redirectHome(sessionCheck)
{
	var flag = false
	if (sessionCheck) {
		flag = confirm('Another Session is currently active!\nDo you want to terminate the remote session?');
		if (flag) {
			var oOptions = {
	            method: "get",
				asynchronous: false,
				parameters: { id: Math.floor(Math.random()*1000001) },
	            onSuccess: function (oXHR, oJson) {
					var response = oXHR.responseText;
					if(response != 'recreateok') {
						alert('Error deleting old session!');
						doLogout();
					}
					else {
						window.top.location.href = "index.php";
					}
	            },
	            onFailure: function (oXHR, oJson) {
					alert("There was an error with the connection, Please try again!"); 
	            }
	        };
			var req = new Ajax.Request('recreate.php', oOptions);
		}
		else {
			doLogout(false);
		}
	}
	else {
		window.top.location.href = "index.php";
	}
}

function doLogin(event)
{
    var ev = getEvent(event);
	if (parent.parent.frames['master'].progressBar != undefined)
	parent.parent.frames['master'].progressBar.open();
	Form.disable(document.dataForm);
	var oOptions = {
            method: "get",
			asynchronous: false,
			parameters: { username: $('username').value, password: $('password').value, id: Math.floor(Math.random()*1000001) },
            onSuccess: function (oXHR, oJson) {
				var response = oXHR.responseText;
				if(response == 'loginok') {
					//alert("Login OK!");
					redirectHome(false);
				}
				else if (response == 'sessionexists') {
					redirectHome(true);
				}
				else {
					if (parent.parent.frames['master'].progressBar != undefined)
			    		parent.parent.frames['master'].progressBar.close();
			    	$('br_head').innerHTML = 'Invalid Username / Password!';
			    	$('errorMessageBlock').style.display = 'block';
			    	$('username').value = '';
			    	$('password').value = '';
			    	Form.enable(document.dataForm);
                    $('username').focus();
                    ev.returnValue = false;
			    	return false;
				}
				return false;                
            },
            onFailure: function (oXHR, oJson) {
				alert("There was an error with the connection, Please try again!"); 
            }
        };
    var req = new Ajax.Request('login.php', oOptions);
}

function doLogout(flag)
{
	if (parent.parent.frames['master'].progressBar != undefined)
		parent.parent.frames['master'].progressBar.open();
	var oOptions = {
		method: "get",
		asynchronous: false,
		parameters: { logout: 'logout', emptySession: flag, id: Math.floor(Math.random()*1000001) },
		onSuccess: function (oXHR, oJson) {
			var response = oXHR.responseText;
			if(response == 'logoutok') {
				//alert("Logout OK!");
				window.top.location.href = "index.php";
			}
			else {
				setTimeout(function() { new Ajax.Request('logout.php', oOptions); },1000);
			    	return false;
			}
			return false;                
		},
		onFailure: function (oXHR, oJson) {
			alert("There was an error with the connection, Please try again!"); 
			parent.parent.frames['master'].progressBar.close();
		}
	};
	var req = new Ajax.Request('logout.php', oOptions);
}

function processLogout()
{
	doLogout();
}



function passwd_disable()
{
	document.dataForm.currentpassword.value = "";
	document.dataForm.adminpassword.value = "";
	document.dataForm.adminpasswordconfirm.value = "";

	document.dataForm.currentpassword.disabled = true;
	document.dataForm.adminpassword.disabled = true;
	document.dataForm.adminpasswordconfirm.disabled = true;
}


function passwd_enable()
{
	document.dataForm.currentpassword.disabled = false;
	document.dataForm.adminpassword.disabled = false;
	document.dataForm.adminpasswordconfirm.disabled = false;
}

function toggleSyslog(obj)
{
	var fields=fetchAllInputFields();
	for(i=0;i<fields.length;i++)
	{
		if (fields[i].id != obj.id)
			fields[i].disabled=!obj.checked;
	}
}

function changeLabel(block,str)
{
	obj=$('modeName'+block);
	//alert(obj.tagName);
	obj.innerHTML=str;
}

function showNewMacAddress()
{
	ableControls(false);
	$('AddMacLayer').style.display='block';
}

function hideNewMacAddress()
{
	$('AddMacLayer').style.display='none';
	ableControls(true);
}

function showBand(obj)
{
	ableInputFields(!obj.activeBand);
//	alert($('band2').hasClassName('legend'));
//	alert($('band5').hasClassName('legendActive'));
	//$('band5').toggleClassName('legendActive');
	//$('band5').toggleClassName('legend');
	//$('band2').toggleClassName('legend');
	//$('band2').toggleClassName('legendActive');
}

function ableInputFields(flag)
{
	if (flag) {
		Form.enable(document.dataForm);
	}
	else {
		Form.disable(document.dataForm);
	}
}

function ableControls(flag)
{
	inputs=fetchObjectByAttributeValue('type',"checkbox",'INPUT');
	for (var k=0;k < inputs.length; k++)
	{
		inputs[k].disabled=!flag;
		if (inputs[k].dependent) {
			if (flag)
				$(inputs[k].dependent).disabled=!inputs[k].checked;
			else
				$(inputs[k].dependent).disabled=true;
		}
	}
	$('refreshAWS').disabled = !flag;
	$('add').disabled = !flag;
	$('addmac').disabled = !flag;
	$('delete').disabled = !flag;
}

function DispProfiles(str)
{
	var obj1=fetchObjectByAttributeValue("mode","secA","TR");
	var obj2=fetchObjectByAttributeValue("mode","secNA","TR");

	if (str=="secA")
	{
		show(obj1);
		hide(obj2);
	}
	else
	{
		hide(obj1);
		show(obj2);
	}
}

function DisplaySettings(mode,val,option)
{
	if (val == undefined) {
		val = $('key_size_11g').value;
	}
	var obj=fetchObjectsByAttribute("mode","TR");
	var flag=false;

	if (mode=='4' || mode == '16') {
		var List=new Array('TKIP','TKIP + AES');
		var ListVal=new Array('2','6');
		$('encryption').value='2';
		$('wepKeyType').disabled=true;
		if (mode=='4' && option!=1) {
			flag=true;
		}
	}
	else if (mode=='8' || mode == '32') {
		var List=new Array('AES','TKIP + AES');
		var ListVal=new Array('4','6');
		$('encryption').value='4';
		$('wepKeyType').disabled=true;
		if (mode=='8' && option!=1) {
			flag=true;
		}
	}
	else if (mode=='12' || mode == '48') {
		var List=new Array('TKIP + AES');
		var ListVal=new Array('6');
		$('encryption').value='6';
		$('wepKeyType').disabled=true;
		if (mode=='12' && option!=1) {
			flag=true;
		}
	}
	else if (mode=='1' && option!=1) {
		var List=new Array('64 bit WEP','128 bit WEP', '152 bit WEP');
		var ListVal=new Array('64','128','152');
		$('wepKeyType').disabled=false;
		$('wepKeyType').value='64';
		$('encryption').value='1';
	}
	else if (mode!= '0' && option != 1) {
		var List=new Array('None');
		var ListVal=new Array('0');
		$('encryption').value='0';
		$('wepKeyType').disabled=true;
		flag = true;
	}
	else {
		var List=new Array('None','64 bit WEP','128 bit WEP', '152 bit WEP');
		var ListVal=new Array('0','64','128','152');
		$('encryption').value='0';
		$('wepKeyType').disabled=true;
	}


	var dataEnc = $('key_size_11g');
	var selEnc = dataEnc.value;
	do
		dataEnc.options[0] = null;
	while (dataEnc.length > 0);

	for (var j=0; j<List.length; j++)
	{
		var opt=document.createElement('OPTION');
		opt.text=List[j];
		opt.value=ListVal[j];
		if (selEnc == ListVal[j] && opt!=1 && mode!='0') {
			if (opt.value > 8) {
				$('wepKeyType').value=opt.value;
			}
			else {
				$('encryption').value=opt.value;
			}
			opt.selected="selected";
		}
//alert(opt.text+'======='+opt.value);
		try {
			dataEnc.add(opt, null); // standards compliant; doesn't work in IE
		}
		catch(ex) {
			dataEnc.add(opt); // IE only
		}
	}
	if (mode=='1') {
		gen_11g_keys();
	}

	for (var k=0; k < obj.length; k++) {
		if (obj[k].getAttribute("mode")==mode && dataEnc.value != 0) {
			showNenable(obj[k]);
		}
		else if ((mode=='16' || mode == '32' || mode == '48') && obj[k].getAttribute("mode")=='16') {
			showNenable(obj[k]);
		}
		else {
			hideNdisable(obj[k]);
		}
	}

	if (flag && $('radiusEnabled').value == 'false') {
		alert("To use this Network Authentication method, "+
			"you must first configure a Primary Authentication Server on the Radius Server Settings screen (Configuration -> Security -> Advanced -> Radius Server).");
	}

}


function showRadiusAlert(obj)
{
	if (obj.value == '2') {
		if ($('radiusEnabled')!= undefined && $('radiusEnabled').value=='false') {
			alert("To use this Network Authentication method, "+
				"you must first configure a Primary Authentication Server on the Radius Server Settings screen (Configuration -> Security -> Advanced -> Radius Server).");
			obj.value = '1';			
			obj.options[0].selected = true;
			event.returnValue = false;
			return false;
		}
	}
}

function wdsDisplaySettings(mode,val,option)
{
	if (val == undefined) {
		val = $('key_size_11g').value;
	}
	var obj=fetchObjectsByAttribute("mode","TR");
	var flag=false;

	if (mode == '2') {
		var List=new Array('TKIP');
		var ListVal=new Array('2');
		$('wepKeyType').disabled=true;
		$('encryption').value='2';
	}
	else if (mode=='4') {
		var List=new Array('AES');
		var ListVal=new Array('4');
		$('wepKeyType').disabled=true;
		$('encryption').value='4';
	}
	else if (mode=='1' && option!=1) {
		var List=new Array('64 bit WEP','128 bit WEP', '152 bit WEP');
		var ListVal=new Array('64','128','152');
		$('wepKeyType').disabled=false;
		$('wepKeyType').value='64';
		$('encryption').value='1';
	}
	else if (mode!= '0' && option != 1) {
		var List=new Array('None');
		var ListVal=new Array('0');
		$('encryption').value='0';
		flag = true;
	}
	else {
		var List=new Array('None','64 bit WEP','128 bit WEP', '152 bit WEP');
		var ListVal=new Array('0','64','128','152');
		$('encryption').value='0';
	}


	var dataEnc = $('key_size_11g');
	var selEnc = dataEnc.value;
	do
		dataEnc.options[0] = null;
	while (dataEnc.length > 0);

	for (var j=0; j<List.length; j++)
	{
		var opt=document.createElement('OPTION');
		opt.text=List[j];
		opt.value=ListVal[j];
		if (selEnc == ListVal[j] && opt!=1 && mode!='0') {
			if (opt.value > 8) {
				$('wepKeyType').value=opt.value;
			}
			else {
				$('encryption').value=opt.value;
			}
			opt.selected="selected";
		}
//alert(opt.text+'======='+opt.value);
		try {
			dataEnc.add(opt, null); // standards compliant; doesn't work in IE
		}
		catch(ex) {
			dataEnc.add(opt); // IE only
		}
	}
	for (var k=0; k < obj.length; k++) {
		if (obj[k].getAttribute("mode")==mode && dataEnc.value != 0) {
			showNenable(obj[k]);
		}
		else if ((mode=='2' || mode == '4') && obj[k].getAttribute("mode")=='4') {
			showNenable(obj[k]);
		}
		else {
			hideNdisable(obj[k]);
		}
	}

}


function setEncryption(val,mode)
{
	if (val > 8) {
		if ($('wepKeyType').disabled == true)
			$('wepKeyType').disabled=false;
		$('wepKeyType').value=val;
		if (mode == 0) {
			$('encryption').value='1';
		}
	}
	else {
		$('wepKeyType').disabled=true;
		$('encryption').value=val;
	}
	if (val >= 64) {
		gen_11g_keys();
	}
}

function hide(objList)
{
	for (var i=0; i < objList.length; i++)
	{
		objList[i].style.display="none";
		if (objList[i].style.visibility=="hidden")
			objList[i].style.visibility="visible";
	}
}

function show(objList)
{
	for (var i=0; i< objList.length; i++)
	{
		objList[i].style.display="block";
		if (objList[i].style.visibility=="visible")
			objList[i].style.visibility="hidden";
	}
}

function enable11nFields(mode, id)
{
	if (config.MODE11N.status) {
		if ($('datarate11n' + id) != undefined) {
			if (mode == 'hide') {
				hideTR(new Array($('bandwidth11n' + id), $('gi11n' + id), $('mcsrate11n' + id)));
				showTR(new Array($('datarate11n' + id)));
			}
			else {
				showTR(new Array($('bandwidth11n' + id), $('gi11n' + id), $('mcsrate11n' + id)));
				hideTR(new Array($('datarate11n' + id)));
			}
		}
	}
}

function hideTR(objList)
{
	for (var i=0; i < objList.length; i++)
	{
		objList[i].style.display="none";
		objList[i].disabled=true;
	}
}

function showTR(objList)
{
	for (var i=0; i< objList.length; i++)
	{
		try {
			objList[i].style.display="table-row";
		}
		catch(ex) {
			objList[i].style.display="block";
		}
		objList[i].disabled=false;

	}
}
function calpos(obj,x)
{
}

function toggleSshEnabled(opt)
{
}

function toggleSnmpEnabled(opt)
{
}

function hideNdisable(obj)
{
	obj.style.display="none";
	obj.disabled = true;
	$(obj).descendants().each(function (element) {
		element.disabled = true;
	});
	obj.getElementsByTagName('td')[1].getElementsByTagName('INPUT')[0].disabled=true;
}

function showNenable(obj)
{
	try {
		obj.style.display="table-row";
	}
	catch(e) {
		obj.style.display="block";
	}
	obj.disabled = false;
	$(obj).descendants().each(function (element) {
		element.disabled = false;
	});
	obj.getElementsByTagName('td')[1].getElementsByTagName('INPUT')[0].disabled=false;
}

function MacAddrEdit(event, obj)
{
	var KeyValue = event.keyCode;
	var str = String.fromCharCode(KeyValue);
	var maskAlp = /^([a-fA-F])*$/i
	var maskNum = /^([0-9])*$/i
	if (KeyValue == 0x08) {       // BackSpace
		if (posit > 0) {
			posit=posit-1;
		}
		return;
	}
	if (KeyValue == 0x09 ||     // Tab
		KeyValue == 0x2e        // Delete
	) {
		return;
	}
	if (!maskAlp.test(str) && !maskNum.test(str) ) {
		event.returnValue = false;
		return false;
	}
	if (maskNum.test(str) && event.shiftKey) {
		event.returnValue = false;
		return false;
	}

/*	if ((KeyValue < 0x30 || KeyValue > 0x39) &&
		//(KeyValue < 0x60 || KeyValue > 0x69) && // numbers
		(KeyValue < 0x41 || KeyValue > 0x46)    // 'a' to 'f' or 'A' to 'F'
	) {
		event.returnValue = false;
		return false;
	}*/

	  var t = document.selection.createRange();
	  var vt = t.text;
	  if (vt != '' && vt.length == 2 && obj.value.length == 2 && t.parentElement() == obj) {
		  obj.value = '';
	  }
	if (obj.value.length == 2) {
		$(obj.nextid).focus();
	}
}

function toggleChilds(mode)
{
	var inputs = fetchInputFieldsInTable('mode');
	for (var b = 0; b < inputs.length; b++) {
		if (inputs[b].id != obj.id)
			inputs[b].disabled = true;
	}
}

function toggleProfiles(index, id)
{
	if (returnVal) {
		var iface = (id == undefined) ? $('currentInterfaceNum').value : id;
		var table = $('profilesList_' + iface);
		for (var k = 1; k < table.rows.length; k++)
		 {
			if ((index + 1) >= k) {
				try {
					table.rows[k].style.display = "table-row";
				} 
				catch (e) {
					table.rows[k].style.display = "block";
				}
				if ((eval('typeof(disableWDSonChannel'+iface+')') == 'boolean') && (parseInt($('apMode' + iface).value) == 5 || parseInt($('apMode' + iface).value) == 0)) {
					Form.disable(document.dataForm);
					if (parseInt($('apMode' + iface).value) != 0) {
						$RD('wdsMode'+iface).each(function(input){
							input.disabled = false;
						});
					}
					if (config.CLIENT.status) {
						$RD('macClone'+iface).each(function(input) {
							input.disabled=false;
						});
						toggleMACCloneAddress($S('macClone'),iface);
					}

				    $('apMode'+iface).disabled=false;
					$('cb_wdsEnabled'+iface).disabled=false;
					$('wdsEnabled'+iface).disabled=false;
					enablePrimaryFields();
				}
			}
			else {
				table.rows[k].style.display = "none";
			}
		}
		if (index == '1') {
			$('WCArow_' + iface).style.visibility = "visible";
		}
		else {
			if (index == '0') {
				if (config.CLIENT.status)
					$('macClone_' + iface).style.display = "block";
				$('WCArow_' + iface).style.visibility = "hidden";
				$('profilesList_'+iface).style.display = "none";
			}
			else {
				$('WCArow_' + iface).style.visibility = "hidden";
				if (config.CLIENT.status)
					$('macClone_' + iface).style.display = "none";
				$('profilesList_' + iface).style.display = "block";
			}
		}
		if ((eval('typeof(disableWDSonChannel'+iface+')') == 'boolean') && (parseInt($('apMode'+iface).value) == 5 || parseInt($('apMode'+iface).value) == 0))
		{
			//$('profilesList_'+iface).disabled = "true";
			try {
				$('WCArow_' + iface).style.display = "table-row";
			} 
			catch (e) {
				$('WCArow_' + iface).style.display = "block";
			}
			$('WCArow_' + iface).disabled = false;
		}
		else{
			if (parseInt($('apMode' + iface).value) != 5) {
				$('profilesList_' + iface).style.display = "block";
				$('profilesList_'+iface).disabled = false;
			}
			else {
				$('profilesList_' + iface).style.display = "none";
			}
		}
	}
	else {
		return false;
	}
}


function getCheckedRadioValue(name) {
	var radioButtons = document.getElementsByName(name);
	for (var x = 0; x < radioButtons.length; x ++) {
		if (radioButtons[x].checked) {
			return radioButtons[x].value;
		}
	}
}

function getCheckedRadio(name) {
	var radioButtons = document.getElementsByName(name);
	for (var x = 0; x < radioButtons.length; x ++) {
		if (radioButtons[x].checked) {
			return radioButtons[x];
		}
	}
}

function getCheckedRadioById(id) {
	$RD(id).each (function(radio) {
		if (radio.id == id && radio.checked) {
			return radio;
		}
	});
}
var radioValChanged = true;
function setRadioCheckById(id, val, setVal) {
	$RD(id).each (function(radio) {
		radio.checked = (radio.value==val);
		if (radio.value==val) {
			radio.value = (setVal==undefined)?val:setVal;
			radioValChanged = (setVal==undefined)?false:true;
		}
	});
}

var returnVal = true;

function setApMode(val, id)
{
	var iface = (id == undefined)?$('currentInterfaceNum').value:id;
	if (($('apMode' + iface).value == 5 || $('apMode' + iface).value == '5') && $S('macClone'+iface) == 1 && $('macCloneAddr'+iface).value == '' && val != 0) {
		$RD('wdsMode'+iface).each(function(radio) {
			if (radio.value == $('apMode' + iface).value) {
				radio.checked=true;
			}
			else {
				radio.checked=false;
			}
		});
		if (config.CLIENT.status) {
			$('macCloneAddr'+iface).focus();
			$('macCloneAddr'+iface).blur();
		}
		returnVal = false;
		return false;
	}
	else{ 
		$('apMode'+iface).value=val;
		returnVal = true;
	}
}

function enableProfiles(flag,id)
{
	var iface = (id == undefined)?$('currentInterfaceNum').value:id;
	var inputs = Form.getElements(document.dataForm);
	for (var k=0;k < inputs.length; k++)
	{
		if (inputs[k].id != 'cb_wdsEnabled'+iface) {
			inputs[k].disabled=!flag;
			if (inputs[k].dependent) {
				if (flag)
					$(inputs[k].dependent).disabled=!inputs[k].checked;
				else
					$(inputs[k].dependent).disabled=true;
			}
		}
	}
}

function activateSubTab(obj, id, mainId, tabCount, iface){
	if (returnVal) {
		tabCount = (config.CLIENT.status)?4:3;
		iface = (iface == undefined)? $('currentInterfaceNum').value: iface;
		for (var i = 1; i <= tabCount; i++) {
			if (id == i) {
				$(mainId + i).className = "Active";
			}
			else {
				$(mainId + i).className = "";
			}
		}
		$('WCAenabled' + iface).checked = (($('apMode' + iface).value == 2) || ($('apMode' + iface).value == 0));
		toggleProfiles((id==0)?0:4, iface);
		setActiveContent();
	
		if ((eval('typeof(disableWDSonChannel'+iface+')') == 'boolean')) {
			if ($('apMode' + iface).value == 5) {
				$('errorMessageBlock').hide();
				activateApply();
				if (config.CLIENT.status) {
					$RD('macClone'+iface).each(function(input) {
						input.disabled=false;
					});
					toggleMACCloneAddress($S('macClone'),iface);
				}
			}
			else {
				$('br_head').innerHTML = 'Bridging cannot be enabled with channel set to Auto!';
				$('errorMessageBlock').show();
				activateApply(false);
			}
		}
		else {
			if ($('apMode' + iface).value == 5) {
				window.top.frames['action'].$('edit').src = 'images/edit_off.gif';
				window.top.frames['action'].$('edit').disabled = true;
				if (config.CLIENT.status) {
					$RD('macClone'+iface).each(function(input) {
						input.disabled=false;
					});
					toggleMACCloneAddress($S('macClone'),iface);
				}
			}
			else {
				window.top.frames['action'].$('edit').src = 'images/edit_on.gif';
				window.top.frames['action'].$('edit').disabled = false;
			}
		}
	}
	else {
		return false;
	}
}

function checkWDSStatus(obj, id)
{
	if (typeof(obj)!='object') {
		delete obj;
		var obj = window.top.frames['master'].$('ChannelList'+((id=='1')?'2':'1'));
	}
	if (obj.value == '0' && eval('typeof(disableChannelonWDS'+id+') == "boolean"'))
	{
		$('br_head').innerHTML = 'Channel cannot be set to Auto with Wireless Bridge enabled!';
		$('errorMessageBlock').show();
		return false;
	}
	else {
		$('br_head').innerHTML = '';
		$('errorMessageBlock').hide();
		return true;
	}
}

function wdsOnEnable(flag, id, enable)
{
	if (id == undefined)
		id = $('currentInterfaceNum').value;
	var inputs=fetchAllInputFields();
	
	if (!enable) {
		setApMode((flag)?'2':'0', id);
		if (!flag) {
			setRadioCheckById('wdsMode'+id,((radioValChanged)?2:1),2);
			activateSubTab(getCheckedRadioById('wdsMode'+id),'1','includeSubTab'+id,4,id);
			var editButton = window.top.frames['action'].$('edit');
			editButton.src = 'images/edit_off.gif';
			editButton.disabled = true;
			toggleProfiles(1,id);
		}
	}
	if ($('cb_wdsEnabled'+id).checked) {
		Form.enable(document.dataForm);
		enableProfiles($('cb_wdsEnabled'+id).checked);
		if (config.CLIENT.status) {
			toggleMACCloneAddress($S('macClone'+id),id);
		}
		if (eval('typeof(disableWDSonChannel'+id+')') == 'boolean') {
			activateApply(false);
			window.top.frames['action'].$('edit').src = 'images/edit_off.gif';
			window.top.frames['action'].$('edit').disabled = true;
		}
		if ((eval('typeof(disableWDSonChannel'+id+')') == 'boolean') && parseInt($('apMode'+id).value) != 5 && parseInt($('apMode'+id).value) != 0)	{
			$('br_head').innerHTML = 'Bridging cannot be enabled with channel set to Auto!';
			$('errorMessageBlock').show();
			window.top.frames['action'].$('edit').src = 'images/edit_off.gif';
			window.top.frames['action'].$('edit').disabled = true;
			Form.disable(document.dataForm);
			$RD('wdsMode'+id).each (function(input) {
  					input.disabled=false;
			});
		}
		else{
			window.top.frames['action'].$('edit').src = 'images/edit_on.gif';
			window.top.frames['action'].$('edit').disabled = false;
		}
	}
	else {
		Form.disable(document.dataForm);
		window.top.frames['action'].$('edit').disabled = true;
		$('errorMessageBlock').hide();
		activateApply();
	}
    var x = (parseInt($('apMode'+id).value)<=2)?1:(parseInt($('apMode'+id).value)-1);
    if (window.top.frames['action'].$('edit').disabled == false || window.top.frames['action'].$('edit').disabled == 'false') {
        window.top.frames['action'].$('edit').src = 'images/edit_on.gif';
    }
    else {
        window.top.frames['action'].$('edit').src = 'images/edit_off.gif';
    }

    $('apMode'+id).disabled=false;
	$('cb_wdsEnabled'+id).disabled=false;
	$('wdsEnabled'+id).disabled=false;
	enablePrimaryFields();
}

function integralityOnEnable() {
	if ($('gateway') != undefined && ($('gateway').value == '' && $('cb_networkintegrality').checked)) {
			$('br_head').innerHTML = 'Integrality check cannot be enabled with default gateway empty!';
			$('errorMessageBlock').show();
			$('cb_networkintegrality').checked = false;
			$('networkintegrality').value = "0";
			setActiveContent(false);
			return false;
	}
	else {
			$('errorMessageBlock').hide();
	}
}

function checkNICStatus(obj)
{
	if (config.NETWORK_INTEGRALITY.status) {
		if ($('gateway').value == '' &&  $('cb_networkintegrality').checked) {
			$('br_head').innerHTML = 'Gateway cannot be empty with Network Integrality enabled!';
			$('errorMessageBlock').show();
			setActiveContent(false);
			obj.focus();
			return false;
		}
		else {
			$('br_head').innerHTML = '';
			$('errorMessageBlock').hide();
		}
	}
}

function showHelp(tlt,hlpurl)
{
	var w='550', h='500';
	features="resizable=yes";
	var winl = (screen.width-w)/2; var wint = (screen.height-h)/2;
	if (winl < 0) winl = 0;
	if (wint < 0) wint = 0;
	var settings = 'height=' + h + ',';
	settings += 'width=' + w + ',';
	settings += 'top=' + wint + ',';
	settings += 'left=' + winl + ',';
	settings += features;
	win = window.open("help/help.html#"+hlpurl,hlpurl,settings);
	win.window.focus();
}

var win = null;

function showHelpwindow()
{
//	alert('help_'+window.opener.$('helpURL').value+'.html#'+window.name);
	document.getElementById('maincontent').contentWindow.location.replace('help_'+window.opener.$('helpURL').value+'.html#'+window.name);
}

function deleteRows(id, index)
{
	var table = $(id);
	var j=0;
	var flag = false;
	var deletedMacs=new Array();
	var tmpText = '';
	for (var i=0;i<table.rows.length;i++)
	{
		var rowObj=table.rows[i].cells[0].childNodes;
		for (var k=0; k < rowObj.length; k++)
		{
			if (rowObj[k].tagName == 'INPUT') {
				if (rowObj[k].checked == true) {
					if(table.rows[i].id!='headRow') {
						try {
							tmpText = table.rows[i].cells[1].innerText.toUpperCase();
						}
						catch(err) {
							tmpText = table.rows[i].cells[1].textContent.toUpperCase();
						}
						deletedMacs.push(tmpText);
						//deletedMacs.push(table.rows[i].cells[1].innerText);
						table.deleteRow(i);
						j++;
						i=i-1;
					}
					else {
						rowObj[k].checked=false;
					}
					flag = true;
				}
			}
		}
	}
	if (!flag) {
		alert('No rows selected!');
		return false;
	}

	$('rowCount'+index).value=parseInt($('rowCount'+index).value)-j;
	deletedMacs.each (function(mac) {
		if ($('addedAPs'+index) != undefined) {
			var str = $('addedAPs'+index).value.toUpperCase();
			var flag = str.match(mac);
			var string = str.replace('\''+mac+'\',','');
			$('addedAPs'+index).value = string;
			if (!flag)
				$('deletedAPs'+index).value = $('deletedAPs'+index).value + '\''+mac+'\',';
		}
		else if ($('addedStations'+index) != undefined) {
			var str = $('addedStations'+index).value.toUpperCase();
			var flag = str.match(mac);
			var string = str.replace('\''+mac+'\',','');
			$('addedStations'+index).value = string;
			if (!flag)
				$('deletedStations'+index).value = $('deletedStations'+index).value + '\''+mac+'\',';
		}
	});
	$('div_'+id).style.height = ($(id).rows.length > 5)?'140px':'auto';
	reAlternateLines(id);
	setActiveContent();
}

function toggleDisplay(show)
{
    var disabled=false;
	if (show == undefined || show == '') {
		if ($('menu1').value != 'Monitoring') {
			if (config.TWOGHZ.status) {
				show="wlan1";
			}
			else {
				if (config.FIVEGHZ.status) {
					show="wlan2";
				}
			}
			disabled = true;
			var mode = '';
			if (config.TWOGHZ.status) {
				mode = (config.MODE11G.status)?((config.MODE11N.status)?'2':'1'):'0';
			}
			else if (config.FIVEGHZ.status) {
				mode = (config.MODE11N.status)?'4':'3'
			}
		}
		else {
			show="wlan0";
		}
	}
	var listArr = new Array(1,2);
	if (mode == undefined && $('activeMode') != undefined) {
		var mode = $('activeMode').value;
	}
    for (var i=0; i<listArr.length; i++)
	{
		var bandRow = $('wlan'+listArr[i]);
		if (bandRow == undefined)
			continue;
//alert(show+"------"+('wlan'+listArr[i])+"--------"+bandRow);
		if (bandRow.id == show) {
			if ($('currentInterfaceNum')!= undefined) {
				$('currentInterfaceNum').value=parseInt(listArr[i])-1;
			}
            try {
                bandRow.style.display="table-row";
            }
            catch(e) {
                bandRow.style.display="block";
            }
		}
		else {
			bandRow.style.display="none";
		}
		if (disabled) {
			if (config.TWOGHZ.status) {
				$('inlineTab1').className = "Active";
				$('inlineTab1').blur();
			}
			if (config.FIVEGHZ.status) {
				if (!config.TWOGHZ.status) {
					$('inlineTab2').className = "Active";
					$('inlineTab2').blur();
				}
				else {
					$('inlineTab2').className = "";
				}
			}
			Form.disable(document.dataForm);
			if (config.TWOGHZ.status && $('cb_chkRadio0') != undefined) {
				$('cb_chkRadio0').disabled=false;
				$('chkRadio0').disabled=false;
				if (mode <= 2) {
					$('modeWlan0').value=mode;
					enableFields(mode);
				}
				enable11nFields((mode == '2')?'show':'hide',1);
				DispChannelList(1,mode,'chkRadio0');
				$RD('WirelessMode1').each(function(radio) {
					radio.disabled=false;
					if (radio.value==mode){
						Event.emulateClick(radio);
					}
				});
			}
			if (config.FIVEGHZ.status && $('cb_chkRadio1') != undefined) {
				$('cb_chkRadio1').disabled=false;
				$('chkRadio1').disabled=false;
				if (mode >= 3) {
					$('modeWlan1').value=mode;
					enableFields(mode);
				}
				enable11nFields((mode == '4')?'show':'hide',2);
				DispChannelList(2,mode,'chkRadio1');
				$RD('WirelessMode1').each(function(radio) {
					radio.disabled=false;
					if (radio.value==mode){
						Event.emulateClick(radio);
					}
				});
			}
		}
	}
    if ($('currentInterfaceNum') != undefined) {
		var iface = $('currentInterfaceNum').value;
        if (($('apMode'+ iface)!= undefined) && ($('apMode'+ iface) != undefined)) {
			toggleProfiles(($('apMode'+iface).value<=2)?1:4,iface);
	    }
    }
    enablePrimaryFields();
}

function setActiveMode (obj, name, flag)
{
	var currentRadio = $S(name);
	var str, str2, alertFlag = false;
	if (currentRadio == -1) {
		//alert('Please Select a Wireless Mode!');
		obj.checked = false;
	}
	else {
        var modeString = new Array('b','bg','ng','a','na');
		if (!config.TWOGHZ.status) {
			modeString.splice(0, 3, "");
		}
		if (!config.FIVEGHZ.status) {
			modeString.splice(3,2, "");
		}
        var flag1 = true;
        if ($('activeMode').value != currentRadio && flag && $('activeMode').value != '') {
            var flag1 = confirm("Do you want to disable 11"+modeString[$('activeMode').value]+" mode and enable 11"+modeString[currentRadio]+" mode?\n\nNote: Click APPLY for the changes to take effect.");
            if (flag1)
                alertFlag = true;
        }
        else if ($('activeMode').value == '' && obj.checked != false) {
        	alertFlag=true;
        }
        if (flag1) {
            var group = fetchObjectByAttributeValue('id',name,'INPUT');
            var str="802.11";
            var str2="802.11";
            var count=0, count2=0;
            if (obj.checked) {
                if ($('activeMode').value!=currentRadio) {
                    if (currentRadio+1 != false && flag) {
                        $('activeMode').value=currentRadio;
                    }

                    for (i=0; i<group.length; i++) {
                        var groupMode = 'mode_'+modeString[group[i].value];
                        if (parseInt($('activeMode').value) == parseInt(group[i].value)) {
                            if ($(groupMode)) {
                                $(groupMode).className = "Active";
                                Event.observe($(groupMode),"mouseover",function() { showLayer(this); });
                                Event.observe($(groupMode),"mouseout",function() { hideLayer(this); });
                                $(groupMode).innerHTML="11"+modeString[group[i].value]+"<img src='../images/activeRadio.gif'><span>Radio is set to 'ON'</span>";
                                if (config.TWOGHZ.status && group[i].value != '3' && group[i].value != '4') {
                                    str=str+"<span class='Active' onmouseover='showLayer(this);' onmouseout='hideLayer(this);'><b class='RadioTextActive'>"+modeString[group[i].value]+"<img src='../images/activeRadio.gif'><span>Radio is set to 'ON'</span></b></span>";
                                    if (count++ < 2)
                                        str=str+'/';
                                }
                                if (config.FIVEGHZ.status && group[i].value != '0' && group[i].value != '1' && group[i].value != '2') {
                                    str2=str2+"<span class='Active' onmouseover='showLayer(this);' onmouseout='hideLayer(this);'><b class='RadioTextActive'>"+modeString[group[i].value]+"<img src='../images/activeRadio.gif'><span>Radio is set to 'ON'</span></b></span>";
                                    if (count2++ < 1)
                                        str2=str2+'/';
                                }
                            }
                        }
                        else {
                            if ($(groupMode)) {
                                $(groupMode).className = "";
                                $(groupMode).innerHTML="11"+modeString[group[i].value];
                                if (config.TWOGHZ.status && group[i].value != '3' && group[i].value != '4') {
                                    str=str+"<span class='Active'><b class='RadioText'>"+modeString[group[i].value]+"</b></span>";
                                    if (count++ < 2)
                                        str=str+'/';
                                }
                                if (config.FIVEGHZ.status && group[i].value != '0' && group[i].value != '1' && group[i].value != '2') {
                                    str2=str2+"<span class='Active'><b class='RadioText'>"+modeString[group[i].value]+"</b></span>";
                                    if (count2++ < 1)
                                        str2=str2+'/';
                                }
                            }
                        }
                    }
					if (config.TWOGHZ.status) {
						$('inlineTabLink1').update(str);
					}
					if (config.FIVEGHZ.status) {
						$('inlineTabLink2').update(str2);
					}
                }
                if ($('br_head').innerText == 'Wireless Radio is turned off!') {
                    $('errorMessageBlock').style.display = 'none';
                }
            }
            else {
                if (flag) {
                    $('activeMode').value = '';
                    for (i=0; i<group.length; i++) {
                        var groupMode = 'mode_'+modeString[group[i].value];
                        if ($(groupMode)) {
                            $(groupMode).className = "";
                            $(groupMode).innerHTML="11"+modeString[group[i].value];
							if (config.TWOGHZ.status && group[i].value != '3' && group[i].value != '4') {
                                str=str+"<span class='Active'>"+modeString[group[i].value]+"</span>";
                                if (count++ < 2)
                                    str=str+'/';
                            }
							if (config.FIVEGHZ.status && group[i].value != '0' && group[i].value != '1' && group[i].value != '2') {
                                str2=str2+"<span class='Active'>"+modeString[group[i].value]+"</span>";
                                if (count2++ < 1)
                                    str2=str2+'/';
                            }
                        }
                    }
					if (config.TWOGHZ.status) {
						$('inlineTabLink1').update(str);
					}
					if (config.FIVEGHZ.status) {
						$('inlineTabLink2').update(str2);
					}
                }
            }
        }
    }
    if (alertFlag) {
        $('br_head').innerHTML = 'Click APPLY for the changes to take effect!';
        $('errorMessageBlock').show();
    }
    else {
		if ($('activeMode').value == '') {
			if (config.TWOGHZ.status) {
				$('chkRadio0').value='0';
			}
			if (config.FIVEGHZ.status) {
				$('chkRadio1').value='0';
			}
		}
	}
    enableFields($S(name));
}

function enableFields(mode)
{
	var inputs=fetchAllInputFields();
	var fieldsList1 = ["idbroadcastSSID1","ChannelList1","DatarateList1","MCSrateList1","Bandwidth1","GI1","PowerList1"];
	var fieldsList2 = ["idbroadcastSSID2","ChannelList2","DatarateList2","MCSrateList2","Bandwidth2","GI2","PowerList2"];
	for (var a = 0; a < inputs.length; a++)
	{
		if (inputs[a].id != 'WirelessMode1') {
		//if ($('activeMode').value != $S('WirelessMode1') && (inputs[a].id == 'chkRadio0' || inputs[a].id == 'chkRadio1'))
		 	if ($('activeMode').value == mode) {
				if (inputs[a].id == 'cb_chkRadio0' || inputs[a].id == 'cb_chkRadio1') {
					inputs[a].checked = true;
					if (mode == '3'||mode == '4') {
						if (inputs[a].id == 'cb_chkRadio0')
							$(inputs[a].id.replace('cb_','')).value='0';
					}
					else if (inputs[a].id == 'cb_chkRadio1') {
							$(inputs[a].id.replace('cb_','')).value='0';
					}
				}
				inputs[a].disabled = false;
			}
			else if ($('activeMode').value == '') {
				if (inputs[a].id == 'cb_chkRadio0' || inputs[a].id == 'cb_chkRadio1') {
					inputs[a].checked = false;
					inputs[a].disabled = false;
				}
				else {
					inputs[a].disabled = true;
				}
			}
			else {
				if (inputs[a].id == 'cb_chkRadio0' || inputs[a].id == 'cb_chkRadio1') {
					inputs[a].checked = false;
					//$(inputs[a].id.replace('cb_','')).value='0';
				}
                else
                    inputs[a].disabled = true;
			}
		}
		if ($('activeMode').value == mode) {
			if (config.TWOGHZ.status) {
				if ($('apMode0') != undefined && $('apMode0').value == 5) {
					fieldsList1.each( function(id) {
						if (id == inputs[a].id)
							inputs[a].disabled=true;
					});
				}
				else {
					inputs[a].disabled = false;
				}
			}
			if (config.FIVEGHZ.status) {
				if ($('apMode1') != undefined && $('apMode1').value == 5) {
					fieldsList2.each( function(id) {
						if (id == inputs[a].id)
							inputs[a].disabled=true;
					});
				}
				else {
					inputs[a].disabled = false;
				}
			}
		}		
	}
	//setActiveContent();
}

function checkMacAlreadyExists(tableId, macId)
{
	var mainTable=$(tableId);
	var tmpText = '';
	for (var i=0;i<mainTable.rows.length;i++) {
		try {
		     tmpText = mainTable.rows[i].cells[1].innerText.toUpperCase();
		}
		catch(err) {
		    tmpText = mainTable.rows[i].cells[1].textContent.toUpperCase();
		}
		if (tmpText == macId.toUpperCase()) {
			return false;
		}
	}
	return true;
}

function addMacRow(macId, id)
{
	if (!id)
		var id = $('currentInterfaceNum').value;

	if (!checkMacAlreadyExists('trustedTable'+id, macId)) {
		alert("MAC Address already exists!")
		return false;
	}
	tableObj=$('trustedTable'+id);
	e=document.createElement("tr");

	f=document.createElement("td");
	var classString='';
	if (($('rowCount'+id).value % 2)==1)
		classString="Alternate";

	f.className=classString;
	objtxt=document.createElement("input");
	objtxt.setAttribute("type","checkbox");
	objtxt.setAttribute("id","trustStation"+id);

	f.appendChild(objtxt);
    e.appendChild(f);
//For Delete Image
	f=document.createElement("td");
	if (($('rowCount'+id).value % 2)==1)
		classString="Alternate";

	f.className=classString;
	f.innerHTML = macId;

	//f.appendChild(objtxt);

	e.appendChild(f);
	try {
		rowAdded = tableObj.childNodes[1].appendChild(e);
	}
	catch(err) {
		rowAdded = tableObj.childNodes[0].appendChild(e);
	}

	new Effect.Highlight(rowAdded, {keepBackgroundImage:true});

	$('addedStations'+id).value = $('addedStations'+id).value+'\''+macId+'\',';

	$('rowCount'+id).value=parseInt($('rowCount'+id).value)+1
	//alert(tableObj.innerHTML);
	setActiveContent();
	return true;
}

function addTrustedStation(list, id, event){
	var ev = getEvent(event);
	var macId = '';
	if (inputForm && inputForm.formLiveValidate()) {
		var id = ($('activeMode').value == '3' || $('activeMode').value == '4') ? 1 : 0;

		if (list) {
			var table = $('avblStationList' + id);
			var flag = false;
			for (var i = 0; i < table.rows.length; i++) {
				var rowObj = table.rows[i].cells[0].childNodes;
				for (var k = 0; k < rowObj.length; k++) {
					if (rowObj[k].tagName == 'INPUT') {
						if (rowObj[k].checked == true && table.rows[i].id != 'headRow') {
							//alert(rowObj[k].checked);
							macId = table.rows[i].cells[2].innerHTML.toUpperCase();
							addMacRow(macId);
							table.deleteRow(i);
							i = i - 1;
							flag = true;
						}
						else {
							if (rowObj[k].checked) {
								rowObj[k].checked = false;
							}
						}
					}
				}
			}
			if (!flag)
				alert("Please select atleast one Station!")
		}
		else {
			macId = macId + $('addNewMac' + id).value.toUpperCase();
			if (macId == '00:00:00:00:00:00' || macId.length != 17) {
				alert("Invalid MAC Address!")
				return false;
			}
			addMacRow(macId);
			$('addNewMac' + id).value = '';
			//addMacWin.close();
		}
		$('div_avblStationList' + id).style.height = ($('avblStationList' + id).rows.length > 5) ? '140px' : 'auto';
		$('div_trustedTable' + id).style.height = ($('trustedTable' + id).rows.length > 5) ? '140px' : 'auto';
		reAlternateLines('avblStationList' + id);
		reAlternateLines('trustedTable' + id);
		setActiveContent();
	}
	Event.stop(ev);
}

function addKnownAP(list,id)
{
	if (list) {
		var table = $('unknownAPList'+id);
		var flag = false;
		for (var i=0;i<table.rows.length;i++)
		{
			var rowObj=table.rows[i].cells[0].childNodes;
			if (rowObj[0].tagName == 'INPUT' && rowObj[0].checked == true)	{
				if (table.rows[i].id!='headRow') {
					//alert(table.rows[i].innerHTML);
					addRow('knownAPList'+id,table.rows[i],id);
					//table.deleteRow(i);
					i=i-1;
				}
				else {
					rowObj[0].checked=false;
				}
				flag =  true;
			}
		}
		if (!flag) {
			alert("Please Select atleast one AP!")
		}
	}
	$('div_knownAPList'+id).style.height = ($('knownAPList'+id).rows.length > 5)?'140px':'auto';
	$('div_unknownAPList'+id).style.height = ($('unknownAPList'+id).rows.length > 5)?'140px':'auto';
	reAlternateLines('unknownAPList'+id);
	reAlternateLines('knownAPList'+id);
	//alert($('knownAPList').innerHTML);
	setActiveContent();
}

function addRow(tableID, rowObject, id)
{
	var tableObj = $(tableID);
	var e;
	var cnt = rowObject.cells.length;
	var tmpText = '';
	convert2KnownAP(rowObject,id);
	try {
		rowOb = tableObj.childNodes[1].appendChild(rowObject);
	}
	catch(err) {
		rowOb = tableObj.childNodes[0].appendChild(rowObject);
	}
	new Effect.Highlight(rowOb, {keepBackgroundImage:true});
	$('rowCount'+id).value=parseInt($('rowCount'+id).value)+1;

	try {
		tmpText = rowOb.cells[1].innerText.toUpperCase();
	}
	catch(err) {
		tmpText = rowOb.cells[1].textContent.toUpperCase();
	}
	$('addedAPs'+id).value = $('addedAPs'+id).value+'\''+ tmpText +'\',';
}

function convert2KnownAP(rowObj, id)
{
	inputObj=rowObj.cells[0].childNodes[0];
	inputObj.id="knownAPitem"+id;
	inputObj.checked=false;
}

function getposOffset(what, offsettype)
{
	var totaloffset=(offsettype=="left")? what.offsetLeft : what.offsetTop;
	var parentEl=what.offsetParent;
	while (parentEl!=null){
			totaloffset=(offsettype=="left")? totaloffset+parentEl.offsetLeft : totaloffset+parentEl.offsetTop;
			parentEl=parentEl.offsetParent;
	}
	return totaloffset;
}

function showLayer(obj)
{
	if ((layerDiv = $(obj).down(2)) != undefined) {
		layerDiv.style.top = ($(obj).viewportOffset().top - 38) + 'px';
		layerDiv.style.left = ($(obj).viewportOffset().left - 10) + 'px';
		layerDiv.style.display = 'block';
	}
	else if ((layerDiv = $(obj).down(1)) != undefined) {
		layerDiv.style.top = ($(obj).viewportOffset().top - 38) + 'px';
		layerDiv.style.left = ($(obj).viewportOffset().left - 10) + 'px';
		layerDiv.style.display = 'block';
	}
}

function hideLayer(obj)
{
	if ($(obj).down(2) != undefined)
		$(obj).down(2).style.display = 'none';
	else if ($(obj).down(1) != undefined)
		$(obj).down(1).style.display = 'none';
}
