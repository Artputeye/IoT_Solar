var gateway = `ws://${window.location.hostname}/ws`;
var websocket;

// Init web socket when the page loads
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
}

function getReadings(){
    websocket.send("getReadings");
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.binaryType = "arraybuffer"; // สำคัญ! รับเป็น ArrayBuffer
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

// When websocket is established, call the getReadings() function
function onOpen(event) {
    console.log('Connection opened');
    getReadings();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

// Function that receives the message from the ESP32 with the readings
function onMessage(event) {
    try {
        // แปลง ArrayBuffer → String Base64
        const base64Text = new TextDecoder().decode(new Uint8Array(event.data));

        // Decode Base64 → JSON string
        const jsonText = atob(base64Text);

        // Parse JSON string → object
        var myObj = JSON.parse(jsonText);

        var keys = Object.keys(myObj);

        for (var i = 0; i < keys.length; i++){
            var key = keys[i];
            if (document.getElementById(key)) { // ป้องกัน key ที่ไม่มี element
                document.getElementById(key).innerHTML = myObj[key]; 
            }
        }

    } catch (err) {
        console.error("Decode/Parse error", err);
    }
}
