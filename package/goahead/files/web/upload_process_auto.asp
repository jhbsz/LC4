<html>
<meta http-equiv="Pragma" content="no-cache">
<script language="JavaScript" type="text/javascript">
var file_size=<% checkAutoUploadFirmware(); %>;
if (file_size>0)
{
//	parent.showWebMessage(0, "Firmware downloading . . . ( "+file_size+" / "+parent.zyfw_size+" )");
	parent.showWebMessage(0, "Firmware upgrade is proceeding. Please wait and do NOT power off your device.");
}
if (file_size==parent.zyfw_size)
	parent.parent.location.href='/local/advance/upload_process.html';
if (file_size>parseInt(parent.zyfw_size))
{
	parent.showWebMessage(1, "Not a valid firmware.");
	self.location.href="/kill_wget.asp";
}
</script>
</html>