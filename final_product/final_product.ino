#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

#define RELAY 14
const char MAIN_page[] PROGMEM = R"=====(
    <!DOCTYPE html>
    <html>
    <style>
      input[type=text], select {
        width: 100%;
        padding: 12px 20px;
        margin: 8px 0;
        display: inline-block;
        border: 1px solid #ccc;
        border-radius: 4px;
        box-sizing: border-box;
      }
      
      input[type=submit] {
        width: 100%;
        background-color: #4CAF50;
        color: white;
        padding: 14px 20px;
        margin: 8px 0;
        border: none;
        border-radius: 4px;
        cursor: pointer;
      }
      
      div {
        border-radius: 5px;
        background-color: #f2f2f2;
        padding: 20px;
        width: 40%;
        margin: auto;
      }
    </style>
    <body bgcolor="#004680" align="center">
    
    <h3 style="color: white;">Matt Daniel's Server Door Lock</h3>
    
    <div align="center">
        <form action="/action_page">
          <label for="passphrase">Door Passphrase</label>
          <input type="text" id="passp" name="passphrase" placeholder="Enter Passphrase">
            <input type="submit" value="Submit">
        </form>
    </div>
    
    </body>
    </html>
)=====";

const char Unlocked_page[] PROGMEM = R"=====(
  <h1 style="color:green;"> Door Unlocked </h1>
  <h2> Reenter passphrase to unlock the door</h2>
  <br><br>
  <h1> <a href='/'> Go Back </a> </h1>
)=====";

const char block_page[] PROGMEM = R"=====(
  <h1 style="color:red;">Wrong Passphrase. Door locked. </h1>
  <h1> <a href='/'> Go Back </a> </h1>
)=====";

//SSID and Password of your WiFi router
const char* ssid = "asmawi71-2.4GHz@unifi";
const char* password = "papa281275";

ESP8266WebServer server(80); //Server on port 80

void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

//===============================================================
// This routine is executed when you press submit
//===============================================================

void handleForm() {
 String passphrase = server.arg("passphrase"); 
 
 
 pinMode(RELAY, OUTPUT);
 digitalWrite(RELAY, LOW);
 
 Serial.print("\nPassphrase:");
 Serial.println(passphrase);
 

 if(passphrase == "Daniel")
 {

   String s = Unlocked_page;
   server.send(200, "text/html", s); //Send web page
   Serial.print("Door Unlocked\n");
   digitalWrite(RELAY, LOW);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Door Unlocked");
   lcd.setCursor(0,1);
   lcd.print("Relocking in 5s");
   lcd.setCursor(0,1);
   delay(1000);
   lcd.print("Relocking in 4s");
   lcd.setCursor(0,1);
   delay(1000);
   lcd.print("Relocking in 3s");
   lcd.setCursor(0,1);
   delay(1000);
   lcd.print("Relocking in 2s");
   lcd.setCursor(0,1);
   delay(1000);
   lcd.setCursor(0,1);
   lcd.print("Relocking in 1s");
   delay(1000);
   digitalWrite(RELAY, HIGH);

   lcd.clear();
   enterMessage();

   
 }
 else
 {
   String s = block_page;
   server.send(200, "text/html", s); //Send web page
   digitalWrite(RELAY, HIGH);
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("WRONG PASSPHRASE");
   lcd.setCursor(0,1);
   lcd.print("DOOR LOCKED");
   delay(2000);
   enterMessage();
 }
 
 
 
}

void setup() {

 
  //Setup LCD
  lcd.begin(16,2);
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Enter Passphrase");
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
 
  server.on("/", handleRoot);      //Which routine to handle at root location
  server.on("/action_page", handleForm); //form action is handled here

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

void loop() {
   server.handleClient();          //Handle client requests
}

void enterMessage(){
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Enter Passphrase");
   lcd.setCursor(0,1);
   lcd.print(WiFi.localIP());
}
