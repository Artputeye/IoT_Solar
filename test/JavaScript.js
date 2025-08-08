// ดึง element ของ toggle switch และ label มาเก็บไว้ในตัวแปร
const toggleSwitch = document.getElementById('voltageToggle');
const voltageLabel = document.getElementById('voltageLabel');

// กำหนดค่าเริ่มต้น
let currentVoltage = 24;

// ฟังก์ชันสำหรับเปลี่ยนค่าโวลต์
function toggleVoltage() {
    // ถ้าสวิตช์ถูกเปิดอยู่ (checked)
    if (toggleSwitch.checked) {
        currentVoltage = 48; // เปลี่ยนค่าเป็น 48
    } else {
        currentVoltage = 24; // เปลี่ยนค่ากลับเป็น 24
    }
    
    // อัปเดตข้อความใน label
    voltageLabel.textContent = currentVoltage + 'V';
    console.log(`Voltage changed to: ${currentVoltage}V`);
}

// เพิ่ม Event Listener ให้กับ switch
// เมื่อสถานะของ switch เปลี่ยนแปลง (onchange) ให้เรียกใช้ฟังก์ชัน toggleVoltage
toggleSwitch.addEventListener('change', toggleVoltage);