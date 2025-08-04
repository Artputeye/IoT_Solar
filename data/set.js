document.addEventListener("DOMContentLoaded", function () {
  fetch('/getstatus')
    .then(response => response.json())
    .then(data => {
      //console.log(data);
      for (const key in data) {
        const selectElement = document.getElementById(key);

        if (selectElement && selectElement.tagName === "SELECT") {
          const valueToSelect = data[key];
          console.log(data);
          for (const option of selectElement.options) {
            console.log(key);
            if (option.value.toLowerCase() === valueToSelect.toLowerCase()) {
              selectElement.value = option.value;
              break;
            }
          }
        }
      }

      for (const key in data) {
        const checkbox  = document.getElementById(key);
        if (checkbox ) {
          checkbox .checked = data[key] === "ON";
        }
      }



    })
    .catch(error => {
      console.error("Error fetching toggle data:", error);
    });
});

function toggleSetting(checkbox, settingName) {
const status = checkbox.checked ? "ON" : "OFF";
//console.log(`${settingName} is now ${status}`);
const cmd = settingName + " " + status;
console.log(cmd);
toggleToserver(cmd);
}

// ส่ง POST ไปยังเซิร์ฟเวอร์
function toggleToserver(settingName) {
    console.log(`${settingName} has been toggled`);
    const formdata = new FormData();
    
    formdata.append("plain", settingName);
    const requestOptions = {
    method: "POST",
    body: formdata,
    redirect: "follow"
  };
  submitAllSettings();
  fetch("/setting", requestOptions)
    .then((response) => response.text())
    .then((result) => console.log("ผลลัพธ์จากเซิร์ฟเวอร์:", result))
    .catch((error) => console.error("เกิดข้อผิดพลาด:", error));
}

//ส่งค่า dropdown ไป server
document.querySelectorAll('.setting-form').forEach(form => {
    form.addEventListener('submit', function (event) {
        event.preventDefault();

        const select = form.querySelector('select');
        const settingType = form.getAttribute('data-setting');
        const value = select.value;
        submitAllSettings();
        fetch('/setting', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/x-www-form-urlencoded',
            },
            body: new URLSearchParams({
                setting: settingType,
                value: value
            }),
        })
        .then(response => response.text())
        .then(data => {
            console.log(`Setting ${settingType} updated to ${value}`);
            // Optional: Show success message in UI
            //alert(`Setting "${settingType}" updated to "${value}"`);
            //alert(`Updated`);
        })
        .catch(error => {
            console.error('Error:', error);
            //alert('Failed to update setting: ' + settingType);
        });
    });
});



function submitAllSettings() {
  const data = {};

  // เก็บค่าจาก toggle switch ทั้งหมด
  document.querySelectorAll('input[type="checkbox"]').forEach(input => {
    const id = input.id;
    if (id) {
      data[id] = input.checked ? "ON" : "OFF";
    }
  });

  // เก็บค่าจาก dropdown ทั้งหมด
  document.querySelectorAll('select').forEach(select => {
    const id = select.id;
    const selectedOption = select.options[select.selectedIndex];
    if (id && selectedOption) {
      data[id] = selectedOption.text;
    }
  });
  console.log("ส่งสำเร็จ:", data);
  
  // ส่ง JSON ไปยัง ESP32
  fetch('/setall', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json'
    },
    body: JSON.stringify(data)
  })
    .then(response => response.text())
    .then(result => {
      console.log("ส่งสำเร็จ:", result);
      //alert("Settings updated successfully!");
    })
    .catch(error => {
      console.error("เกิดข้อผิดพลาด:", error);
      //alert("Error saving settings.");
    });
}