function sendConfig() {
    const inputs = document.querySelectorAll('.setting-input input');
    const config = {};
    console.log(inputs);
    inputs.forEach(input => {
        if (input.id && input.value !== "") {
            config[input.id] = input.value;
        }
    });

    fetch('/networkconfig', {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json'
        },
        body: JSON.stringify(config)
    })
        .then(response => {
            if (response.ok) {
                alert("Configuration saved successfully.");
            } else {
                alert("Error saving configuration.");
            }
        })
        .catch(error => {
            console.error('Fetch error:', error);
            alert("Fetch failed.");
        });
}

document.querySelectorAll('.icon-btn').forEach(btn => {
    btn.addEventListener('click', () => {
        const targetId = btn.getAttribute('data-target');
        const input = document.getElementById(targetId);

        const isShown = input.type === 'text';
        input.type = isShown ? 'password' : 'text';

        if (!input) {
            console.error(`Element with id="${targetId}" not found`);
            return;
        }

        btn.setAttribute('aria-pressed', String(!isShown));
        btn.setAttribute('aria-label', isShown ? 'แสดงรหัสผ่าน' : 'ซ่อนรหัสผ่าน');
    });
});

document.addEventListener("DOMContentLoaded", () => {
    fetch('/getnetworkconfig')
        .then(response => {
            if (!response.ok) throw new Error('Network response was not ok');
            return response.json();
        })
        .then(data => {
            console.log("Received config:", data);

            // ใส่ค่าลงใน input ตาม id
            document.getElementById('wifi_name').value = data['wifi_name'] || '';
            document.getElementById('wifi_password').value = data['wifi_password'] || '';
            document.getElementById('mqtt_user').value = data['mqtt_user'] || '';
            document.getElementById('mqtt_password').value = data['mqtt_password'] || '';
            document.getElementById('mqtt_server').value = data['mqtt_server'] || '';
            document.getElementById('mqtt_port').value = data['mqtt_port'] || '';
        })
        .catch(error => {
            console.error('Error fetching network config:', error);
        });

});