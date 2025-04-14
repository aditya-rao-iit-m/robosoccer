#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi credentials
const char* ssid     = "robosoccer";
const char* password = "iitmadras";

ESP8266WebServer server(80);

// Pin Definitions
const int LED_PIN = 2; // Built-in LED

// Left Motor Driver
const int motorAForward  = 5;   // D1 (FL Forward)
const int motorABackward = 4;   // D2 (FL Backward)
const int motorCForward  = 13;  // D7 (RL Forward)
const int motorCBackward = 15;  // D8 (RL Backward)

// Right Motor Driver
const int motorBForward  = 16;  // D0 (FR Forward)
const int motorBBackward = 0;   // D3 (FR Backward)
const int motorDForward  = 14;  // D5 (RR Forward)
const int motorDBackward = 12;  // D6 (RR Backward)

String state = "READY";
bool ledState = HIGH; // Initially ON (built-in LED is active LOW)

void move_forward() {
  digitalWrite(motorAForward, HIGH); digitalWrite(motorABackward, LOW);
  digitalWrite(motorBForward, HIGH); digitalWrite(motorBBackward, LOW);
  digitalWrite(motorCForward, HIGH); digitalWrite(motorCBackward, LOW);
  digitalWrite(motorDForward, HIGH); digitalWrite(motorDBackward, LOW);
}

void move_backward() {
  digitalWrite(motorAForward, LOW); digitalWrite(motorABackward, HIGH);
  digitalWrite(motorBForward, LOW); digitalWrite(motorBBackward, HIGH);
  digitalWrite(motorCForward, LOW); digitalWrite(motorCBackward, HIGH);
  digitalWrite(motorDForward, LOW); digitalWrite(motorDBackward, HIGH);
}

void move_stop() {
  digitalWrite(motorAForward, LOW); digitalWrite(motorABackward, LOW);
  digitalWrite(motorBForward, LOW); digitalWrite(motorBBackward, LOW);
  digitalWrite(motorCForward, LOW); digitalWrite(motorCBackward, LOW);
  digitalWrite(motorDForward, LOW); digitalWrite(motorDBackward, LOW);
}

void move_left() {
  digitalWrite(motorAForward, HIGH); digitalWrite(motorABackward, LOW);
  digitalWrite(motorCForward, LOW);  digitalWrite(motorCBackward, HIGH);
  digitalWrite(motorBForward, LOW);  digitalWrite(motorBBackward, HIGH);
  digitalWrite(motorDForward, HIGH); digitalWrite(motorDBackward, LOW);
}

void move_right() {
  digitalWrite(motorAForward, LOW); digitalWrite(motorABackward, HIGH);
  digitalWrite(motorCForward, HIGH); digitalWrite(motorCBackward, LOW);
  digitalWrite(motorBForward, HIGH); digitalWrite(motorBBackward, LOW);
  digitalWrite(motorDForward, LOW); digitalWrite(motorDBackward, HIGH);
}

void spin_left() {
  // Spin clockwise - CONTINUOUS
  digitalWrite(motorAForward, LOW); digitalWrite(motorABackward, HIGH);
  digitalWrite(motorBForward, HIGH); digitalWrite(motorBBackward, LOW);
  digitalWrite(motorCForward, LOW); digitalWrite(motorCBackward, HIGH);
  digitalWrite(motorDForward, HIGH); digitalWrite(motorDBackward, LOW);
  // Removed delay and stop
}

void spin_right() {
  // Spin counter-clockwise - CONTINUOUS
  digitalWrite(motorAForward, HIGH); digitalWrite(motorABackward, LOW);
  digitalWrite(motorBForward, LOW);  digitalWrite(motorBBackward, HIGH);
  digitalWrite(motorCForward, HIGH); digitalWrite(motorCBackward, LOW);
  digitalWrite(motorDForward, LOW);  digitalWrite(motorDBackward, HIGH);
  // Removed delay and stop
}

