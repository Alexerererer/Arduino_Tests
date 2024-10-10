static const char PROGMEM INDEX_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>esp8266</title>
<style type="text/css">
.mbody {
width: 100%;
background-color: black;
}

.mhead {
left: 0px;
top: 3px;
position: absolute;
display: block;
float: left;
width: 220px;
border: 1px solid grey;
}
.mhead span {
display: inline-block;
float: left;
color: #FFFFFF;
margin-left: 3px;
margin-top: 2px;
}
.mhead img {
display: inline-block;
float: right;
background-color: #16120c;
width: 24px;
height: 24px;
}
.mhead img:hover {
    background-color: #906931;
}
.vmtable{
left: 0px;
top: 30px;
position: absolute;
display: block;
border: 1px solid grey;
background-color: #000000; color: #999999;
text-align: center;
}
.mtable {
position: relative;
width: 220px;
height: 190px;
margin: 10px auto;
border-collapse: collapse;
border: 0px;
}
.mtable tr {
border: 0px solid grey;
}
.mtable td {
border: 0px solid grey;
padding: 30px 10px;
}

.onoffswitch {
position: relative; width: 90px;
-webkit-user-select:none; -moz-user-select:none; -ms-user-select: none;
}
.onoffswitch-checkbox {
display: none;
}
.onoffswitch-label {
display: block; overflow: hidden; cursor: pointer;
border: 2px solid #999999; border-radius: 20px;
}
.onoffswitch-inner {
display: block; width: 200%; margin-left: -100%;
transition: margin 0.3s ease-in 0s;
}
.onoffswitch-inner:before, .onoffswitch-inner:after {
display: block; float: left; width: 50%; height: 30px; padding: 0; line-height: 30px;
font-size: 14px; color: white; font-family: Trebuchet, Arial, sans-serif; font-weight: bold;
box-sizing: border-box;
}
.onoffswitch-inner:before {
content: "ON";
padding-left: 10px;
background-color: #E3DC19; color: #FFFFFF;
}
.onoffswitch-inner:after {
content: "OFF";
padding-right: 10px;
background-color: #000000; color: #999999;
text-align: right;
}
.onoffswitch-switch {
display: block; width: 18px; margin: 6px;
background: #12233D;
position: absolute; top: 0; bottom: 0;
right: 56px;
border: 2px solid #999999; border-radius: 20px;
transition: all 0.3s ease-in 0s;
}
.onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-inner {
margin-left: 0;
}
.onoffswitch-checkbox:checked + .onoffswitch-label .onoffswitch-switch {
right: 0px;
}

.vmprop{
top: 30px;
left: 0px;
width: 220px;
position: absolute;
display: none;
border: 1px solid grey;
background-color: #000000; color: #999999;
text-align: center;
}
.iprop{
display: block;    
width: 90%;    
text-align: right;    
}
.iwdth {
 width: 100px;   
}
.ilab {
 width: 50px;
 text-align: right;
}
</style>
<script type="text/javascript">
var DO=[0,0,0,0];
xmlHttp = new XMLHttpRequest();

function viewStatus(){
for(var i=0;i<DO.length;i++){
 var ch=(DO[i]!=0)?"checked":"";
 var id="DO"+i;
 document.getElementById(id).checked = ch;}
}

function setDO(sDO)
{
var theUrl = "setdo?do="+sDO;
xmlHttp.open("GET", theUrl, true);

xmlHttp.onreadystatechange = function(){
 if (xmlHttp.readyState == 4 && xmlHttp.status == 200)
  DO=JSON.parse(xmlHttp.responseText).do;
  viewStatus();
}

xmlHttp.send(null);
}

function goProp(vProp){
if (vProp){
  document.getElementById("vmprop").style.display="block";    
  document.getElementById("vmtable").style.display="none";
  sendParam(false);            
}
else
{
  document.getElementById("vmprop").style.display="none";    
  document.getElementById("vmtable").style.display="block";            
}    
}

function setRW(rw){
if(rw){
 document.getElementById("ipaddr").readOnly=false;
 document.getElementById("ipmask").readOnly=false;
 document.getElementById("ipgetw").readOnly=false;
}
else
{
 var el = document.getElementById("ipaddr");
 el.value = "192.168.4.1"; el.readOnly=true;
 el = document.getElementById("ipmask");
 el.value = "255.255.255.0"; el.readOnly=true;
 el = document.getElementById("ipgetw");
 el.value = "192.168.4.1"; el.readOnly=true;
}
}

