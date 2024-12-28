#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define ROW_1 15
#define ROW_2 2
#define ROW_3 0
#define ROW_4 4
#define ROW_5 5
#define ROW_6 18
#define ROW_7 19
#define ROW_8 21

#define COL_1 12
#define COL_2 14
#define COL_3 27
#define COL_4 26
#define COL_5 25
#define COL_6 33
#define COL_7 32
#define COL_8 13

// Arrays to store pin numbers
const int row_pins[8] = {ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8};
const int col_pins[8] = {COL_1, COL_2, COL_3, COL_4, COL_5, COL_6, COL_7, COL_8};

const char* ssid = "ARRIS-EC79";
const char* password = "nWajbPtXgJiL";

WebServer server(80);
bool ledMatrix[8][8] = {0};

// Rectangle pattern (1 means LED on, 0 means LED off)
/*const byte rectangle[8][8] = {
  {1, 1, 1, 1, 1, 1, 0, 0},
  {1, 0, 0, 0, 0, 1, 0, 0},
  {1, 0, 0, 0, 0, 1, 0, 0},
  {1, 0, 0, 0, 0, 1, 0, 0},
  {1, 0, 0, 0, 0, 1, 0, 0},
  {1, 1, 1, 1, 1, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};*/

//Page html
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
    <title>Contrôle Matrice LED</title>
    <style>
        .grid {
            display: grid;
            grid-template-columns: repeat(8, 40px);
            gap: 2px;
            margin: 20px;
        }
        .cell {
            width: 40px;
            height: 40px;
            border: 1px solid #ccc;
            background-color: white;
            cursor: pointer;
        }
        .active {
            background-color: #00ff00;
        }
        button {
            margin: 20px;
            padding: 10px 20px;
        }
    </style>
</head>
<body>
    <div class="grid" id="ledGrid"></div>
    <button onclick="sendData()">Envoyer à la matrice</button>
    <script>
        let matrix = Array(8).fill().map(() => Array(8).fill(false));
        
        // Créer la grille
        const grid = document.getElementById('ledGrid');
        for(let i = 0; i < 8; i++) {
            for(let j = 0; j < 8; j++) {
                const cell = document.createElement('div');
                cell.className = 'cell';
                cell.onclick = () => {
                    matrix[i][j] = !matrix[i][j];
                    cell.classList.toggle('active');
                };
                grid.appendChild(cell);
            }
        }
        
        // Envoyer les données
        function sendData() {
            fetch('/update', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify({matrix: matrix})
            })
            .then(response => response.text())
            .then(data => {
                if(data === 'OK') {
                    console.log('Mise à jour réussie');
                } else {
                    console.error('Erreur lors de la mise à jour');
                }
            });
        }
    </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);
    
    // Configuration des pins
    for (int i = 0; i < 8; i++) {
        pinMode(row_pins[i], OUTPUT);
        pinMode(col_pins[i], OUTPUT);
        digitalWrite(row_pins[i], LOW);
        digitalWrite(col_pins[i], HIGH);
    }
    
    // Connexion WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connecté à ");
    Serial.println(ssid);
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
    // Routes du serveur web
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html", index_html);
    });
    
    server.on("/update", HTTP_POST, []() {
        String json = server.arg("plain");
        
        // Créer un document JSON pour parser les données
        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, json);
        
        if (error) {
            Serial.print("Erreur de parsing JSON: ");
            Serial.println(error.c_str());
            server.send(400, "text/plain", "Erreur JSON");
            return;
        }
        
        // Mettre à jour la matrice LED
        JsonArray matrix = doc["matrix"];
        for (int i = 0; i < 8; i++) {
            JsonArray row = matrix[i];
            for (int j = 0; j < 8; j++) {
                ledMatrix[i][j] = row[j].as<bool>();
            }
        }
        
        Serial.println("Matrice mise à jour");
        server.send(200, "text/plain", "OK");
    });
    
    server.begin();
}

void loop() {
  server.handleClient();
  displayPattern();
}

void displayPattern() {
  // Scan through each row
  for (int row = 0; row < 8; row++) {
    // Turn off all rows first
    for (int i = 0; i < 8; i++) {
      digitalWrite(row_pins[i], LOW);
    }
    
    // Set columns for the current row first
    for (int col = 0; col < 8; col++) {
      // For columns: LOW = ON, HIGH = OFF
      digitalWrite(col_pins[col], !ledMatrix[row][col]);
    }
    
    // Then activate the current row
    // For rows: HIGH = ON, LOW = OFF
    digitalWrite(row_pins[row], HIGH);
    
    // Small delay for multiplexing
    delayMicroseconds(500); // Increased delay for better visibility
  }
}