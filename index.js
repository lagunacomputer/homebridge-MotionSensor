var Service, Characteristic;
const request = require('request');

const DEF_TIMEOUT = 3000;

module.exports = function (homebridge) {
   Service = homebridge.hap.Service;
   Characteristic = homebridge.hap.Characteristic;
   homebridge.registerAccessory("homebridge-MotionSensor", "Motion", HttpMotion);
}


function HttpMotion(log, config) {
   this.log = log;

   // url info
   this.url = config["url"];
   this.http_method = config["http_method"] || "GET";
   this.name = config["name"];
   this.manufacturer = config["manufacturer"] || "@lagunacomputer";
   this.model = config["model"] || "Simple HTTP motion sensor";
   this.serial = config["serial"] || "Non-defined serial";
   this.timeout = DEF_TIMEOUT;
   this.json_response = config["json_response"] || "";
}

HttpMotion.prototype = {

   getState: function (callback) {
      var ops = {
         uri:    this.url,
         method: this.http_method,
         timeout: this.timeout
      };
      this.log('Requesting motion on "' + ops.uri + '", method ' + ops.method + ', timeout ' + ops.timeout);
      request(ops, (error, res, body) => {
         var value = null;
         if (error) {
            this.log('HTTP bad response (' + ops.uri + '): ' + error.message);
         } else if (this.json_response === "") {
            value = body;
            this.log('HTTP successful response: ' + body);
         } else {
            try {
               value = JSON.parse(body)[this.json_response];
               this.log('HTTP successful response: ' + body);
            } catch (parseErr) {
               this.log('Error processing received information: ' + parseErr.message);
               error = parseErr;
            }
         }
         if (!error) {
            // Properly set the return value
            if (value === '1' || value === 1 || value === 'true' || value === 'TRUE') value = true;
            else if (value === '0' || value === 0 || value === 'false' || value === 'FALSE') value = false;

            // Check if return value is valid
            if (value !== true && value !== false) {
               this.log('Received value is not valid. Returning "false"');
               value = false;
            }
         }
        
         callback(error, value);
      });
   },

   getServices: function () {
      this.informationService = new Service.AccessoryInformation();
      this.informationService
      .setCharacteristic(Characteristic.Manufacturer, this.manufacturer)
      .setCharacteristic(Characteristic.Model, this.model)
      .setCharacteristic(Characteristic.SerialNumber, this.serial);

      this.motionService = new Service.MotionSensor(this.name);
      this.motionService
         .getCharacteristic(Characteristic.MotionDetected)
         .on('get', this.getState.bind(this));

      return [this.informationService, this.motionService];
   }
};
