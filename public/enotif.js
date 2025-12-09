function hexToRGB(hex) {
  const r = parseInt(hex.slice(1, 3), 16);
  const g = parseInt(hex.slice(3, 5), 16);
  const b = parseInt(hex.slice(5, 7), 16);
  return [r, g, b];
}
document.addEventListener("DOMContentLoaded", () => {
  const picker = document.getElementById("LEDColor");

  picker.addEventListener("input", () => {
    const [r, g, b] = hexToRGB(picker.value);
    console.log("R:", r, "G:", g, "B:", b);
  });

  const spikeToggle = document.getElementById("toggle-spike");


  const userEmail = "ashleyjefferson245@gmail.com";

  spikeToggle?.addEventListener("change", async (e) => {
    if (!e.target.checked) return; 

    try {
      const res = await fetch("http://localhost:3000/api/send-email", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ email: userEmail })
      });
      const data = await res.json();
      if (data.ok) {
        alert("Email sent!");
      } else {
        alert("Error: " + (data.error || "Failed to send"));
      }
    } catch (err) {
      console.error(err);
      alert("Network error sending email.");
    }
  });
});
document.addEventListener("DOMContentLoaded", () => {
  fetch("/api/test-db");
});
