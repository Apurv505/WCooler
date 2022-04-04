const char index_html[] PROGMEM = R"rawliteral(
  
<!DOCTYPE HTML>
<html>

<head>
  <title>ESP IOT DASHBOARD</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
    integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
      font-family: 'Times New Roman', Times, serif;
      display: inline-block;
      text-align: center;
    }

    h2 {
      color: rgb(209, 90, 0);
      padding: 10px;
      text-align: center;
      font-size: 30px;
      font-family: 'Times New Roman', Times, serif;
    }

    h3 {
      font-size: 1.8rem;
      color: white;
    }

    h4 {
      font-size: 1.2rem;
      color: rgb(107, 160, 251)
    }

    p {
      font-size: 1rem;
    }

    body {
      margin: 0;
      background-color: black;
      color: white;

    }

    /* .switch {position: relative; display: inline-block; width: 60px; height: 48px; }
    .switch input {display: none;} */
    /* .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 68px;   opacity: 0.8;   cursor: pointer;} */
    /* .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px} */
    input:checked+.slider {
      background-color: #1b78e2
    }

    input:checked+.slider:before {
      -webkit-transform: translateX(52px);
      -ms-transform: translateX(52px);
      transform: translateX(52px)
    }

    .topnav {
      overflow: hidden;
      background-color: #1b78e2;
    }

    .content {
      padding: 20px;
    }

    .card {
      background-color: black;
      box-shadow: 2px 2px 12px 1px rgba(47, 180, 247, 0.722);
    }

    .cards {
      max-width: 700px;
      margin: 0 auto;
      display: grid;
      grid-gap: 2rem;
      grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    }

    .slider2 {
      -webkit-appearance: none;
      margin: 14px;
      height: 20px;
      background: rgb(255, 170, 1);
      outline: none;
      opacity: 0.8;
      -webkit-transition: .2s;
      transition: opacity .2s;
      margin-bottom: 40px;
    }

    .slider:hover,
    .slider2:hover {
      opacity: 1;
    }

    .slider2::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 40px;
      height: 40px;
      background: #008B74;
      cursor: pointer;
    }

    .slider2::-moz-range-thumb {
      width: 40px;
      height: 40px;
      background: #008B74;
      cursor: pointer;
    }

    .reading {
      font-size: 2.6rem;
    }

    .card-switch {
      color: #50a2ff;
    }

    .card-light {
      color: #008B74;
    }

    .card-bme {
      color: #572dfb;
    }

    .card-motion {
      color: #3b3b3b;
    }

    .icon-pointer {
      cursor: pointer;
    }

    .speedDisplay {
      font-size: 0.4rem;
      color: #50a2ff;
    }

    .slider {
      -webkit-appearance: true;
      width: 40%;
      height: 15px;
      border-radius: 5px;
      /* background: #d3d3d3; */
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
      position: relative;
      margin-bottom: 20px;
    }

    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: #1d04aa;
      cursor: pointer;
    }

    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: #04AA6D;
      cursor: pointer;
    }

    /* ------------------------------------ */
    /* Rounded sliders */


    .switch {
      position: relative;
      display: inline-block;
      width: 60px;
      height: 34px;
    }

    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    .button {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      -webkit-transition: .4s;
      transition: .4s;
      margin-bottom: 2px;
    }

    .button:before {
      position: absolute;
      content: "";
      height: 26px;
      width: 26px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      -webkit-transition: .4s;
      transition: .4s;
    }

    input:checked+.button {
      background-color: #f6f7f7;
    }

    input:focus+.button {
      box-shadow: 0 0 1px #ececec;
    }

    input:checked+.button:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }

    .button.round {
      border-radius: 34px;
    }

    .button.round:before {
      border-radius: 50%;
    }


    /* ---------------------------------- */
  </style>


</head>

<body>
  <h2>ESP Web Server</h2>


  <div class="content">
    <div class="cards">
      <!-- %BUTTONPLACEHOLDER% -->


      <div class="card card-motion">
        <h4><i class="fas fa-wind"></i> FAN</h4>
        <div class="slidecontainer">
          <input type="range" min="0" max="2" value="0" class="slider" onchange="onFanChange()" id="fan">
          <p id="speedDisplay">--</p>
        </div>
      </div>

      <div class="card card-motion">
        <h4><i class="fas fa-snowflake"></i> PUMP</h4>
        <div class="slidecontainer">
          <input type="range" min="0" max="1" value="0" class="slider" onchange="onPumpChange()" id="pump">
        </div>
      </div>
      <div class="card card-motion">
        <h4><i class="fas fa-door-open"></i> DIRECTION</h4>
        <div class="slidecontainer">
          <input type="range" min="0" max="1" value="0" class="slider" onchange="onDirectionChange()" id="direction">
        </div>
      </div>

      <div class="card card-bme">
        <h4><i class="fas fa-chart-bar"></i> TEMPERATURE</h4>
        <div>
          <p class="reading"><span id="temp"></span>&deg;C</p>
        </div>
      </div>
      <div class="card card-bme">
        <h4><i class="fas fa-chart-bar"></i> HUMIDITY</h4>
        <div>
          <p class="reading"><span id="humi"></span>&percnt;</p>
        </div>
      </div>
      <div class="card card-light">
        <h4><i class="fas fa-sun"></i> LIGHT</h4>
        <div>
          <p class="reading"><span id="light">OFF</span></p>
        </div>
      </div>

    </div>
  </div>


  <script>
    console.log("Loading page");
    var fan = document.getElementById("fan");
    var pump = document.getElementById("pump");
    var direction = document.getElementById("direction");
    var speedOutput = document.getElementById("speedDisplay");


    function onFanChange() {
      var xhr = new XMLHttpRequest();
      var url = "/output?fan=" + fan.value;
      xhr.open("GET", url, true);
      xhr.send();
      console.log("fan: " + fan.value)
      speedOutput.innerHTML = fan.value;
    }

    function onPumpChange() {
      var xhr = new XMLHttpRequest();
      var url = "/output?pump=" + pump.value;
      xhr.open("GET", url, true);
      xhr.send();
      console.log("pump: " + pump.value)

    }

    function onDirectionChange() {
      var xhr = new XMLHttpRequest();
      var url = "/output?direction=" + direction.value;
      xhr.open("GET", url, true);
      xhr.send();
      console.log("direction: " + direction.value)

    }

    if (!!window.EventSource) {

      var source = new EventSource('/events');

      source.addEventListener('open', function (e) {
        console.log("Events Connected");
      }, false);

      source.addEventListener('error', function (e) {
        if (e.target.readyState != EventSource.OPEN) {
          console.log("Events Disconnected");
        }
      }, false);

      source.addEventListener('message', function (e) {
        console.log("message", e.data);
      }, false);

      source.addEventListener('fan', function (e) {
        console.log("fan_speed", e.data);
        document.getElementById("fan").value = e.data;
        document.getElementById("speedDisplay").innerHTML = e.data;
      }, false);

      // source.addEventListener('light', function (e) {
      //   console.log("light: ", e.data);
      //   document.getElementById("light").value = e.data;

      // }, false);

      source.addEventListener('pump', function (e) {
        console.log("pump_speed: ", e.data);
        document.getElementById("pump").value = e.data;

      }, false);

      source.addEventListener('direction', function (e) {
        console.log("direction: ", e.data);
        document.getElementById("direction").value = e.data;

      }, false);

    }

    console.log("Loaded page");
  </script>
</body>
</html>

)rawliteral";