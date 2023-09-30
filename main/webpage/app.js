/**
 * Add gobals here
 */
var seconds 	= null;
var otaTimerVar =  null;
var latestUpdateTime = null;
var wifiConnectInterval = null;
/**
 * Initialize functions here.
 */
$(document).ready(function(){
	getUpdateStatus();
    startDHT11SensorInterval();
    $("#connect_wifi").on("click",function(){
        checkCredentials();
    })
});   

/**
 * Gets file name and size for display on the web page.
 */        
function getFileInfo() 
{
    var x = document.getElementById("selected_file");
    var file = x.files[0];

    document.getElementById("file_info").innerHTML = "<h4>File: " + file.name + "<br>" + "Size: " + file.size + " bytes</h4>";
}

/**
 * Handles the firmware update.
 */
function updateFirmware() 
{
    // Form Data
    var formData = new FormData();
    var fileSelect = document.getElementById("selected_file");
    
    if (fileSelect.files && fileSelect.files.length == 1) 
	{
        var file = fileSelect.files[0];
        formData.set("file", file, file.name);
        document.getElementById("ota_update_status").innerHTML = "Uploading " + file.name + ", Firmware Update in Progress...";

        // Http Request
        var request = new XMLHttpRequest();

        request.upload.addEventListener("progress", updateProgress);
        request.open('POST', "/OTAupdate");
        request.responseType = "blob";
        request.send(formData);
    } 
	else 
	{
        window.alert('Select A File First')
    }
}

/**
 * Progress on transfers from the server to the client (downloads).
 */
function updateProgress(oEvent) 
{
    if (oEvent.lengthComputable) 
	{
        getUpdateStatus();
    } 
	else 
	{
        window.alert('total size is unknown')
    }
}

/**
 * Posts the firmware udpate status.
 */
function getUpdateStatus() 
{
    var xhr = new XMLHttpRequest();
    var requestURL = "/OTAstatus";
    xhr.open('POST', requestURL, false);
    xhr.send('ota_update_status');

    if (xhr.readyState == 4 && xhr.status == 200) 
	{		
        var response = JSON.parse(xhr.responseText);
        document.getElementById("latest_firmware").innerHTML = response.compile_date + " - " + response.compile_time
		// If flashing was complete it will return a 1, else -1
		// A return of 0 is just for information on the Latest Firmware request
        if (response.ota_update_status == 1) 
		{
    		// Set the countdown timer time
            seconds = 10;
            // Start the countdown timer
            otaRebootTimer();
        } 
        else if (response.ota_update_status == -1)
		{
            document.getElementById("ota_update_status").innerHTML = "!!! Upload Error !!!";
        }
    }
}

/**
 * Displays the reboot countdown.
 */
function otaRebootTimer() 
{	
    document.getElementById("ota_update_status").innerHTML = "OTA Firmware Update Complete. This page will close shortly, Rebooting in: " + seconds;

    if (--seconds == 0) 
	{
        clearTimeout(otaTimerVar);
        window.location.reload();
    } 
	else 
	{
        otaTimerVar = setTimeout(otaRebootTimer, 1000);
    }
}

/**
 * Gets DHT11 sensor temperature and humidity and status for displaying
 */
function getDHTSensorValues()
{
    $.getJSON('/dhtSensor.json',function(data){
        $("#Sensor_status").text(data["status"]);
        $("#temperature_Readings").text(data["temp"]);
        $("#humidity_reading").text(data["humidity"]);
    });
}

/**
 * Sets the interval for getting the updated DHT11 sensor Value.
 */
function startDHT11SensorInterval()
{
    setInterval(getDHTSensorValues, 5000);
}
/**
 * clear connection interval
 */
function stopWifiConnectStatusInterval() 
{
    if(wifiConnectInterval != null)
    {
        clearInterval(wifiConnectInterval);
        wifiConnectInterval = null;
    }
}
/**
 * gets wifi connection status
 */
function getWifiConnectStatus()
{
    var xhr = new XMLHttpRequest();
    var requestURL = "/wifiConnectStatus";
    xhr.open('POST',requestURL,false);
    xhr.send('wifi_connect_status');

    if(xhr.readyState == 4 && xhr.status == 200)
    {
        var response = JSON.parse(xhr.responseText);
        document.getElementById("wifi_connect_status").innerHTML = "Connecting...";
        if(response.wifi_connect_status == 2)
        {
            document.getElementById("wifi_connect_status").innerHTML = "<h4 class='rd'>Failed to Connect. Check you credentials</h4>";
            stopWifiConnectStatusInterval();
        }
        else if(response.wifi_connect_status == 2)
        {
            document.getElementById("wifi_connect_status").innerHTML = "<h4 class='gr'>Connected Successfully</h4>";
            stopWifiConnectStatusInterval();
        }
    }
}
/**
 * start the interval for checking the connection status
 */
function startWifiConnectStatusInterval()
{
    wifiConnectInterval = setInterval(getWifiConnectStatus,2800);
}
/**
 * connect wifi function called using the ssid and password entered in the text field
 */
function connectWifi()
{
    //Get the SSID
    selectedSSID = $("#Connect_ssid").val();
    pwd = $("#Connect_pass").val();
    $.ajax({
        url: '/wifiConnect.json',
        dataType: 'json',
        method:'POST',
        cache: false,
        headers: {'my-connect-ssid':selectedSSID,'my-connect-pwd':pwd},
        data: {'timestamp':Date.now()}
    });
    startWifiConnectStatusInterval();
}
/**
 * checks credential on connect_wifi button click
 */
function checkCredentials()
{
    errorList = "";
    credsOk = true;
    selectedSSID = $("#connect_ssid").val();
    pwd = $("#connect_pass").val();
    if(selectedSSID == "")
    {
        errorList += "<h4 class='rd'>SSID cannot be empty!</h4>";
        credsOk = false;
    }
    if(pwd == "")
    {
        errorList += "<h4 class='rd'>Password cannot be empty!</h4>";
        credsOk = false;
    }
    if(credsOk == false)
    {
        $("#wifi_connect_credential_error").html(errorList);
    }
    else
    {
        $("#wifi_connect_credential_error").html("");
        connectWifi();
    }
}

/**
 * Shows the if wifi if the box is checked
 */
function showPassword()
{
    var x = document.getElementById("Connect_pass");
    if(x.type === "password")
    {
        x.type = "text";
    }
    else
    {
        x.type = "password";
    }
}