//ส่งค่า dropdown ไป server
document.querySelectorAll('.setting-dropdown').forEach(form => {
    form.addEventListener('submit', function (event) {
        event.preventDefault();

        const select = form.querySelector('select');
        const settingType = form.getAttribute('data-setting');
        const value = select.value;

        fetch('/post', {
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
            alert('Failed to update setting: ' + settingType);
        });
    });
});


// รอให้โครงสร้างเอกสาร HTML โหลดเสร็จสมบูรณ์ก่อนที่จะเริ่มเข้าถึงองค์ประกอบต่างๆ
document.addEventListener('DOMContentLoaded', () => {
    // เลือกฟอร์มทั้งหมดที่มีคลาส 'setting-input'
    const settingForms = document.querySelectorAll('form.setting-input');

    // วนลูป (Loop) ไปทีละฟอร์มที่พบ
    settingForms.forEach(form => {
        // เพิ่มตัวฟังเหตุการณ์ (Event Listener) สำหรับเหตุการณ์ 'submit' (เมื่อกดปุ่มส่งข้อมูล) ให้กับแต่ละฟอร์ม
        form.addEventListener('submit', async (event) => {
            event.preventDefault(); // ป้องกันการส่งฟอร์มตามค่าเริ่มต้นของเบราว์เซอร์ (ซึ่งจะทำให้หน้าเว็บโหลดใหม่)

            // ดึงประเภทการตั้งค่าเฉพาะจากแอตทริบิวต์ data-setting ของฟอร์มนี้
            const settingType = form.dataset.setting;

            // ค้นหาองค์ประกอบ input แบบไดนามิกภายในฟอร์มนี้โดยเฉพาะ
            // เราสามารถรับ ID ของ input จากค่า data-setting หรือถ้า ID มีรูปแบบที่สอดคล้องกัน
            // เราก็สามารถเลือก input[type="number"] ตัวแรกภายในฟอร์มนี้ได้
            // เพื่อความแข็งแกร่ง เราจะสมมติว่า ID ของ input ตรงกับค่า data-setting (เช่น voltage_charge, voltage_floating, voltage_cutt)
            const inputId = settingType; // สมมติว่า ID ของ input เหมือนกับค่า data-setting
            const inputElement = form.querySelector(`#${inputId}`); // เลือก input โดยใช้ ID ภายในฟอร์มนี้

            // (ทางเลือก) หาก ID ไม่ตรงกับ data-setting หรือต้องการใช้แอตทริบิวต์ name แทน
            // const inputElement = form.querySelector('input[type="number"]');

            if (!inputElement) {
                console.error(`ข้อผิดพลาด: ไม่พบองค์ประกอบ input สำหรับประเภทการตั้งค่า "${settingType}"`);
                alert('เกิดข้อผิดพลาดภายใน: ไม่พบองค์ประกอบ input');
                return;
            }

            const inputValue = inputElement.value; // รับค่าที่ผู้ใช้ป้อนจากช่อง input

            // ตรวจสอบความถูกต้องเบื้องต้น
            if (inputValue === "" || isNaN(parseFloat(inputValue))) {
                alert(`กรุณาป้อนค่า ${settingType} ที่ถูกต้อง`); // ข้อความแจ้งเตือนแบบไดนามิก
                return;
            }

            // เตรียมข้อมูลที่จะส่งไปยังเซิร์ฟเวอร์
            const dataToSend = {
                setting: settingType,
                value: parseFloat(inputValue) // แปลงค่า string เป็นตัวเลขทศนิยม
            };

            // กำหนดปลายทาง (Endpoint) ของเซิร์ฟเวอร์ที่คุณต้องการส่งข้อมูลไป
            // *** สำคัญ: กรุณาแทนที่ '/api/update-setting' ด้วย URL จริงของเซิร์ฟเวอร์ของคุณ ***
            const serverEndpoint = '/post'; // ตัวอย่าง URL
            console.log(inputId + " " + inputValue);
            console.log(inputValue);

            try {
                // ส่งข้อมูลโดยใช้ Fetch API
                const response = await fetch(serverEndpoint, {
                    method: 'POST', // ใช้วิธี POST ในการส่งข้อมูล
                    headers: {
                        'Content-Type': 'application/json' // แจ้งเซิร์ฟเวอร์ว่าข้อมูลที่เราส่งเป็น JSON
                    },
                    body: JSON.stringify(dataToSend) // แปลงวัตถุ JavaScript ให้เป็นสตริง JSON
                });

                // ตรวจสอบว่าการตอบกลับจากเซิร์ฟเวอร์สำเร็จหรือไม่ (รหัสสถานะ 200-299)
                if (response.ok) {
                    const result = await response.json(); // แยกวิเคราะห์ข้อมูล JSON ที่ได้รับจากเซิร์ฟเวอร์
                    console.log(`การตอบกลับจากเซิร์ฟเวอร์สำหรับ ${settingType}:`, result);
                    //alert(`ตั้งค่า '${settingType}' สำเร็จ! เซิร์ฟเวอร์ตอบกลับ: ` + (result.message || 'สำเร็จ'));
                    // คุณอาจต้องการอัปเดต UI หรือแสดงข้อความยืนยันความสำเร็จเฉพาะสำหรับ Card นี้
                } else {
                    // จัดการข้อผิดพลาดจากเซิร์ฟเวอร์
                    const errorData = await response.json(); // พยายามแยกวิเคราะห์ข้อความข้อผิดพลาดจากเซิร์ฟเวอร์
                    console.error(`ตั้งค่า '${settingType}' ไม่สำเร็จ:`, response.status, errorData);
                    //alert(`ตั้งค่า '${settingType}' ไม่สำเร็จ: ` + (errorData.message || 'ข้อผิดพลาดที่ไม่ทราบสาเหตุ'));
                }
            } catch (error) {
                // จัดการข้อผิดพลาดเกี่ยวกับเครือข่าย
                console.error(`ข้อผิดพลาดเครือข่ายหรือไม่สามารถเข้าถึงเซิร์ฟเวอร์ได้สำหรับ ${settingType}:`, error);
                //alert('ไม่สามารถเชื่อมต่อกับเซิร์ฟเวอร์ได้ กรุณาตรวจสอบการเชื่อมต่อของคุณ');
            }
        });
    });
});