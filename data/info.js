document.addEventListener("DOMContentLoaded", function () {
    const terminal = document.getElementById("terminal");

    const ws = new WebSocket(`ws://${window.location.host}/ws`);
    ws.binaryType = "arraybuffer";

    ws.onopen = () => appendToTerminal("✅ WebSocket Connected");

    ws.onmessage = (event) => {
        try {
            const base64Text = new TextDecoder().decode(new Uint8Array(event.data));// แปลง ArrayBuffer → String Base64
            const jsonText = atob(base64Text);// Decode Base64 → JSON string
            const data = JSON.parse(jsonText);// Parse JSON

            if (data["Inverter Data"]) {
                appendToTerminal(`Inverter Data: ${data["Inverter Data"]}`);
            }
            if (data["Command Data"]) {
                appendToTerminal(`Command Data: ${data["Command Data"]}`);
            }

        } catch (err) {
            console.error("Decode/Parse error", err);
        }
    };

    ws.onclose = () => appendToTerminal("❌ WebSocket Disconnected");

    document.getElementById("sendBtn").addEventListener("click", () => {
        const msg = document.getElementById("messageInput").value.trim();
        if (msg) {
            fetchToserver(msg);
            appendToTerminal(`Sent: ${msg}`);
            document.getElementById("messageInput").value = "";
        }
    });

    document.getElementById("clearBtn").addEventListener("click", () => {
        terminal.innerHTML = "";
        appendToTerminal("🧹 Terminal Cleared");
    });

    function appendToTerminal(message) {
        const div = document.createElement("div");
        div.textContent = message;
        terminal.appendChild(div);
        terminal.scrollTop = terminal.scrollHeight;
    }
});

function fetchToserver(message) {
    console.log(`${message} to Server`);
    const formdata = new FormData();
    formdata.append("plain", message);
    const requestOptions = {
        method: "POST",
        body: formdata,
        redirect: "follow"
    };
    fetch("/setting", requestOptions)
        .then((response) => response.text())
        .then((result) => console.log("Respond:", result))
        .catch((error) => console.error("Error:", error));
}
