document.getElementById("file").addEventListener("change", function () {
  const fileName = this.files.length > 0 ? this.files[0].name : "No file chosen";
  
  const fileNameDisplay = document.getElementById("file-name");
  if (fileNameDisplay) {
    fileNameDisplay.textContent = fileName;
  } else {
    console.warn("Element with ID 'file-name' not found. File name will not be displayed.");
  }
});

function upload() {
  const fileInput = document.getElementById("file");
  const typeElement = document.getElementById("type"); // Element สำหรับเลือกประเภท (select/input)
  const progressElement = document.getElementById("progress"); // Element สำหรับ progress bar

  const file = fileInput.files[0];
  if (!file) {
    alert("Please select a file to upload");
    return;
  }

  const updateType = typeElement.value; // ดึงค่า 'type' จาก element (เช่น "firmware", "filesystem")
  // **** เริ่มการสร้าง Data และ XMLHttpRequest ****
  var Data = new FormData();
  Data.append("file", file, file.name);
  Data.append("type", updateType);

  var xhr = new XMLHttpRequest();
  xhr.withCredentials = true; // หากเซิร์ฟเวอร์ ESP32 ต้องการ (มักไม่จำเป็นสำหรับ OTA พื้นฐาน)
  // จัดการความคืบหน้าการอัปโหลด
  xhr.upload.onprogress = function (e) {
    if (e.lengthComputable) {
      if (progressElement) {
        progressElement.value = (e.loaded / e.total) * 100;
      }
    }
  };

  xhr.onload = function () {
    if (xhr.status === 200) {
      console.log("Server response (onload):", xhr.responseText);
      // ตรวจสอบข้อความตอบกลับจาก ESP32
      if (xhr.responseText.trim() === "OK") { // ESP32 ของคุณส่ง "OK" กลับมา
        alert("Upload complete! Rebooting...");
      } else {
        alert("Upload Server response: " + xhr.responseText);
      }
    } else {
      console.error("Upload failed (onload):", xhr.status, xhr.responseText);
      alert("Upload HTTP Status: " + xhr.status + " - " + xhr.responseText);
    }
  };

  xhr.addEventListener("readystatechange", function() {
    if (this.readyState === 4) { // readyState 4 คือ REQUEST_FINISHED (คำขอเสร็จสมบูรณ์)
      console.log("Server response (readystatechange):", this.responseText);
     
    }
  });
  xhr.open("POST", "/"+ updateType); // ใช้ http:// นำหน้า
  console.log("POST", "/"+ updateType);
  xhr.send(Data);
}