
document.addEventListener("DOMContentLoaded", () => {
  const spikeToggle = document.getElementById("toggle-spike");


  const userEmail = "ashleyjefferson245@gmail.com";

  spikeToggle?.addEventListener("change", async (e) => {
    if (!e.target.checked) return; 

    try {
      const res = await fetch("http://localhost:3000/api/send-spike-email", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ email: userEmail })
      });
      const data = await res.json();
      if (data.ok) {
        alert("Spike email sent!");
      } else {
        alert("Error: " + (data.error || "Failed to send"));
      }
    } catch (err) {
      console.error(err);
      alert("Network error sending email.");
    }
  });
});