void diagonal_left_forward() {
  digitalWrite(motorAForward, HIGH); digitalWrite(motorABackward, LOW);
  digitalWrite(motorDForward, HIGH); digitalWrite(motorDBackward, LOW);
  digitalWrite(motorBForward, LOW);  digitalWrite(motorBBackward, LOW);
  digitalWrite(motorCForward, LOW);  digitalWrite(motorCBackward, LOW);
}

void diagonal_right_forward() {
  digitalWrite(motorBForward, HIGH); digitalWrite(motorBBackward, LOW);
  digitalWrite(motorCForward, HIGH); digitalWrite(motorCBackward, LOW);
  digitalWrite(motorAForward, LOW);  digitalWrite(motorABackward, LOW);
  digitalWrite(motorDForward, LOW);  digitalWrite(motorDBackward, LOW);
}

void diagonal_left_backward() {
  // FIXED: This is now correctly left backward
  digitalWrite(motorBForward, LOW); digitalWrite(motorBBackward, HIGH);
  digitalWrite(motorCForward, LOW); digitalWrite(motorCBackward, HIGH);
  digitalWrite(motorAForward, LOW); digitalWrite(motorABackward, LOW);
  digitalWrite(motorDForward, LOW); digitalWrite(motorDBackward, LOW);
}

void diagonal_right_backward() {
  // FIXED: This is now correctly right backward
  digitalWrite(motorAForward, LOW); digitalWrite(motorABackward, HIGH);
  digitalWrite(motorDForward, LOW); digitalWrite(motorDBackward, HIGH);
  digitalWrite(motorBForward, LOW); digitalWrite(motorBBackward, LOW);
  digitalWrite(motorCForward, LOW); digitalWrite(motorCBackward, LOW);
}

void handleLED() {
  ledState = !ledState;
  digitalWrite(LED_PIN, !ledState); // Invert because built-in LED is active LOW
  String ledStatus = ledState ? "ON" : "OFF";
  state = "LED " + ledStatus;
  server.send(200, "text/plain", state);
}

void handleMove(String direction) {
  if (direction == "forward") {
    move_forward();
    state = "MOVING FORWARD";
  } else if (direction == "backward") {
    move_backward();
    state = "MOVING BACKWARD";
  } else if (direction == "left") {
    move_left();
    state = "MOVING LEFT";
  } else if (direction == "right") {
    move_right();
    state = "MOVING RIGHT";
  } else if (direction == "stop") {
    move_stop();
    state = "STOPPED";
  } else if (direction == "spinleft") {
    spin_left();
    state = "SPINNING CLOCKWISE";
  } else if (direction == "spinright") {
    spin_right();
    state = "SPINNING COUNTER-CLOCKWISE";
  } else if (direction == "fld") {
    diagonal_left_forward();
    state = "DIAGONAL FORWARD-LEFT";
  } else if (direction == "frd") {
    diagonal_right_forward();
    state = "DIAGONAL FORWARD-RIGHT";
  } else if (direction == "bld") {
    diagonal_left_backward();
    state = "DIAGONAL BACKWARD-LEFT";
  } else if (direction == "brd") {
    diagonal_right_backward();
    state = "DIAGONAL BACKWARD-RIGHT";
  }
  server.send(200, "text/plain", state);
}

