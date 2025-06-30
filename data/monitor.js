
    const socket = new WebSocket(`ws://${location.host}/ws`);

    socket.onopen = () => {
      console.log("WebSocket connected");
    };

    socket.onmessage = event => {
      try {
        const data = JSON.parse(event.data);
        document.getElementById("gridVoltage").textContent      = data.gridVoltage;
        document.getElementById("gridFreq").textContent         = data.gridFreq;
        document.getElementById("outputVoltage").textContent    = data.outputVoltage;
        document.getElementById("outputFreq").textContent       = data.outputFreq;
        document.getElementById("apparentPower").textContent    = data.apparentPower;
        document.getElementById("activePower").textContent      = data.activePower;
        document.getElementById("loadPercent").textContent      = data.loadPercent;
        document.getElementById("busVoltage").textContent       = data.busVoltage;
        document.getElementById("batteryVoltage").textContent   = data.batteryVoltage;
        document.getElementById("temperature").textContent      = data.temperature;
        document.getElementById("pvCurrent").textContent        = data.pvCurrent;
        document.getElementById("pvVoltage").textContent        = data.pvVoltage;
        document.getElementById("inverterStatus").textContent   = data.inverterStatus;
      } catch (e) {
        console.error("Invalid JSON from server:", event.data);
      }
    };

    socket.onerror = error => {
      console.error("WebSocket error:", error);
    };
