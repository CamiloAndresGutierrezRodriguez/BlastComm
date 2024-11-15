#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Galaxy";           // Reemplaza con el SSID de tu red Wi-Fi
const char* password = "Digitales24";  // Reemplaza con la contraseña de tu red Wi-Fi

WebServer server(80);
int numberStored = 0;  // Variable para almacenar el número
int img = 0;
int multi = 23;  // Inicialización de multi

int dataPins[] = { 13 /*B*/, 12 /*C*/, 14 /*D*/, 27 /*E*/, 26 /*F*/, 25 /*A*/, 33 /*G*/ };
int controlPins[] = { 5 /*4*/, 4 /*3*/, 2 /*2*/, 15 /*1*/ };
int numbers[] = { 0x3f, 0x03, 0x6d, 0x67, 0x53, 0x76, 0x7e, 0x23, 0x7f, 0x77 };
int buzzer = 32;
int timeOn = 1;
bool upRoad = true;


void handleRoot() {
  String html = "<!DOCTYPE html>";
  html += "<html lang=\"es\">";
  html += "<head>";
  html += "    <meta charset=\"UTF-8\">";
  html += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">";
  html += "    <style>";
  html += "        body {";
  html += "            font-family: Arial, sans-serif;";
  html += "            text-align: center;";
  html += "            margin: 0;";
  html += "            padding: 20px;";
  html += "            background-color: #cde4e1;";
  html += "        }";
  html += "        input[type='number'] {";
  html += "            width: 100%;";
  html += "            max-width: 100px;";
  html += "            padding: 10px;";
  html += "            margin: 5px 0;";
  html += "            border-radius: 10px;";
  html += "            text-align: center;";
  html += "            border: 3px dashed black;";
  html += "            background-color: #f5f7d2;";
  html += "        }";
  html += "        input[type='submit'] {";
  html += "            width: 100%;";
  html += "            max-width: 19%;";
  html += "            padding: 10px;";
  html += "            margin: 5px 0;";
  html += "            border-radius: 10px;";
  html += "            background-color: #4CAF50; /* Verde */";
  html += "            color: white;";
  html += "            border: none;";
  html += "            cursor: pointer;";
  html += "        }";
  html += "        input[type='submit']:hover {";
  html += "            background-color: #45a049;";
  html += "        }";
  html += "        button {";
  html += "            width: 100%;";
  html += "            max-width: 19%;";
  html += "            padding: 3%;";
  html += "            margin: -5px 0;";
  html += "            border-radius: 15px;";
  html += "            color: white;";
  html += "            border: none;";
  html += "            cursor: pointer;";
  html += "        }";
  html += "        .btn-on {";
  html += "            background-color: #4CAF50; /* Verde */";
  html += "        }";
  html += "        .btn-on:hover {";
  html += "            background-color: #45a049;";
  html += "        }";
  html += "        .btn-off {";
  html += "            background-color: #f44336; /* Rojo */";
  html += "        }";
  html += "        .btn-off:hover {";
  html += "            background-color: #d32f2f;";
  html += "        }";
  html += "        h1, h2 {";
  html += "            color: #000000;";
  html += "        }";
  html += "        .hidden {";
  html += "            display: none;";
  html += "        }";
  html += "        #controls {";
  html += "            margin-top: 100px; /* Ajusta el espacio de los controles */";
  html += "        }";
  html += "    </style>";
  html += "    <script>";
  html += "        function updateNumber() {";
  html += "            var formData = new FormData(document.getElementById('numberForm'));";
  html += "            fetch('/submit', { method: 'POST', body: formData })";
  html += "                .then(response => response.text())";
  html += "                .then(data => {";
  html += "                    document.getElementById('currentNumber').innerText = data;";
  html += "                    document.getElementById('controls').classList.add('hidden');";  // Oculta los controles
  html += "                });";
  html += "            return false;";  // Prevent form submission
  html += "        }";
  html += "        function setBuzzer(state) {";
  html += "            fetch('/buzzer?state=' + state);";
  html += "        }";
  html += "        function setMulti(state) {";
  html += "            fetch('/multi?state=' + state);";
  html += "        }";
  html += "        function checkState() {";
  html += "            fetch('/buzzer/state').then(response => response.text()).then(state => {";
  html += "                if (state === '0') {";
  html += "                    document.getElementById('controls').classList.remove('hidden');";  // Muestra los controles
  html += "                }";
  html += "            });";
  html += "            fetch('/multi/state').then(response => response.text()).then(state => {";
  html += "                if (state === '0') {";
  html += "                    document.getElementById('controls').classList.remove('hidden');";  // Muestra los controles
  html += "                }";
  html += "            });";
  html += "        }";
  html += "        setInterval(checkState, 1000);";
  html += "    </script>";
  html += "</head>";
  html += "<body>";
  html += "    <h1>Ingrese Número</h1>";
  html += "    <form id=\"numberForm\" onsubmit=\"return updateNumber();\">";
  html += "        <input type=\"number\" name=\"number\" min=\"0\" max=\"9999\" required>";
  html += "        <input type=\"submit\" value=\"Update\">";
  html += "    </form>";
  html += "    <h2>Temporizador Actual: <span id=\"currentNumber\">0</span></h2>";
  html += "    <div id=\"controls\" class=\"hidden\">";
  html += "        <h2>Estado del Buzzer</h2>";
  html += "        <button class=\"btn-on\" onclick=\"setBuzzer('on')\">ON</button>";
  html += "        <button class=\"btn-off\" onclick=\"setBuzzer('off')\">OFF</button>";
  html += "        <h2>Estado del Multiplicador</h2>";
  html += "        <button class=\"btn-on\" onclick=\"setMulti('on')\">ON</button>";
  html += "        <button class=\"btn-off\" onclick=\"setMulti('off')\">OFF</button>";
  html += "    </div>";
  html += "</body>";
  html += "</html>";

  server.send(200, "text/html", html);
}

