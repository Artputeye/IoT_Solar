document.addEventListener("DOMContentLoaded", function () {
  fetch('/getsetting')
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
        const checkbox = document.getElementById(key);
        if (checkbox) {
          checkbox.checked = data[key] === "1";
        }
      }
    })
    .catch(error => {
      console.error("Error fetching toggle data:", error);
    });
});

function toggleSetting(checkbox, settingName) {
  const status = checkbox.checked ? 1 : 0;
  console.log(`${settingName} ${status}`);
  settingToserver(settingName, status);  // ✅ ส่งชื่อกับค่าแยกกัน
}

// ส่งค่า Toggle ไปยังเซิร์ฟเวอร์
function settingToserver(settingName, state) {
  console.log(`${settingName} has been toggled to ${state}`);

  fetch('/setting', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify({
      setting: settingName,   // ✅ ไม่มีเลขติดชื่อ
      value: state
    }),
  })
    .then(response => response.text())
    .then(result => {
      console.log(`✅ Toggle ${settingName} updated to ${state}`);
      console.log("Respond:", result);
    })
    .catch(error => {
      console.error("❌ Error:", error);
    });

  submitAllSettings();
}

//ส่งค่า dropdown ไปยังเซิร์ฟเวอร์
function sendSetting(data) {
  const container = data.closest('.setting-dropdown');
  const settingType = container.getAttribute('data-setting');
  const value = container.querySelector('select').value;

  fetch('/setting', {
    method: 'POST',
    headers: {
      'Content-Type': 'application/json',
    },
    body: JSON.stringify({ setting: settingType, value }),
  })
    .then(response => response.text())
    .then(data => {
      console.log(`✅ Setting ${settingType} updated to ${value}`);
    })
    .catch(error => {
      console.error('❌ Error:', error);
    });
    submitAllSettings();
}

function submitAllSettings() { 
  const data = {};

  // เก็บค่าจาก toggle switch ทั้งหมด
  document.querySelectorAll('input[type="checkbox"]').forEach(input => {
    const id = input.id;
    if (id) {
      data[id] = input.checked ? "1" : "0";
    }
  });

  // เก็บค่าจาก dropdown ทั้งหมด
  document.querySelectorAll('select').forEach(select => {
    const id = select.id;
    if (id) {
      data[id] = select.value;   // <-- ใช้ value ไม่ใช่ text
    }
  });

  console.log("Successed:", data);

  // ส่ง JSON ไปยัง ESP32
  fetch('/savesetting', {
    method: 'POST',
    headers: { 'Content-Type': 'application/json' },
    body: JSON.stringify(data)
  })
    .then(response => response.text())
    .then(result => {
      console.log("Successed:", result);
    })
    .catch(error => {
      console.error("Error:", error);
    });
}

function restoreDefaults() {
  console.log("⚠️ Restoring defaults...");

  // รีเซ็ต dropdown → ค่าแรก
  document.querySelectorAll('select').forEach(select => {
    if (select.options.length > 0) {
      select.selectedIndex = 0;
    }
  });

  // รีเซ็ต toggle switch → 0
  document.querySelectorAll('input[type="checkbox"]').forEach(cb => {
    cb.checked = false;
  });

  // ส่งค่าที่รีเซ็ตไปยัง ESP32
  settingToserver("RestoreDefaults", 1) 
  submitAllSettings();
  alert("✅ Settings restored to defaults.");
}
