

#include <SPI.h>
//#include <Ethernet.h>

byte mac[] = { 0x5C, 0xCF, 0x7F, 0xC1, 0x1A, 0x33 };
//byte mac[] = { 0x18, 0xFE, 0x34, 0xD4, 0x28, 0x1E }; //physical mac address 5c:cf:7f:c1:1a:33
//byte mac[] = { 0x5C, 0xCF, 0x7F, 0x24, 0x84, 0x3D }; //physical mac address 18:fe:34:d4:28:1e
//byte mac[] = { 0x5C, 0xCF, 0x7F, 0x24, 0x66, 0x69 }; //physical mac address
//byte ip[] = { 192, 168, 1, 147 }; // ip in lan
//byte gateway[] = { 192, 168, 1, 1 }; // internet access via router
//byte subnet[] = { 255, 255, 255, 0 }; //subnet mask
//EthernetServer server(80); //server port

String readString; 

//#include <DHT.h>
//#define DHT11Pin D6




//////#include <OneWire.h>
//#include <DallasTemperature.h>

/*-----( Declare Constants )-----*/
//#define ONE_WIRE_BUS D4 /*-(Connect to Pin 2 )-*/

/*-----( Declare objects )-----*/
/* Set up a oneWire instance to communicate with any OneWire device*/
//OneWire ourWire(ONE_WIRE_BUS);

/* Tell Dallas Temperature Library to use oneWire Library */
//DallasTemperature sensors(&ourWire);

/*-----( Declare Variables )-----*/

//////////////////////



int photocellPin = 0;     // the cell and 10K pulldown are connected to a0
int photocellReading;     // the analog reading from the analog resistor divider



boolean pirReading = false; 
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 10;

//the time when the sensor outputs a low impulse
long unsigned int lowIn;

//the amount of milliseconds the sensor has to be low
//before we assume all motion has stopped  was 5000
long unsigned int pause = 3000;

boolean lockLow = true;
boolean takeLowTime;
int pirPin = 13;    //the digital pin connected to the PIR sensor's output arduino13=nodemcu's D7 or D6 cant remember






#include <ESP8266WiFi.h>

IPAddress ip(192, 168, 1, 59); 
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);





const char* ssid = "yourwifiSSIDchangeme";
const char* password = "yourwifipasswordchangeme";

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);

//DHT dht(DHT11Pin, DHT11);

void setup(){
Serial.begin(115200);
  pinMode(5, OUTPUT); //pin selected to control
  //start Ethernet
  //Ethernet.begin(mac, ip, gateway, gateway, subnet);

WiFi.begin(ssid, password);
//WiFi.config(ip, gateway, subnet);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //WiFi.begin(ssid, password);
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi connected");
  
  // Start the server
  server.begin();
  //Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());

  
  //server.begin();
  //enable serial data print 
  //Serial.begin(9600); 

delay(500);
//sensors.begin();


  pinMode(pirPin, INPUT);  ///PIR
    digitalWrite(pirPin, LOW);
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
    Serial.println(WiFi.localIP());
}

