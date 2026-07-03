#include "WebServerManager.h"

#include <ESP8266WebServer.h>

#include "SensorManager.h"
#include "MqttManager.h"
#include "SolarLogic.h"
#include "HistoryManager.h"
#include "ConfigManager.h"
#include "PumpController.h"

ESP8266WebServer server(80);

// --------------------------------------------------
// FORWARD DECLARATIONS
// --------------------------------------------------
void handleRoot();
void handleApiData();
void handleApiHistory();
void handleSettings();
void handleSaveSettings();

// --------------------------------------------------
// INIT
// --------------------------------------------------
void WebServerManager::begin()
{
    server.on("/", handleRoot);
    server.on("/api/data", handleApiData);
    server.on("/api/history", handleApiHistory);

    server.on("/api/sensors", HTTP_GET, []() {
        server.send(200, "application/json", SensorManager::getSensorListJson());
    });

    // 🔥 SENSOR MAPPING SAVE
server.on("/saveMapping", HTTP_POST, []() {

    String id = server.arg("id");
    String role = server.arg("role");

    ConfigData &cfg = ConfigManager::get();

    if (role == "vorlauf") cfg.sensors.vorlaufID = id;
    else if (role == "ruecklauf") cfg.sensors.ruecklaufID = id;
    else if (role == "case") cfg.sensors.caseID = id;

    ConfigManager::save();

    server.send(200, "text/plain", "OK");
});

    server.on("/settings", handleSettings);
    server.on("/save", HTTP_POST, handleSaveSettings);

    server.begin();

    Serial.println("[WEB] Server gestartet");
}

// --------------------------------------------------
// LOOP
// --------------------------------------------------
void WebServerManager::loop()
{
    server.handleClient();
}

// --------------------------------------------------
// ROOT DASHBOARD
// --------------------------------------------------
void handleRoot()
{
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Solar Monitor</title>

<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>

<style>
body { font-family: Arial; background:#111; color:#fff; }
.card {
    background:#222;
    padding:15px;
    margin:10px;
    border-radius:10px;
    display:inline-block;
    width:150px;
}
</style>
</head>

<body>

<h2>🌞 Solar Monitor</h2>

<div class="card">Vorlauf<br><span id="v">-</span></div>
<div class="card">Ruecklauf<br><span id="r">-</span></div>
<div class="card">Case<br><span id="c">-</span></div>
<div class="card">Delta<br><span id="d">-</span></div>

<div class="card">MQTT<br><span id="mqtt">-</span></div>
<div class="card">State<br><span id="state">-</span></div>
<div class="card">Pumpe<br><span id="pump">-</span></div>

<div class="card">
<a href="/settings" style="color:white;">⚙ Settings</a>
</div>

<div class="card" style="width:90%;">
<canvas id="chart"></canvas>
</div>

<script>

let chart;

function initChart() {
    const ctx = document.getElementById('chart').getContext('2d');

    chart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [
                { label: 'Vorlauf', data: [], borderColor: 'red' },
                { label: 'Ruecklauf', data: [], borderColor: 'blue' },
                { label: 'Case', data: [], borderColor: 'green' }
            ]
        },
        options: {
            responsive: true
        }
    });
}

function update() {
    fetch('/api/data')
    .then(r => r.json())
    .then(data => {

        document.getElementById("v").innerHTML = data.v;
        document.getElementById("r").innerHTML = data.r;
        document.getElementById("c").innerHTML = data.c;
        document.getElementById("d").innerHTML = data.delta;

        document.getElementById("mqtt").innerHTML =
            data.mqtt ? "🟢 CONNECTED" : "🔴 OFFLINE";

        document.getElementById("state").innerHTML = data.state;
        document.getElementById("pump").innerHTML = data.pump ? "🟢 EIN" : "⚪ AUS";
    });
}

function updateHistory() {
    fetch('/api/history')
    .then(r => r.json())
    .then(data => {

        chart.data.labels = [];

        let v = [];
        let r = [];
        let c = [];

        data.values.forEach((e, i) => {
            chart.data.labels.push(i);
            v.push(e.v);
            r.push(e.r);
            c.push(e.c);
        });

        chart.data.datasets[0].data = v;
        chart.data.datasets[1].data = r;
        chart.data.datasets[2].data = c;

        chart.update();
    });
}

initChart();
setInterval(update, 2000);
setInterval(updateHistory, 5000);

update();
updateHistory();