function sendParam(snd){
var theUrl = "setprop";  
if(snd){  
var apmode=(document.getElementById("apmod1").checked)?"apsta":"softap";
theUrl +="?apmod="+apmode;
theUrl +="&ssid="+document.getElementById("ssid").value;
theUrl +="&pass="+document.getElementById("pass").value;
theUrl +="&ipadr="+document.getElementById("ipaddr").value;
theUrl +="&ipmsk="+document.getElementById("ipmask").value;
theUrl +="&ipgtw="+document.getElementById("ipgetw").value;
}

xmlHttp.open("GET", theUrl, true);

xmlHttp.onreadystatechange = function(){
 if(xmlHttp.readyState == 4 && xmlHttp.status == 200){
  var prop=JSON.parse(xmlHttp.responseText);
  if(prop.apmod=="apsta") {
    document.getElementById("apmod1").checked=true
    setRW(true);
  }
  else {
    document.getElementById("apmod2").checked=true;
    setRW(false);
  }
  document.getElementById("ssid").value = prop.ssid;
  document.getElementById("pass").value = prop.pass;
  document.getElementById("ipaddr").value = prop.ipadr;
  document.getElementById("ipmask").value = prop.ipmsk;
  document.getElementById("ipgetw").value = prop.ipgtw;
 }
}

xmlHttp.send(null);
}

function setParam(){
 sendParam(true);
 goProp(false);
}

setTimeout(function tmrun(){
 setDO("00");
 setTimeout(tmrun, 1000);
}, 1000);
</script>
</head>
<body class="mbody" onload="setDO('00');">
    <div class="mhead" ><span>esp8266</span><img src="propsvg" onclick="goProp(true);"></div>    
<div id="vmtable" class="vmtable">
<table class="mtable">
<tr><td>

<div class="onoffswitch">
<input type="checkbox" id="DO0" name="DO0" class="onoffswitch-checkbox" onclick="setDO(0);">
<label class="onoffswitch-label" for="DO0">
<span class="onoffswitch-inner"></span>
<span class="onoffswitch-switch"></span>
</label>
</div>

</td><td>

<div class="onoffswitch">
<input type="checkbox" id="DO1" name="DO1" class="onoffswitch-checkbox" onclick="setDO(1);">
<label class="onoffswitch-label" for="DO1">
<span class="onoffswitch-inner"></span>
<span class="onoffswitch-switch"></span>
</label>
</div>

</td></tr>
<tr><td>

<div class="onoffswitch">
<input type="checkbox" id="DO2" name="DO2" class="onoffswitch-checkbox" onclick="setDO(2);">
<label class="onoffswitch-label" for="DO2">
<span class="onoffswitch-inner"></span>
<span class="onoffswitch-switch"></span>
</label>
</div>

</td><td>

<div class="onoffswitch">
<input type="checkbox" id="DO3" name="DO3" class="onoffswitch-checkbox" onclick="setDO(3);">
<label class="onoffswitch-label" for="DO3">
<span class="onoffswitch-inner"></span>
<span class="onoffswitch-switch"></span>
</label>
</div>

</td></tr>
</table>
</div>

<div  id="vmprop" class="vmprop">
<form action="setprop" method="GET">
<br>
<div class="iprop">
<label for="apmode">APMode: </label><br>    
<input id="apmod1" name="apmode" value="apsta" type="radio" onclick="setRW(true);">Client
<input id="apmod2" name="apmode" value="softap" type="radio" onclick="setRW(false);">Point   
<br><br>
<label class="ilab" for="ssid">SSID: </label><input id="ssid" name="ssid" type="text" class="iwdth" value=""><br>
<label class="ilab" for="pass">Pass: </label><input id="pass" name="pass" type="text" class="iwdth" value=""><br>
<br>
<label class="ilab" for="ipaddr">IP: </label><input id="ipaddr" name="ipaddr" type="text" class="iwdth" value=""><br>
<label class="ilab" for="ipmask">Mask: </label><input id="ipmask" name="ipmask" type="text" class="iwdth" value=""><br>
<label class="ilab" for="ipgetw">Gateway: </label><input id="ipgetw" name="ipgetw" type="text" class="iwdth" value=""><br>
</div>
<br>
<input type="button" value="OK" onclick="setParam();">&nbsp;&nbsp;<input type="button" value="Cancel" onclick="goProp(false);">
<br>&nbsp;
</form>
</div>

</body>
</html>
)rawliteral";
