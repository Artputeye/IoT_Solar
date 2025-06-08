  function toggleSetting(checkbox, settingName) {
    const status = checkbox.checked ? "ON" : "OFF";
    //console.log(`${settingName} is now ${status}`);
    const cmd = settingName + " " + status;
    console.log(cmd);
    toggleToserver(cmd);
    // ตัวอย่าง: ถ้าคุณต้องการเรียกใช้งานฟังก์ชันอื่นหรือส่งข้อมูลไปยัง server:
    // sendSettingToServer(settingName, checkbox.checked);
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

  fetch("/post", requestOptions)
    .then((response) => response.text())
    .then((result) => console.log("ผลลัพธ์จากเซิร์ฟเวอร์:", result))
    .catch((error) => console.error("เกิดข้อผิดพลาด:", error));
}