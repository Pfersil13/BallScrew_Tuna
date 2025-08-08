#include "GitOTA.h"




void CheckNewFirmware(){
    ESP32OTAPull ota; //Create ota object

    ota.EnableSerialDebug(); //Enables verbose output, telling you what the OTA is doing under the hood. Handy for finding issues.

    ota.SetCallback(callback_percent); //Setting a callback
    //The callback can be used to make things happen as the update installs.
    //You pass it a function, that expects the parameters int offset and int totallength
    //You can then use this information for progress bars and similar.
    //In our situation, we print the progress in bytes and as a percent.

    int ret = ota.CheckForOTAUpdate("https://github.com/JimSHED/ESP32-OTA-Pull-GitHub/releases/download/example/otadirectory.json", VERSION, ESP32OTAPull::UPDATE_AND_BOOT);

    Serial.print("OTA Update Check Returned the Status Code: "); Serial.println(ret);
    //Serial.print("This means: "); Serial.println(errtext(ret));

    Serial.println("If you made it this far, something went wrong. Check the above message.");


}

  


const char *errtext(int code)
{
	switch(code)
	{
		case ESP32OTAPull::UPDATE_AVAILABLE:
			return "An update is available but wasn't installed";
		case ESP32OTAPull::NO_UPDATE_PROFILE_FOUND:
			return "No profile matches";
		case ESP32OTAPull::NO_UPDATE_AVAILABLE:
			return "Profile matched, but update not applicable";
		case ESP32OTAPull::UPDATE_OK:
			return "An update was done, but no reboot";
		case ESP32OTAPull::HTTP_FAILED:
			return "HTTP GET failure";
		case ESP32OTAPull::WRITE_ERROR:
			return "Write error";
		case ESP32OTAPull::JSON_PROBLEM:
			return "Invalid JSON";
		case ESP32OTAPull::OTA_UPDATE_FAIL:
			return "Update fail (no OTA partition?)";
		default:
			if (code > 0)
				return "Unexpected HTTP response code";
			break;
	}
	return "Unknown error";
}

void callback_percent(int offset, int totallength)
{
	static int prev_percent = -1;
	int percent = 100 * offset / totallength;
	if (percent != prev_percent)
	{
		Serial.printf("Updating %d of %d (%02d%%)...\n", offset, totallength, 100 * offset / totallength);
		prev_percent = percent;
	}
}