String webpage() {
  String html = "<!DOCTYPE html><html lang='en'><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>";
  html += "<title>Robo Controller</title>";
  html += "<style>";
  html += "* { box-sizing: border-box; margin: 0; padding: 0; -webkit-tap-highlight-color: transparent; }";
  html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background: #1a1a2e; color: #fff; min-height: 100vh; padding: 10px; touch-action: manipulation; }";
  html += ".container { max-width: 100%; margin: 0 auto; }";
  html += "header { text-align: center; padding: 10px 0; }";
  html += "h1 { font-size: 1.8rem; margin-bottom: 5px; text-shadow: 0 2px 4px rgba(0,0,0,0.3); }";
  html += "h3 { color: #7fffd4; margin-bottom: 10px; font-weight: 300; }";
  html += ".status { background: rgba(0,0,0,0.2); border-radius: 15px; padding: 10px; margin: 10px 0; text-align: center; }";
  html += ".status-text { font-size: 1.2rem; font-weight: bold; color: #7fffd4; }";
  
  // Main control grid
  html += ".control-grid { display: grid; grid-template-columns: repeat(3, 1fr); grid-gap: 10px; margin-top: 15px; }";
  html += ".btn { border: none; border-radius: 12px; padding: 15px 5px; font-size: 1rem; font-weight: bold; cursor: pointer; transition: all 0.2s; letter-spacing: 1px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); user-select: none; }";
  html += ".btn-primary { background: #3498db; color: white; }";
  html += ".btn-secondary { background: #9b59b6; color: white; }";
  html += ".btn-danger { background: #e74c3c; color: white; }";
  html += ".btn-warning { background: #e67e22; color: white; }";
  html += ".btn-success { background: #2ecc71; color: white; }";
  html += ".btn-light { background: #95a5a6; color: white; }";
  html += ".btn:active { transform: scale(0.95); box-shadow: 0 2px 3px rgba(0,0,0,0.2); }";
  html += ".btn-stop { grid-column: span 3; }";
  html += ".empty { visibility: hidden; }";
  html += "footer { margin-top: 20px; text-align: center; font-size: 0.8rem; color: rgba(255,255,255,0.6); }";
  html += "@media (orientation: landscape) {";
  html += "  .control-grid { grid-template-columns: repeat(5, 1fr); }";
  html += "  .btn-stop { grid-column: span 5; }";
  html += "}";
  html += "</style>";
  html += "</head><body>";
  html += "<div class='container'>";
  
  html += "<header>";
  html += "<h1>RoboSoccer Controller</h1>";
  html += "<h3>Naman's Robot Car</h3>";
  html += "</header>";
  
  html += "<div class='status'>";
  html += "<p>Status: <span id='status-text' class='status-text'>" + state + "</span></p>";
  html += "</div>";
  
  html += "<div class='control-grid'>";
  // Row 1
  html += "<button id='fld' class='btn btn-secondary'>↖️</button>";
  html += "<button id='forward' class='btn btn-primary'>⬆️</button>";
  html += "<button id='frd' class='btn btn-secondary'>↗️</button>";
  
  // Row 2
  html += "<button id='left' class='btn btn-primary'>⬅️</button>";
  html += "<button id='stop' class='btn btn-danger'>⏹️</button>";
  html += "<button id='right' class='btn btn-primary'>➡️</button>";
  
  // Row 3
  html += "<button id='bld' class='btn btn-secondary'>↙️</button>";
  html += "<button id='backward' class='btn btn-primary'>⬇️</button>";
  html += "<button id='brd' class='btn btn-secondary'>↘️</button>";
  
  // Row 4
  html += "<button id='spinleft' class='btn btn-warning'>↪️</button>";
  html += "<button id='toggleLED' class='btn btn-success'>💡</button>";
  html += "<button id='spinright' class='btn btn-warning'>↩️</button>";
  html += "</div>";
  
  html += "<footer>";
  html += "<p>Robo Controller v2.0 | Made with ❤️ by Naman Goel</p>";
  html += "</footer>";
  
  html += "</div>";
  
  // JavaScript for AJAX requests
  html += "<script>";
  html += "document.addEventListener('DOMContentLoaded', function() {";
  html += "  const statusText = document.getElementById('status-text');";
  html += "  const buttons = ['forward', 'backward', 'left', 'right', 'stop', 'spinleft', 'spinright', 'fld', 'frd', 'bld', 'brd'];";
  
  // Function to send AJAX requests
  html += "  function sendCommand(url) {";
  html += "    fetch(url)";
  html += "      .then(response => response.text())";
  html += "      .then(data => { statusText.textContent = data; })";
  html += "      .catch(error => { console.error('Error:', error); });";
  html += "  }";
  
  // Add event listeners for movement buttons
  html += "  buttons.forEach(button => {";
  html += "    document.getElementById(button).addEventListener('touchstart', function(e) {";
  html += "      e.preventDefault();";
  html += "      sendCommand('/' + button);";
  html += "    });";
  html += "    document.getElementById(button).addEventListener('click', function(e) {";
  html += "      e.preventDefault();";
  html += "      sendCommand('/' + button);";
  html += "    });";
  html += "  });";
  
  // Toggle LED button
  html += "  document.getElementById('toggleLED').addEventListener('click', function(e) {";
  html += "    e.preventDefault();";
  html += "    sendCommand('/toggleLED');";
  html += "  });";
  
  // Prevent scrolling when touching control buttons
  html += "  document.querySelectorAll('.btn').forEach(btn => {";
  html += "    btn.addEventListener('touchstart', e => e.preventDefault());";
  html += "  });";
  
  html += "});";
  html += "</script>";
  
  html += "</body></html>";
  return html;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT); 
  digitalWrite(LED_PIN, !ledState); // Initialize LED (active LOW)
  
  pinMode(motorAForward, OUTPUT); pinMode(motorABackward, OUTPUT);
  pinMode(motorBForward, OUTPUT); pinMode(motorBBackward, OUTPUT);
  pinMode(motorCForward, OUTPUT); pinMode(motorCBackward, OUTPUT);
  pinMode(motorDForward, OUTPUT); pinMode(motorDBackward, OUTPUT);

  move_stop();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected. IP: " + WiFi.localIP().toString());

  // Main page and static content
  server.on("/", HTTP_GET, []() { 
    server.send(200, "text/html", webpage()); 
  });
  
  // LED toggle endpoint
  server.on("/toggleLED", HTTP_GET, handleLED);
  
  // Movement endpoints
  server.on("/forward", HTTP_GET, []() { handleMove("forward"); });
  server.on("/backward", HTTP_GET, []() { handleMove("backward"); });
  server.on("/left", HTTP_GET, []() { handleMove("left"); });
  server.on("/right", HTTP_GET, []() { handleMove("right"); });
  server.on("/stop", HTTP_GET, []() { handleMove("stop"); });
  server.on("/spinleft", HTTP_GET, []() { handleMove("spinleft"); });
  server.on("/spinright", HTTP_GET, []() { handleMove("spinright"); });
  server.on("/fld", HTTP_GET, []() { handleMove("fld"); });
  server.on("/frd", HTTP_GET, []() { handleMove("frd"); });
  server.on("/bld", HTTP_GET, []() { handleMove("bld"); });
  server.on("/brd", HTTP_GET, []() { handleMove("brd"); });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

