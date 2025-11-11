
window.addEventListener("DOMContentLoaded", () => {
  const xValues = [0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24];
  const yValues = [7,8,8,9,9,9,10,11,14,14,15];

  const ctx = document.getElementById('myChart');

  new Chart(ctx, {
    type: "line",
    data: {
      labels: xValues,
      datasets: [{
        fill: false,
        lineTension: 0,
        backgroundColor: "rgba(0,0,255,1.0)",
        borderColor: "rgba(0,0,255,0.1)",
        data: yValues
      }]
    },
    options: {
      plugins: {
        legend: { display: false },
        title: {
          display: true,
          text: "Voltage vs Time",
          font: { size: 16 }
        }
      }
    }
  });
});
