# homebridge-MotionSensor
HomeBridge HomeKit Plugin for Arduino based HC-SR501 PIR motion sensors


Creates a Motion Sensor Accessory for iOS Home app/Homekit via Homebridge.

sudo npm install -g https://www.npmjs.com/package/homebridge-MotionSensor

Please feel free to fix it, I am a copy/paste type of programmer and my skills are meager and limited.

This is a feeble attempt at a work in progress. Most of the code is stolen from other authors, will update as I go. the basic idea here is to rip off https://github.com/lucacri/homebridge-http-temperature-humidity and/or https://github.com/metbosch/homebridge-http-temperature/blob/master/index.js, but change the Accessory/Service info for MotionDetected instead of CurrentTemperature. This plugin is based on the plugins above, as well as https://github.com/lagunacomputer/homebridge-CurrentAmbientLightLevel.

Once this is complete, basically all sorts of input sensors become available to us .... The list of possible sensor types are here: https://github.com/KhaosT/HAP-NodeJS/blob/master/lib/gen/HomeKitTypes.js I imagine that soon HomeKit will include all kinds of options for automation based on input sensors. ie: "If there is Motion Detected in the Garage, turn on the Living Room Lamp" etc...

*update 11/1/2016 i should be able to convert the CurrentAmbientLightLevel code into Motion Sensor, stay tuned

*update 11/1/2016 ok i got it working!  We now have Motion Sensors in HomeKit!  Use an AppleTV4 or the EVE iOS app to create Automation Rules and Triggers! See http://www.instructables.com/id/HomeKit-HomeBridge-Siri-Enabled-Arduino-ESP8266-No-1/


{

"bridge": {
"name": "HomeBridge",
"username": "CC:22:3D:E3:CE:30",
"port": 51826,
"pin": "031-45-154"
},

"description": "",
    "accessories": [
    {
        "accessory": "Motion",
        "name": "Motion Sensor",
        "url": "http://192.168.1.101/?light",
        "sendimmediately": "",
        "http_method": "GET"
    }
]

"platforms": []
}
