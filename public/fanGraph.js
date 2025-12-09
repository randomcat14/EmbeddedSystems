
 
  

window.addEventListener("DOMContentLoaded", async () => {
  const yValues = [7,8,8,9,9,9,10,11,14,14,15];
  try {
    const response = await fetch("/api/db-test");
    const json = await response.json();   // <-- use response here

    if (!json.ok) {
      console.error("API error:", json.error);
      return;
    }

    const { v1 } = json;

    // Format timestamps for x-axis labels
    

    const ctx = document.getElementById("fanGraph");

    new Chart(ctx, {
      type: "line",
      data: {
        labels:yValues,          // x-axis = times
        datasets: [{
          fill: false,
          lineTension: 0,
          backgroundColor: "rgba(0,0,255,1.0)",
          borderColor: "rgba(0,0,255,0.1)",
          data: v1       // y-axis = voltage2 values
        }]
      },
      options: {
        plugins: {
          legend: { display: false },
          title: {
            display: true,
            text: "Voltage2 vs Time",
            font: { size: 16 }
          }
        }
      }
    });
  } catch (err) {
    console.error("Front-end error:", err);
  }
});
