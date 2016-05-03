/******************************************/
/*change background of leftmenu           */
/******************************************/
function change_background(obj)
{
	var tempTrName = "";
	for(var i=1; i<10; i++)
	{
		tempTrName = "tr"+String(i);
  		document.getElementById(tempTrName).className="";
	}
	var trName = "tr"+String(obj);
  	document.getElementById(trName).className="orange";
}

/******************************************/
/*wizard display control                  */
/******************************************/
function wizard_display(obj){
	if(parseInt(obj) == 1){
		var TD1 = document.getElementById("myTd1");
		TD1.innerHTML = "&nbsp;";
		var TD2 = document.getElementById("myTd2");
		TD2.display = "";
		TD2.width = 500;
		TD2.innerHTML = "<input id='but' type='button' value='Next' onclick='wizard_display(2);'/>";
	}
	else if(parseInt(obj) == 5){
		var TD1 = document.getElementById("myTd1");
		TD1.display = "";
		TD1.width = 430;
		TD1.innerHTML = "<input id='but' type='button' value='Back' onclick='wizard_display(4);'/>";
		var TD2 = document.getElementById("myTd2");
		TD2.display = "";
		TD2.width = 70;
		TD2.innerHTML = "<input id='but' type='submit' name='wizard_commit' value='Finish'/>";
	}
	else{
		var TD1 = document.getElementById("myTd1");
		TD1.display = "";
		TD1.width = 430;
		TD1.innerHTML = "<input id='but' type='button' value='Back' onclick='wizard_display(" + parseInt(obj-1) +");'/>";
		var TD2 = document.getElementById("myTd2");
		TD2.display = "";
		TD2.width = 70;
		TD2.innerHTML = "<input id='but' type='button' value='Next' onclick='wizard_display(" + parseInt(obj+1) +");'/>";
	}
		
	for(var i=1;i<6;i++){
		var tmpName = "wizard" + String(i);
		if(parseInt(obj) == i){
			document.getElementById(tmpName).style.display = "";
		}
		else{
			document.getElementById(tmpName).style.display = "none";
		}
	}
}



/******************************************/
/*3g_wan display control                  */
/******************************************/
function wizard_display_for_3g_wan(obj){

	if(parseInt(obj) == 1){
		var TD1 = document.getElementById("myTd1");
		
		TD1.innerHTML = "&nbsp;";
		var TD2 = document.getElementById("myTd2");
		TD2.style.display = "";
		TD2.width = 500;
		TD2.innerHTML = "<input id='but' type='button' value='Next' onclick='wizard_display_for_3g_wan(2);'/>";
		document.getElementById("3g_wan_commit").style.display = "none";
	}
	else if(parseInt(obj) == 4){
		
		var TD1 = document.getElementById("myTd1");
		
		TD1.style.display = "";
		TD1.width = 500;
		TD1.innerHTML = "<input id='but' type='button' value='Back' onclick='wizard_display_for_3g_wan(3);'/>";
		var TD2 = document.getElementById("myTd2");
		TD2.style.display = "none";
		TD2.width = 70;
		TD2.innerHTML = "<input id='but' type='submit' name='wizard_commit' value='Finish'/>";
		document.getElementById("3g_wan_commit").style.display = "";
	}
	else{
		var TD1 = document.getElementById("myTd1");
		TD1.style.display = "";
		TD1.width = 430;
		TD1.innerHTML = "<input id='but' type='button' value='Back' onclick='wizard_display_for_3g_wan(" + parseInt(obj-1) +");'/>";
		var TD2 = document.getElementById("myTd2");
		TD2.style.display = "";
		TD2.width = 70;
		TD2.innerHTML = "<input id='but' type='button' value='Next' onclick='wizard_display_for_3g_wan(" + parseInt(obj+1) +");'/>";
		document.getElementById("3g_wan_commit").style.display = "none";
	}
		
	for(var i=1;i<5;i++){
		var tmpName = "wizard" + String(i);
		if(parseInt(obj) == i){
			document.getElementById(tmpName).style.display = "";
		}
		else{
			document.getElementById(tmpName).style.display = "none";
		}
	}
}
