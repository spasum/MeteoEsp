#ifndef WEBCOMMON_H
#define WEBCOMMON_H

const char headStart[] PROGMEM = "<html lang='en'><head><title>Meteo ESP8266</title><meta charset='utf-8'/><meta name='viewport' content='width=device-width, initial-scale=1'/>";

const char mainMenu[] PROGMEM = "<div class='header'><ul>\
<li><a href='/'>Home</a></li>\
<li><a href='/setup'>Setup</a></li>\
<li><a href='/time'>Setup Time</a></li>\
<li><a class='reboot' href='/reboot?reboot_delay=10'>Reboot</a></li>\
</ul></div>";

const char styles[] PROGMEM =
"<style>\
body {font-family: tahoma; font-size: 14px;}\
.header {height: 30px; width: 100%; border-bottom: 1px solid #ddd;}\
.header ul {list-style: none; padding: 0;}\
.header ul li {float: left;}\
.header ul li a {padding: 10px; font-weight: bold; font-size: 16px; text-decoration: none; color: black;}\
.header ul li a:hover {color: #007acc;}\
.reboot {color: red !important;}\
.container {border: 1px solid #ddd; width: 400px;}\
.row {height: 40px; border: 1px solid #ddd; margin: 10px;}\
.label {display: inline-block; width: 150px; padding: 10px; background-color: #eee; height: 20px; border-right: 1px solid #ddd;}\
.value {display: inline-block; padding-left: 10px;}\
.value input {height: 30px; width: 190px; border: 0 none; font-family: tahoma; font-size: 14px;}\
.footer {height: 40px; background-color: #eee; padding: 10px; text-align: right; border-top: 1px solid #ddd;}\
.footer input {border: 1px solid #ddd; background-color: white; height: 40px; border-radius: 10px; font-weight: bold; font-family: tahoma; font-size: 16px;}\
</style>";

const char rebootScripts[] PROGMEM =
"<script type='text/javascript'>\
function \
getUrlVars()\
{\
var vars = [], hash;\
var hashes = window.location.href.slice(window.location.href.indexOf('?') + 1).split('&');\
for(var i = 0; i < hashes.length; i++)\
{\
hash = hashes[i].split('=');\
vars.push(hash[0]);\
vars[hash[0]] = hash[1];\
}\
return vars;\
}\
window.onload = function() {\
var params = getUrlVars();\
var delay = parseInt(params['reboot_delay'], 10);\
console.log(delay);\
var txt = document.getElementById('info');\
var counter = delay;\
var id = setInterval(function () {\
txt.innerHTML = 'Module Reboot in ' + counter + ' sec(s)';\
counter--;\
console.log(counter);\
if (counter < 0) {\
clearInterval(id);\
txt.innerHTML = 'Module rebooted.';\
console.log('Reboot!');\
}\
}, 1000);\
}\
</script>";

const char scripts[] PROGMEM =
"<script type='text/javascript'>\
function \
setNowDateTime() {\
var today = new Date();\
var dd = today.getDate();\
var mm = today.getMonth() + 1;\
var yyyy = today.getFullYear();\
var h = today.getHours();\
var m = today.getMinutes();\
var s = today.getSeconds();\
document.getElementById('day').value = dd;\
document.getElementById('month').value = mm;\
document.getElementById('year').value = yyyy;\
document.getElementById('hour').value = h;\
document.getElementById('minute').value = m;\
document.getElementById('second').value = s;\
}\
function \
saveFormData(pageToRecall) {\
var data = [];\
var inputs = document.getElementsByTagName('input');\
for (var i = 0; i < inputs.length; i++) {\
var input = inputs[i];\
if (input.type == 'text' || input.type == 'password') {\
data.push({key: input.id, value: input.value});\
}\
}\
console.log(data);\
var url = pageToRecall + '?';\
for (var j = 0; j < data.length; j++) {\
var param = data[j];\
url += param.key + '=' + param.value + '&';\
}\
document.location = url;\
}\
</script>";

const char headEnd[] PROGMEM = "</head>";
const char bodyStart[] PROGMEM = "<body>";
const char bodyEnd[] PROGMEM = "</body></html>";

#endif
