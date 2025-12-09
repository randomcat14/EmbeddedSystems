  <button onclick="triggerWarning()">Trigger Warning</button>

      <div id="popup-overlay" class="hidden">
        <div class="popup">
          <h2>⚠ Pump Failure Warning</h2>
          <p id="popup-text"></p>
          <button onclick="closePopup()">Acknowledge</button>
        </div>
      </div>
 <script>
    function triggerWarning() {
      document.getElementById("popup-text").textContent =
        "Pump 3 is predicted to fail in 2 minutes.\nTemperature: 72.6°F";
      document.getElementById("popup-overlay").classList.remove("hidden");
    }

    function closePopup() {
      document.getElementById("popup-overlay").classList.add("hidden");
    }

    
    function checkPumpStatus() {
      let pumpFailing = true; 
      if (pumpFailing) {
        triggerWarning();
      }
    }

    
    setInterval(checkPumpStatus, 2000);
  </script>