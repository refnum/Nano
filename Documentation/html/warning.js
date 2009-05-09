// Emit a warning for MSIE
document.write('<div id="warning">');
document.write('This page requires a modern browser, such as <a href="http://www.getfirefox.com">FireFox</a>');
document.write('</div>');

if (navigator.userAgent.toLowerCase().indexOf("msie") != -1)
	document.getElementById("warning").style.display = 'block';

