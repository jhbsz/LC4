<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
    "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<meta http-equiv="Pragma" content="no-cache">
<head><link href="images/css.css" rel="stylesheet" type="text/css" />
<script type="text/JavaScript">
var startLLTDcheck=<% startLLTDcheck(); %>;
var startRoutercheck=<% startRoutercheck(); %>;
var startInternetcheck=<% startInternetcheck(); %>;
if (startLLTDcheck)
setTimeout('self.location.href="easy_info.asp";', 6000);
else
self.location.href="easy_info.asp";
</script>
<body>
</body>
</html>
