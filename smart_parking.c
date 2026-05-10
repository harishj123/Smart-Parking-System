#include <WiFi.h>
#include <WebServer.h>

// ---------------- PINS ----------------
#define TRIG 5
#define ECHO 18
#define IR_PIN 4

#define MAX_DISTANCE 30

// ---------------- WIFI ----------------
const char* ssid = "Wifi_name";
const char* password = "passward";

WebServer server(80);

// ---------------- ULTRASONIC ----------------
float getDistance() {
  long sum = 0;
  int samples = 3;

  for (int i = 0; i < samples; i++) {
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    long duration = pulseIn(ECHO, HIGH, 30000);
    float distance = duration * 0.034 / 2;

    if (distance > 0 && distance < 400) {
      sum += distance;
    }

    delay(10);
  }

  return sum / samples;
}

// ---------------- SCORE CALCULATION ----------------
int getOccupancyScore(float distance, int ir) {

  int irScore = (ir == 0) ? 100 : 0;

  float distScore = 0;

  if (distance > 0 && distance < MAX_DISTANCE) {
    distScore = (1 - (distance / MAX_DISTANCE)) * 100;
  }

  int score = (0.5 * irScore) + (0.5 * distScore);

  if (score > 100) score = 100;
  if (score < 0) score = 0;

  return score;
}

// ---------------- WEB PAGE ----------------
void handleRoot() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Smart Parking</title>

<style>
body {
  margin: 0;
  font-family: Arial;
  background: linear-gradient(to right, #141E30, #243B55);
  color: white;
  text-align: center;
}

.card {
  background: rgba(255,255,255,0.1);
  margin: 15px;
  padding: 20px;
  border-radius: 12px;
}
</style>
</head>

<body>

<h1>🚗 Smart Parking Dashboard</h1>

<div class="card">
  <h2>Status</h2>
  <div id="status">Loading...</div>
</div>

<div class="card">
  <h2>Distance</h2>
  <div id="distance">--</div>
</div>

<div class="card">
  <h2>IR Sensor</h2>
  <div id="ir">--</div>
</div>

<div class="card">
  <h2>Occupancy Score</h2>
  <div id="score">--</div>
</div>

<script>
setInterval(() => {
  fetch('/data')
    .then(res => res.json())
    .then(data => {

      document.getElementById("distance").innerHTML =
        data.distance.toFixed(2) + " cm";

      document.getElementById("ir").innerHTML =
        data.ir == 0 ? "Object Detected" : "Clear";

      document.getElementById("score").innerHTML =
        data.score + " %";

      document.getElementById("status").innerHTML =
        data.status;

    });
}, 1000);
</script>

</body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

// ---------------- API ----------------
void handleData() {

  float distance = getDistance();
  int ir = digitalRead(IR_PIN);

  int score = getOccupancyScore(distance, ir);

  String status;

  if (score > 70) status = "OCCUPIED 🔴";
  else if (score > 40) status = "HALF FILLED 🟠";
  else status = "EMPTY 🟢";

  String json = "{";
  json += "\"distance\":" + String(distance) + ",";
  json += "\"ir\":" + String(ir) + ",";
  json += "\"score\":" + String(score) + ",";
  json += "\"status\":\"" + status + "\"";
  json += "}";

  server.send(200, "application/json", json);
}

// ---------------- SETUP ----------------
void setup() {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(IR_PIN, INPUT);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  server.on("/", handleRoot);
  server.on("/data", handleData);

  server.begin();
}

// ---------------- LOOP ----------------
void loop() {
  server.handleClient();
}