//ROBO-SOCCER ESP8266 Base Code for Mecanum Wheeled Robots.

//Released under the GNU GPL v3.0 May 2024

//This is the base code released, as part of the IITM-DIY SchoolConnect Program at IIT Madras. 
//This is meant to be a baseline for creating an ESP8266 based WiFi Mecanum Wheeled robot that can be coded using a webserver. 
//Participants are free to use this code for non-commercial purposes only and are encouraged to modify it and create macros, multiple moves coded into a single button and so on. 
//Please remember the GNU GPL v3.0 license requires attribution and also requires you to share derivative works after the course/competition is over.

//We wish to thank Dr.Janakiraman Viraraghavan our chief mentor at IIT Madras, for his support and enthusiasm without which this robotics project will not have been successful.

//A big thanks to Naman Goel of Bal Bharati Public School, Ganga Ram Hospital Marg, Delhi for porting the base webserver based code during his hands-on activity in the IITM-DIY SchoolConnect Program.

//Concept and Program by: Aditya Rao 23f3000019@es.study.iitm.ac.in BS in Electronic Systems IIT Madras Friday, 17th May 2024
//ESP8266 port by Naman Goel, Bal Bharati Public School, Ganga Ram Hospital Marg, Delhi, Jan 2025 batch IITM SchoolConnect ES.
