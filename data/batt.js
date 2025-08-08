const toggle = document.getElementById("battypeToggle");
const batType = document.getElementById("battType");
const bulkCharge = document.getElementById("bulkCharging");
const floatingCharge = document.getElementById("floatingCharging");
const dcCuttoff = document.getElementById("lowDCcutoff");

function toggleSelect(checkbox) {
    const status = checkbox.checked ? "48V" : "24V";
    console.log(status);
    if (status == "48V") {
        set48();
    } else {
        set24();
    }
    collectAndSendSettings();
};

function set24() {
    batType.textContent = "24V";
    bulkCharge.textContent = "25.0 - 31.5V";
    floatingCharge.textContent = "25.0 - 31.5";
    dcCuttoff.textContent = "20.0 - 24.0V";
    updateDropdown('plain_batt', ['Full', '24.0', '24.5', '25.0', '25.5', '26.0', '26.5', '27.0', '27.5', '28.0', '28.5', '29.0']);
    updateDropdown('plain_utility', ['21.0', '21.5', '22.0', '22.5', '23.0', '23.5', '24.0', '24.5', '25.0', '25.5']);
    changeInputRange("voltage_charge", true);
    changeInputRange("voltage_floating", true);
    changeInput(true);
}

function set48() {
    batType.textContent = "48V";
    bulkCharge.textContent = "48.0 - 61.0V";
    floatingCharge.textContent = "48.0 - 61.0";
    dcCuttoff.textContent = "40.0 - 48.0V";
    updateDropdown('plain_batt', ['Full', '48', '49', '50', '51', '52', '53', '54', '55', '56', '57', '58']);
    updateDropdown('plain_utility', ['42', '43', '44', '45', '46', '47', '48', '49', '50', '51']);
    changeInputRange("voltage_charge", false);
    changeInputRange("voltage_floating", false);
    changeInput(false);
}

function updateDropdown(selectId, valuesArray) {
    const select = document.getElementById(selectId);
    select.innerHTML = ""; // เคลียร์ option เดิม

    valuesArray.forEach((value, index) => {
        const option = document.createElement("option");
        option.value = `${selectId}${index + 1}`; // เช่น plain_batt1, plain_batt2
        option.text = value;
        select.appendChild(option);
    });
}

function changeInputRange(selectId, V_Type) {
    const input = document.getElementById(selectId);
    if (V_Type) {
        input.min = "25.0";
        input.max = "31.5";
    } else {
        input.min = "48.0";
        input.max = "61.0";
    }
}

function changeInput(V_Type) {
    const input = document.getElementById("voltage_cutt");
    if (V_Type) {
        input.min = "20.0";
        input.max = "24.0";
    } else {
        input.min = "40.0";
        input.max = "48.0";
    }
}

function collectAndSendSettings() {
    const settings = {
        battypeToggle: document.getElementById('battypeToggle').checked ? '48V' : '24V',
        maximumChargingCurrent: document.getElementById('plain_ac').value,
        batteryType: document.getElementById('plain_batt_type').value,
        voltageBackToUtility: document.getElementById('plain_utility').value,
        voltageBackToBattery: document.getElementById('plain_batt').value,
        bulkChargingVoltage: parseFloat(document.getElementById('voltage_charge').value),
        floatingChargingVoltage: parseFloat(document.getElementById('voltage_floating').value),
        lowDCCutoff: parseFloat(document.getElementById('voltage_cutt').value)
    };
    console.log(settings);

    fetch('/battsetting', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(settings)
    })
        .then(response => {
            if (response.ok) {
                alert("Settings sent successfully!");
            } else {
                alert("Error sending settings!");
            }
        })
        .catch(error => {
            console.error("Fetch error:", error);
            alert("Failed to send settings.");
        });
}

document.addEventListener("DOMContentLoaded", function () {
    fetch('/getbattsetting')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            console.log(data.battypeToggle);
            // Toggle switch (24V = checked, 48V = unchecked)
            //const toggle = document.getElementById('battypeToggle');
            if (data.battypeToggle === '48V') {
                toggle.checked = (data.battypeToggle === '48V');
                batType.textContent = "48V";
                set48();

            } else {
                batType.textContent = "24V";
                set24();
            }

            // Dropdowns
            document.getElementById('plain_ac').value = data.maximumChargingCurrent;
            document.getElementById('plain_batt_type').value = data.batteryType;
            document.getElementById('plain_utility').value = data.voltageBackToUtility;
            document.getElementById('plain_batt').value = data.voltageBackToBattery;

            // Inputs
            document.getElementById('voltage_charge').value = data.bulkChargingVoltage;
            document.getElementById('voltage_floating').value = data.floatingChargingVoltage;
            document.getElementById('voltage_cutt').value = data.lowDCCutoff;
        })
        .catch(error => {
            console.error('Error fetching battery settings:', error);
        });
});


