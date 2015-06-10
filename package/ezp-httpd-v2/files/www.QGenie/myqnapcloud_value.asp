<html>
<head></head>
<body>
<input type="hidden" name="wan0_ipaddr" value="<%nv_get("wan0_ipaddr");%>" />
<input type="hidden" name="license_invalid" value="<%nv_get("license_invalid");%>" />
<input type="hidden" name="get_qcloud_status" value="<%nv_attr_get("myqnapcloud_id_rule","0","status");%>" />
<input type="hidden" name="get_qcloud_qid" value="<%nv_attr_get("myqnapcloud_id_rule","0","qid");%>" />
<input type="hidden" name="get_qcloud_device_name" value="<%nv_attr_get("myqnapcloud_id_rule","0","device_name");%>" />
<input type="hidden" name="qcloud_get_web_mgmt_outport" value="<%nv_attr_get("upnpc_status_rule","0","outport");%>" />
<input type="hidden" name="qcloud_get_file_station_outport" value="<%nv_attr_get("upnpc_status_rule","1","outport");%>" />
<input type="hidden" name="qcloud_test_fetch_result_web_mgmt" value="<%nv_attr_get("upnpc_status_rule","0","fetch_result");%>" />
<input type="hidden" name="qcloud_test_fetch_result_file_station" value="<%nv_attr_get("upnpc_status_rule","1","fetch_result");%>" />
<input type="hidden" name="qcloud_last_update_status" value="<%show_myqnapcloud_last_update_status("last_update_status");%>" />
<input type="hidden" name="qcloud_last_update_ret_code" value="<%show_myqnapcloud_last_update_ret_code();%>" />
</body>
