var Service, Characteristic;
const request = require('request');

const DEF_TIMEOUT = 3000, //3s
      DEF_INTERVAL = 60000; //60s

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
   this.update_interval = Number( config["update_interval"] || DEF_INTERVAL );

   // Internal variables
   this.last_state = false;
   this.waiting_response = false;
}

HttpMotion.prototype = {

   updateState: function () {
      //Ensure previous call finished
      if (this.waiting_response) {
         this.log('Avoid updateState as previous response does not arrived yet');
         return;
      }
      this.waiting_response = true;

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
                this.log('Received value is not valid. Keeping last_state: "' + this.last_state + '"');
            } else {
                this.motionService
                   .getCharacteristic(Characteristic.MotionDetected).updateValue(value, null, "updateState");
                this.last_state = value;
            }
         }
         this.waiting_response = false;
      });
   },

   getState: function (callback) {
      var state = this.last_state;
      var update = !this.waiting_response;
      var sync = this.update_interval === 0;
      this.log('Call to getState: last_state is "' + state + '", will update state now "' + update + '"' );
      if (update) {
         setImmediate(this.updateState.bind(this));
      }
      callback(null, state);
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

      if (this.update_interval > 0) {
         this.timer = setInterval(this.updateState.bind(this), this.update_interval);
      }

      return [this.informationService, this.motionService];
   }
};