</script>

</body>
</html>
)rawliteral";

    server.send(200, "text/html", html);
}

// --------------------------------------------------
// LIVE DATA
// --------------------------------------------------
void handleApiData()
{
    float v = SensorManager::getVorlauf();
    float r = SensorManager::getRuecklauf();
    float c = SensorManager::getCase();
    float d = SensorManager::getVorlauf() - SensorManager::getRuecklauf();

    String json = "{";
    json += "\"v\":" + String(v) + ",";
    json += "\"r\":" + String(r) + ",";
    json += "\"c\":" + String(c) + ",";
    json += "\"delta\":" + String(d) + ",";
    json += "\"mqtt\":" + String(MqttManager::isConnected() ? 1 : 0) + ",";
    json += "\"pump\":" + String(PumpController::isRunning() ? 1 : 0) + ",";
    json += "\"state\":\"" + SolarLogic::getStateString() + "\"";
    json += "}";

    server.send(200, "application/json", json);
}

// --------------------------------------------------
// HISTORY
// --------------------------------------------------
void handleApiHistory()
{
    String json = "{";
    json += "\"size\":" + String(HistoryManager::size()) + ",";
    json += "\"values\":[";

    for (int i = 0; i < HistoryManager::size(); i++)
    {
        HistoryEntry e = HistoryManager::get(i);

        json += "{";
        json += "\"v\":" + String(e.v) + ",";
        json += "\"r\":" + String(e.r) + ",";
        json += "\"c\":" + String(e.c);
        json += "}";

        if (i < HistoryManager::size() - 1) json += ",";
    }

    json += "]}";

    server.send(200, "application/json", json);
}

// --------------------------------------------------
// SETTINGS PAGE
// --------------------------------------------------
void handleSettings()
{
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Settings</title>
<style>
body { font-family: Arial; background:#111; color:#fff; padding:20px; }
.card { background:#222; padding:15px; margin:10px 0; border-radius:10px; }
input { width:100%; padding:8px; margin:5px 0; }
.sensor { background:#1a1a1a; padding:10px; margin:5px 0; border-radius:8px; }
.warn { color:orange; }
</style>
</head>
<body>

<h2>Settings</h2>

<div class="card">
<h3>WiFi / MQTT</h3>

<form action="/save" method="POST">

WiFi SSID:<br>
<input name="ssid"><br>

Password:<br>
<input name="pass" type="password"><br>

MQTT Server:<br>
<input name="mqtt"><br>

Port:<br>
<input name="port"><br>

<button type="submit">Save</button>
</form>
</div>

<div class="card">
<h3>Sensoren</h3>
<div id="sensors">Lade...</div>
</div>

<script>

fetch('/api/sensors')
.then(r => r.json())
.then(data => {

    let html = '';

    if (data.length === 1 && data[0].id === "NO_SENSORS") {
        html = "<div class='sensor warn'>Keine Sensoren erkannt</div>";
    } else {
        data.forEach(s => {

            html += "<div class='sensor'>";
            html += "Index: " + s.index + "<br>";
            html += "ID: " + s.id + "<br><br>";

            html += "<select onchange='saveMapping(\"" + s.id + "\", this.value)'>";
            html += "<option value='none'>keine Zuordnung</option>";
            html += "<option value='vorlauf'>Vorlauf</option>";
            html += "<option value='ruecklauf'>Rücklauf</option>";
            html += "<option value='case'>Case</option>";
            html += "</select>";

            html += "</div>";
        });
    }

    document.getElementById("sensors").innerHTML = html;
});

function saveMapping(id, role)
{
    fetch('/saveMapping', {
        method: 'POST',
        headers: {'Content-Type': 'application/x-www-form-urlencoded'},
        body: 'id=' + id + '&role=' + role
    });
}

</script>

</body>
</html>
)rawliteral";

    server.send(200, "text/html", html);
}

// --------------------------------------------------
// SAVE SETTINGS
// --------------------------------------------------
void handleSaveSettings()
{
    ConfigData &cfg = ConfigManager::get();

    cfg.wifi_ssid = server.arg("ssid");
    cfg.wifi_pass = server.arg("pass");
    cfg.mqtt_server = server.arg("mqtt");
    cfg.mqtt_port = server.arg("port").toInt();

    ConfigManager::save();

    server.send(200, "text/html", "Saved. Rebooting...");

    delay(1000);
    ESP.restart();
}