void loop(){
//readPIR();
delay(1000);
  Serial.println(pirReading);     // the raw analog reading
 if (digitalRead(pirPin) == HIGH) {
    //digitalWrite(ledPin, HIGH);   //the led visualizes the sensors output pin state
    if (lockLow) {
      //makes sure we wait for a transition to LOW before any further output is made:
      lockLow = false;
      Serial.println("---");
      Serial.print("motion detected at ");
      Serial.print(millis() / 1000);
      Serial.println(" sec");
      delay(50);
    }
    takeLowTime = true;
pirReading = true; 
    //alarmTriggered();
  }


  if (digitalRead(pirPin) == LOW) {
    //digitalWrite(ledPin, LOW);  //the led visualizes the sensors output pin state
pirReading = false; 
    if (takeLowTime) {
      lowIn = millis();          //save the time of the transition from high to LOW
      takeLowTime = false;       //make sure this is only done at the start of a LOW phase
    }
    //if the sensor is low for more than the given pause,
    //we assume that no more motion is going to happen
    if (!lockLow && millis() - lowIn > pause) {
      //makes sure this block of code is only executed again after
      //a new motion sequence has been detected
      lockLow = true;
      Serial.print("motion ended at ");      //output
      Serial.print((millis() - pause) / 1000);
      Serial.println(" sec");
      delay(50);
    }
  }





//delay(1000);
readPhotocell();
//delay(1000);
//int chk = DHT.read11(DHT11_PIN);
//delay(1000);


//float temperatureIndoor;
  //float temperatureOutdoor;
  //float humidity;
  //sensors.requestTemperatures(); // Send the command to get temperatures
 //         temperatureIndoor = (sensors.getTempCByIndex(0));
/*
// wait 2 seconds between readings
  delay(2000);
  // get humidity
  float humidity = dht.readHumidity();
  // get temperature as C
  float celsius = dht.readTemperature();
  // get temperature as F
  float fahrenheit = dht.readTemperature(true);
  // print results
  Serial.print("Humidity: "); Serial.print(humidity);
  Serial.print(" Celsius: "); Serial.print(celsius);
  Serial.print(" Fahrenheit: "); Serial.println(fahrenheit);
*/
 
  delay(100);
  // Create a client connection
  //EthernetClient client = server.available();
  WiFiClient client = server.available();
  
  delay(200);
  
  if (client) {
    delay(200);
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();

        //read char by char HTTP request
        if (readString.length() < 100) {

          //store characters to string 
          readString += c; 
          //Serial.print(c);
        } 

        //if HTTP request has ended
        if (c == '\n') {

          ///////////////
       /*   Serial.println(readString); //print to serial monitor for debuging 

          client.println("HTTP/1.1 200 OK"); //send new page
          client.println("Content-Type: text/html");
          client.println();

          client.println("<HTML>");
          client.println("<HEAD>");
          client.println("<TITLE>Arduino GET test page</TITLE>");
          client.println("</HEAD>");
          client.println("<BODY>");

          client.println("<H1>Simple Arduino button</H1>");
          
          client.println("<a href=\"/?on\">ON</a>"); 
          client.println("<a href=\"/?off\">OFF</a>"); 
          client.println("<a href=\"/?temp\">temp</a>"); 

          client.println("</BODY>");
          client.println("</HTML>");

          delay(1);
*/

/*   /////original code for dht
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Server: Arduino");
          client.println("Connnection: close");
          client.println();
          //sensors.requestTemperatures(); // Send the command to get temperatures
          //temperatureIndoor = (sensors.getTempCByIndex(0));
          //temperatureOutdoor = (sensors.getTempFByIndex(0));
 //Serial.print(sensors.getTempCByIndex(0));

 humidity = (DHT.humidity);
          client.print("{");
          client.print("\"temperature\":");
          client.print(temperatureIndoor);
          client.print(",");
          client.print("\"humidity\":");
          client.print(humidity);
          
          //client.print("}");
          //client.println();
             
          
         client.print(",");
          //client.print("{");
          client.print("\"lightval\":");
          client.print(photocellReading);
          client.print("}");
          client.println();
     
 original code for dht
*/





/*
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Server: Arduino");
          client.println("Connnection: close");
          client.println();
          sensors.requestTemperatures(); // Send the command to get temperatures
          //temperatureIndoor = (sensors.getTempCByIndex(0));
          //temperatureOutdoor = (sensors.getTempFByIndex(0));
 
          client.print("{");
          client.print("\"temperature\":");
          //client.print(temperatureIndoor);
          //client.print(temperature);
          client.print(celsius);
          client.print(",");
          client.print("\"humidity\":");
          client.print(humidity);
          
          //client.print("}");
          //client.println();
       
          client.print(",");
          //client.print("{");
          client.print("\"lightval\":");
          client.print(photocellReading);
          client.print(",");
client.print("\"pirval\":");
          client.print(pirReading);
//client.print("True");

          
          client.print("}");
          client.println();

          //stopping client
          client.stop();
*/
          ///////////////////// control arduino pin
          if(readString.indexOf("?on") >0)//checks for on
          {
            Serial.println(readString.indexOf("on"));
            
            digitalWrite(5, HIGH);    // set pin 5 high
            Serial.println("Led On");

          }
          if(readString.indexOf("?off") >0)//checks for off
          {
            Serial.println(readString.indexOf("off"));
            digitalWrite(5, LOW);    // set pin 5 low
            Serial.println("Led Off");
          }
          if(readString.indexOf("pirval") >0)//checks for on
          {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Server: Arduino");
          client.println("Connnection: close");
          client.println();
          //sensors.requestTemperatures(); // Send the command to get temperatures
          //temperatureIndoor = (sensors.getTempCByIndex(0));
          //temperatureOutdoor = (sensors.getTempFByIndex(0));
          client.print("{");
          //client.print("\"temperature\":");
          //client.print(temperatureIndoor);
          //client.print(temperature);
          //client.print(celsius);
          //client.print(",");
          //client.print("\"humidity\":");
          //client.print(humidity);
          //client.print("}");
          //client.println();
          //client.print(",");
          //client.print("{");
          //client.print("\"lightval\":");
          //client.print(photocellReading);
          //client.print(",");
          client.print("\"pirval\":");
          client.print(pirReading);
          //client.print("True");
          client.print("}");
          client.println();
          client.stop();
//delay(200);

            
            //Serial.println(readString.indexOf("on"));
            Serial.println("PIR =");
          //clearing string for next read
          readString="";
          }   


                 /*
          if(readString.indexOf("?temp") >0)//checks for off
          {
   
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Server: Arduino");
          client.println("Connnection: close");
          client.println();
          sensors.requestTemperatures(); // Send the command to get temperatures
          //temperatureIndoor = (sensors.getTempCByIndex(0));
          //temperatureOutdoor = (sensors.getTempFByIndex(0));
          client.print("{");
          client.print("\"temperature\":");
          //client.print(temperatureIndoor);
          //client.print(temperature);
          client.print(celsius);
          client.print(",");
          client.print("\"humidity\":");
          client.print(humidity);
          //client.print("}");
          //client.println();
          //client.print(",");
          //client.print("{");
          //client.print("\"lightval\":");
          //client.print(photocellReading);
          //client.print(",");
          //client.print("\"pirval\":");
          //client.print(pirReading);
          //client.print("True");
          client.print("}");
          client.println();
          //stopping client
          client.stop();
            

            Serial.println(readString.indexOf("temp"));
            //temperatureIndoor = (sensors.getTempCByIndex(0));
            //digitalWrite(5, LOW);    // set pin 5 low
            Serial.println("temp sent");
          }

          */
          if(readString.indexOf("?light") >0)//checks for off
          {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json;charset=utf-8");
          client.println("Server: Arduino");
          client.println("Connnection: close");
          client.println();
          //sensors.requestTemperatures(); // Send the command to get temperatures
          //temperatureIndoor = (sensors.getTempCByIndex(0));
          //temperatureOutdoor = (sensors.getTempFByIndex(0));
          //client.print("{");
          //client.print("\"temperature\":");
          //client.print(temperatureIndoor);
          //client.print(temperature);
          //client.print(celsius);
          //client.print(",");
          //client.print("\"humidity\":");
          //client.print(humidity);
          //client.print("}");
          //client.println();
          //client.print(",");
          client.print("{");
          client.print("\"lightval\":");
          client.print(photocellReading);
          //client.print(",");
//client.print("\"pirval\":");
          //client.print(pirReading);
//client.print("True");
          client.print("}");
          client.println();
          //stopping client
          client.stop();
          //delay(100);
            Serial.println(readString.indexOf("light"));
            //digitalWrite(5, LOW);    // set pin 5 low
            Serial.println("light sent");
          }

         
          //clearing string for next read
          readString="";


//sensors.requestTemperatures(); // Send the command to get temperatures
//Serial.print("Device 1 (index 0) = ");
//Serial.print(sensors.getTempFByIndex(0));
//Serial.println(" Degrees F");




        }                     ///c=n
      }                //client available
             //stopping client

    }             //client connected
  }         //if client
  
}      //loop


void readPhotocell(){

photocellReading = analogRead(photocellPin);  
 Serial.print(WiFi.localIP());
  Serial.print(" Analog reading = ");
  Serial.print(photocellReading);     // the raw analog reading

  // We'll have a few threshholds, qualitatively determined
  if (photocellReading < 10) {
    Serial.println(" - Dark");
  } else if (photocellReading < 200) {
    Serial.println(" - Dim");
  } else if (photocellReading < 500) {
    Serial.println(" - Light");
  } else if (photocellReading < 800) {
    Serial.println(" - Bright");
  } else {
    Serial.println(" - Very bright");
  }
          
}

void readPIR(){


}


