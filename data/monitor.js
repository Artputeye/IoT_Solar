
////////////////////////////////////////////////////////////////////////////////////////////
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
    console.log(event.data);
    var myObj = JSON.parse(event.data); //แปลง json string เป็น json obj
    //console.log(myObj);
    var keys = Object.keys(myObj);//ดึงชื่อของทุก key ใน object myObj มาเก็บไว้ใน array ชื่อ keys เช่น ["voltage", "current"]

    for (var i = 0; i < keys.length; i++){ // Loop ค่าที่อยู่ใน json obj ไปแสดงใน html
        var key = keys[i];
        console.log(keys[i]);
        document.getElementById(key).innerHTML = myObj[key]; 
        console.log(myObj[key]);
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////
// const socket = new WebSocket(`ws://${location.host}/ws`);

// socket.onopen = () => {
//   console.log("WebSocket connected");
// };

// socket.onmessage = event => {
//   try {
//     const data = JSON.parse(event.data);
//     document.getElementById("gridVoltage").textContent      = data.gridVoltage;
//     document.getElementById("gridFreq").textContent         = data.gridFreq;
//     document.getElementById("outputVoltage").textContent    = data.outputVoltage;
//     document.getElementById("outputFreq").textContent       = data.outputFreq;
//     document.getElementById("apparentPower").textContent    = data.apparentPower;
//     document.getElementById("activePower").textContent      = data.activePower;
//     document.getElementById("loadPercent").textContent      = data.loadPercent;
//     document.getElementById("busVoltage").textContent       = data.busVoltage;
//     document.getElementById("batteryVoltage").textContent   = data.batteryVoltage;
//     document.getElementById("temperature").textContent      = data.temperature;
//     document.getElementById("pvCurrent").textContent        = data.pvCurrent;
//     document.getElementById("pvVoltage").textContent        = data.pvVoltage;
//     document.getElementById("inverterStatus").textContent   = data.inverterStatus;
//   } catch (e) {
//     console.error("Invalid JSON from server:", event.data);
//   }
// };

// socket.onerror = error => {
//   console.error("WebSocket error:", error);
// };