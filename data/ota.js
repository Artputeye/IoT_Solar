document.getElementById("file").addEventListener("change", function () {
  const fileName = this.files.length > 0 ? this.files[0].name : "No file chosen";
  document.getElementById("file-name").textContent = fileName;
});

function upload() {
  const fileInput = document.getElementById("file");
  const type = document.getElementById("type").value;
  const progress = document.getElementById("progress");

  const file = fileInput.files[0];
  if (!file) return alert("Please select a file to upload");

  const xhr = new XMLHttpRequest();
  xhr.upload.onprogress = function (e) {
    if (e.lengthComputable) {
      progress.value = (e.loaded / e.total) * 100;
    }
  };

  xhr.onload = function () {
    if (xhr.status === 200) {
      alert("Upload complete! Rebooting...");
    } else {
      alert("Upload failed!");
    }
  };

  xhr.open("POST", "/update?type=" + type);  // ต้องตรงกับ path บน ESP32
  xhr.send(file);
}