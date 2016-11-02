/*
    HTTP webclient code that sends to and/or gets data from devicehub.net. Set up an account
    on their website and add a project with device and sensors. From there if you click on sensors
    you can see all the data that was posted by your NodeMCU! Pretty cool...
    
    IMPORTANT NOTE: For deep sleep to work, jumper D0 (GPIO16) directly to RST pin on NodeMCU
    
    CREDITS: Original code by Leonardo Gonçalves - Instituto Nacional de Telecomunicações INATEL - Brazil
    EDITS: By Timothy Woo
*/
#include <ESP8266WiFi.h>
const char* ssid = "WIFI";
const char* password = "SENHA";

String pubString;

// Escolha aqui o nome do servidor 
const char* host = "oriva.esy.es";//api.devicehub.net

int sensor_pin = A0; // Set  pin you are using to read data. In this case it's analog for reading sensor values

float sensor = 0; // Initialize the value you are going to read
const int sleepTimeS = 1*10; // Set sleep time in seconds for the deep sleep cycle

void setup() {
  Serial.begin(115200);
  delay(100);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // This is your NodeMCU IP address. Could be handy for other projects

  POST(); // Post the value to the web database
  GET();  // If all you need to do is push data, you don't need this

  Serial.println("ESP8266 in sleep mode");
  // Put NodeMCU in deep sleep. When it wakes up it will run setup() again,
  // connect to WiFi, then post and/or get data, then go back to sleep and repeat
  ESP.deepSleep(sleepTimeS * 1000000);
}

void loop()
{
  // This is empty because in order for the sleep cycle to work all code is in setup()
}

void POST(void)
{
  delay(5000);  // You can get rid of this or decrease it
  sensor = analogRead(sensor_pin); // Read sensor value

  Serial.print("connecting to ");
  Serial.println(host);
  Serial.println();

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }

  pubString = "{\"tipo\":\"esp\",\"dado1\":" + String(sensor) + ",\"dado2\":\"2\",\"dado3\":\"3\"}";
  //pubString = "{\"value\": " + String(sensor) + "}";
  String pubStringLength = String(pubString.length(), DEC);
  // We now create a URI for the request
  Serial.print("Requesting POST: ");
  // Send request to the server:
  client.println("POST /api.php/sensor HTTP/1.1");
  // Note: replace the ALL CAPS parts above with info from DeviceHub.net. After setting up an account,
  // add a project, a device (NodeMCU), a sensor, and actuator if you need. After you set
  // up the project it should show you an API key, device UUID, and of course the sensor name you entered.
  client.println("Host: oriva.esy.es");
  //client.print("X-ApiKey: YOUR_API_KEY\r\n"); // Get this from DeviceHub.net
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(pubStringLength);
  client.println();
  client.print(pubString);
  client.println();
  delay(500); // Can be changed
  
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}

// If you only need to post data to the database then you don't need the code below:
void GET(void)
{
  int state_pos;
  String state;
  delay(5000);
  sensor = analogRead(sensor_pin); // Read sensor value
  Serial.print("connecting to ");
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  // We now create a URI for the request
  Serial.print("Requesting GET: ");
  // This will send the request to the server
  client.println("GET /api.php/sensor HTTP/1.1");
  client.println("Host: oriva.esy.es");
  //client.print("X-ApiKey: YOURAPIKEY\r\n");
  client.println("Connection: close");
  client.println();
  delay(500);
  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");
}