void handleSubmit() {
  if (server.hasArg("number")) {
    numberStored = server.arg("number").toInt();                          // Actualiza la variable con el nuevo número
    Serial.println("Temporizador Establecido: " + String(numberStored));  // Imprime el número en la consola
    server.send(200, "text/plain", String(numberStored));                 // Devuelve el número almacenado
  } else {
    server.send(400, "text/plain", "Número no recibido");
  }
}

void handleBuzzer() {
  if (server.hasArg("state")) {
    String state = server.arg("state");
    Serial.println("Buzzer estado: " + state);  // Imprime el estado del buzzer en la consola
    if (state == "off") {
      digitalWrite(buzzer, LOW);
    } else {
      digitalWrite(buzzer, HIGH);
    }
  }
  server.send(200, "text/plain", "Estado del buzzer recibido");
}

void handleMulti() {
  if (server.hasArg("state")) {
    String state = server.arg("state");
    Serial.println("Multi estado: " + state);  // Imprime el estado de multi en la consola
    if (state == "off") {
      digitalWrite(multi, LOW);
    } else {
      digitalWrite(multi, HIGH);
    }
  }
  server.send(200, "text/plain", "Estado de multi recibido");
}

void handleBuzzerState() {
  server.send(200, "text/plain", String(img));  // Devuelve el estado de 'i'
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a WiFi");
  Serial.print("Dirección IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.on("/buzzer", HTTP_GET, handleBuzzer);             // Ruta para manejar el estado del buzzer
  server.on("/multi", HTTP_GET, handleMulti);               // Nueva ruta para manejar el estado de multi
  server.on("/buzzer/state", HTTP_GET, handleBuzzerState);  // Nueva ruta para consultar el estado del buzzer

  server.begin();
  Serial.println("Servidor iniciado");

  for (int i = 0; i < 7; i++) {
    pinMode(dataPins[i], OUTPUT);
  }
  for (int i = 0; i < 4; i++) {
    pinMode(controlPins[i], OUTPUT);
  }
  pinMode(buzzer, OUTPUT);
  pinMode(multi, OUTPUT);  // Configura el pin de multi como salida
}


void mapToPort(int number) {
  int mask = 0x01;
  for (int i = 0; i < 7; i++) {
    if ((numbers[number] & mask) == 0) {
      digitalWrite(dataPins[i], LOW);
    } else {
      digitalWrite(dataPins[i], HIGH);
    }
    mask = mask << 1;
  }
}

void clearControl() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(controlPins[i], LOW);
  }
}

void showDisplay(int number) {
  int numberInFunction = number;
  for (int display = 1; display <= 8;) {
    clearControl();
    delayMicroseconds(250 - timeOn);

    mapToPort(numberInFunction % 10);
    numberInFunction = numberInFunction / 10;

    int mask = 0x01;
    for (int i = 0; i < 4; i++) {
      if ((display & mask) == 0) {
        digitalWrite(controlPins[i], LOW);
      } else {
        digitalWrite(controlPins[i], HIGH);
      }
      delayMicroseconds(timeOn);
      mask = mask << 1;
    }
    display = display << 1;
  }
}

void loop() {
  server.handleClient();


  for (int i = numberStored; i > 0; i--) {
    for (int delay = 0; delay < 249; delay++) {
      if (upRoad)
        timeOn += 2;
      else
        timeOn -= 2;

      if (timeOn < 4) upRoad = true;
      if (timeOn > 246) upRoad = false;
      showDisplay(i);
    }
    img = 1;
    if (i <= 10) {    // Asegúrate de que i esté entre 0 y 50
      if (i == 10) {  //5-5
        digitalWrite(buzzer, HIGH);
      }  else if (i == 9) {
        digitalWrite(buzzer, LOW);
      } else if (i == 8) {
        digitalWrite(buzzer, HIGH);
      } else if (i == 7) {
        digitalWrite(buzzer, LOW);
      } else if (i == 6) {
        digitalWrite(buzzer, HIGH);
      } else if (i == 5) {
        digitalWrite(buzzer, LOW);
      } else if (i == 4) {
        digitalWrite(buzzer, HIGH);
      } else if (i == 3) {
        digitalWrite(buzzer, LOW);
      } else if (i == 2) {
        digitalWrite(buzzer, HIGH);
      } else if (i == 1) {
        digitalWrite(buzzer, LOW);
      }
    }

    if (i <= 1) {
      digitalWrite(buzzer, HIGH);
      digitalWrite(multi, HIGH);
      numberStored = 0;
      img = 0;
      for (int i = 0; i < 7; i++) {
        digitalWrite(dataPins[i], LOW);
      }
    } else if (i >= 141) {
      digitalWrite(buzzer, LOW);
    }

    if (i > 1) {
      digitalWrite(multi, LOW);
    }
  }
}
