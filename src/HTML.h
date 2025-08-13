#ifndef HTML_H
  #define HTML_H
  /*


    OK, ya ready for some fun? HTML + CSS styling + javascript all in and undebuggable environment

    one trick I've learned to how to debug HTML and CSS code.

    get all your HTML code (from html to /html) and past it into this test site
    muck with the HTML and CSS code until it's what you want
    https://www.w3schools.com/html/tryit.asp?filename=tryhtml_intro

    No clue how to debug javascrip other that write, compile, upload, refresh, guess, repeat

    I'm using class designators to set styles and id's for data updating
    for example:
    the CSS class .tabledata defines with the cell will look like
    <td><div class="tabledata" id = "switch"></div></td>

    the XML code will update the data where id = "switch"
    java script then uses getElementById
    document.getElementById("switch").innerHTML="Switch is OFF";


    .. now you can have the class define the look AND the class update the content, but you will then need
    a class for every data field that must be updated, here's what that will look like
    <td><div class="switch"></div></td>

    the XML code will update the data where class = "switch"
    java script then uses getElementsByClassName
    document.getElementsByClassName("switch")[0].style.color=text_color;


    the main general sections of a web page are the following and used here

    <html>
      <style>
      // dump CSS style stuff in here
      </style>
      <body>
        <header>
        // put header code for cute banners here
        </header>
        <main>
        // the buld of your web page contents
        </main>
        <footer>
        // put cute footer (c) 2021 xyz inc type thing
        </footer>
      </body>
      <script>
      // you java code between these tags
      </script>
    </html>


  */

  // note R"KEYWORD( html page code )KEYWORD"; 
  // again I hate strings, so char is it and this method let's us write naturally
 const char index_html[] = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>3-Axis Animation</title>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
  <h2>3-Axis Sine Sequence</h2>
  <canvas id="sequenceChart" width="400" height="200"></canvas>

  <script>
    async function fetchData() {
      const response = await fetch("/sequence.json");
      const data = await response.json();

      const labels = [...Array(data.frames).keys()];
      const u = data.positions.U.map(Number);
      const v = data.positions.V.map(Number);
      const w = data.positions.W.map(Number);

      const ctx = document.getElementById('sequenceChart').getContext('2d');
      new Chart(ctx, {
        type: 'line',
        data: {
          labels: labels,
          datasets: [
            { label: 'U', data: u, borderColor: 'red', fill: false },
            { label: 'V', data: v, borderColor: 'green', fill: false },
            { label: 'W', data: w, borderColor: 'blue', fill: false }
          ]
        },
        options: {
          responsive: true,
          scales: {
            x: { display: true, title: { display: true, text: 'Frame' } },
            y: { display: true, title: { display: true, text: 'Value' } }
          }
        }
      });
    }

    fetchData();
  </script>
</body>
</html>
)rawliteral";



  const char PAGE_MAIN[]  = R"=====(
<!DOCTYPE html>
<html lang="es">
<head>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>Control Escultura</title>
<style>
  body {
    font-family: Arial, sans-serif;
    background: #f5fbfc;
    margin: 0;
    padding: 20px;
    color: #00333d;
  }
  header {
    display: flex;
    align-items: center;
    gap: 15px;
  }
  header img {
    width: 60px;
    height: auto;
  }
  header h1 {
    color: #009CA3;
    margin: 0;
  }
  .main-container {
    display: flex;
    gap: 20px;
    margin-top: 30px;
  }
  .container {
    flex: 1;
    max-width: 500px;
  }
  .category {
    font-weight: bold;
    font-size: 24px;
    color: #009CA3;
    margin-bottom: 15px;
    text-align: center;
  }
  form {
    background-color: #E0F7F8;
    padding: 15px;
    border-radius: 10px;
    border: 2px solid #3CA1D5;
  }
  label.bodytext {
    display: block;
    margin-bottom: 5px;
    color: #009CA3;
    font-weight: 600;
    font-size: 18px;
  }
  .field-group {
    margin-bottom: 15px;
  }
  .part-labels {
    display: flex;
    justify-content: space-between;
    margin-bottom: 5px;
  }
  .part-labels span {
    color: #3CA1D5;
    font-weight: 600;
    width: 30%;
    margin-right: 8%;
    margin-left: 2%;
    text-align: center;
  }
  input[type="number"] {
    width: 28%;
    font-size: 20px;
    margin-right: 2%;
    margin-left: 2%;
    border: 2px solid #3CA1D5;
    border-radius: 5px;
    padding: 5px;
    color: #00333d;
    box-sizing: border-box;
  }
  input[type="number"]:nth-child(3n) {
    margin-right: 0;
  }
  button.btn {
    background-color: #009CA3;
    border: none;
    color: #fff;
    padding: 10px 20px;
    font-size: 18px;
    border-radius: 8px;
    cursor: pointer;
    transition: background-color 0.3s ease;
    margin-top: 10px;
    display: block;
    width: 100%;
  }
  button.btn:hover {
    background-color: #007f85;
  }
  .chart-container {
    flex: 1;
    background: #fff;
    padding: 15px;
    border-radius: 10px;
    border: 2px solid #3CA1D5;
  }
  .logo {
  width: 120px;   /* ancho fijo */
  height: auto;   /* mantiene proporción */
}
.layout {
  display: flex;
  flex-wrap: wrap;
  gap: 20px;
}

.form-section {
  flex: 1 1 300px;
}

.chart-section {
  flex: 2 1 300px;
}

.chart-section canvas {
  width: 100% !important;
  height: 600px !important; /* o la altura que quieras */
}
</style>
</head>
<body>

<header>
  <!-- Logo en Base64 -->
  <h1>Control Escultura</h1>
</header>

<div class="layout">
<div class="form-section">
  <form id="sequenceForm" onsubmit="submitSequenceForm(event)">
    <div class="field-group">
      <label for="period" class="bodytext">Periodo (s):</label>
      <input type="number" id="period" name="period" min="1" max=60" value="7" step="0.1" required />
    </div>

    <div class="field-group">
      <label class="bodytext">Amplitudes:</label>
      <div class="part-labels">
        <span>Cola</span>
        <span>Cuerpo</span>
        <span>Cabeza</span>
      </div>
      <input type="number" id="amp1" name="amp1" step="any" placeholder="Amplitud Cola" required />
      <input type="number" id="amp2" name="amp2" step="any" placeholder="Amplitud Cuerpo" required />
      <input type="number" id="amp3" name="amp3" step="any" placeholder="Amplitud Cabeza" required />
    </div>

    <div class="field-group">
      <label class="bodytext">Fases:</label>
      <div class="part-labels">
        <span>Cola</span>
        <span>Cuerpo</span>
        <span>Cabeza</span>
      </div>
      <input type="number" id="fase1" name="Fase1" step="any" placeholder="Fase Cola" required />
      <input type="number" id="fase2" name="Fase2" step="any" placeholder="Fase Cuerpo" required />
      <input type="number" id="fase3" name="Fase3" step="any" placeholder="Fase Cabeza" required />
    </div>
    
    <div class="field-group">
      <label class="bodytext">Offsets:</label>
      <div class="part-labels">
        <span>Cola</span>
        <span>Cuerpo</span>
        <span>Cabeza</span>
      </div>
      <input type="number" id="offset1" name="offset1" step="any" placeholder="Offset Cola" required />
      <input type="number" id="offset2" name="offset2" step="any" placeholder="Offset Cuerpo" required />
      <input type="number" id="offset3" name="offset3" step="any" placeholder="Offset Cabeza" required />
    </div>

    <button type="submit" class="btn">Enviar</button>
    <button type="button" class="btn" style="background-color: #3CA1D5; margin-top: 8px;" onclick="resetToFactory()">Valores de fábrica</button>
  </form>
    
  <div id="responseMessage"></div>
  <br><br>
  <img class="logo" src="data:image/svg+xml;base64,PD94bWwgdmVyc2lvbj0iMS4wIiBlbmNvZGluZz0iVVRGLTgiIHN0YW5kYWxvbmU9Im5vIj8+CjxzdmcKICAgd2lkdGg9IjEwMCUiCiAgIGhlaWdodD0iMTAwJSIKICAgdmlld0JveD0iMCAwIDQ4NjUgMTU0NyIKICAgdmVyc2lvbj0iMS4xIgogICB4bWw6c3BhY2U9InByZXNlcnZlIgogICBzdHlsZT0iZmlsbC1ydWxlOmV2ZW5vZGQ7Y2xpcC1ydWxlOmV2ZW5vZGQ7c3Ryb2tlLWxpbmVqb2luOnJvdW5kO3N0cm9rZS1taXRlcmxpbWl0OjI7IgogICBpZD0ic3ZnMTQiCiAgIHNvZGlwb2RpOmRvY25hbWU9InplZXRhX2xvZ29fY29sb3Iuc3ZnIgogICBpbmtzY2FwZTp2ZXJzaW9uPSIxLjIuMiAoNzMyYTAxZGE2MywgMjAyMi0xMi0wOSkiCiAgIHhtbG5zOmlua3NjYXBlPSJodHRwOi8vd3d3Lmlua3NjYXBlLm9yZy9uYW1lc3BhY2VzL2lua3NjYXBlIgogICB4bWxuczpzb2RpcG9kaT0iaHR0cDovL3NvZGlwb2RpLnNvdXJjZWZvcmdlLm5ldC9EVEQvc29kaXBvZGktMC5kdGQiCiAgIHhtbG5zPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIKICAgeG1sbnM6c3ZnPSJodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2ZyIKICAgeG1sbnM6c2VyaWY9Imh0dHA6Ly93d3cuc2VyaWYuY29tLyI+PGRlZnMKICAgICBpZD0iZGVmczE4IiAvPjxzb2RpcG9kaTpuYW1lZHZpZXcKICAgICBpZD0ibmFtZWR2aWV3MTYiCiAgICAgcGFnZWNvbG9yPSIjZmZmZmZmIgogICAgIGJvcmRlcmNvbG9yPSIjMDAwMDAwIgogICAgIGJvcmRlcm9wYWNpdHk9IjAuMjUiCiAgICAgaW5rc2NhcGU6c2hvd3BhZ2VzaGFkb3c9IjIiCiAgICAgaW5rc2NhcGU6cGFnZW9wYWNpdHk9IjAuMCIKICAgICBpbmtzY2FwZTpwYWdlY2hlY2tlcmJvYXJkPSIwIgogICAgIGlua3NjYXBlOmRlc2tjb2xvcj0iI2QxZDFkMSIKICAgICBzaG93Z3JpZD0iZmFsc2UiCiAgICAgaW5rc2NhcGU6em9vbT0iMC4xMTUwMDUxNCIKICAgICBpbmtzY2FwZTpjeD0iMTM0Ny43NjU5IgogICAgIGlua3NjYXBlOmN5PSI5NzMuODY5NTMiCiAgICAgaW5rc2NhcGU6d2luZG93LXdpZHRoPSIxOTIwIgogICAgIGlua3NjYXBlOndpbmRvdy1oZWlnaHQ9IjEwMDkiCiAgICAgaW5rc2NhcGU6d2luZG93LXg9Ii04IgogICAgIGlua3NjYXBlOndpbmRvdy15PSItOCIKICAgICBpbmtzY2FwZTp3aW5kb3ctbWF4aW1pemVkPSIxIgogICAgIGlua3NjYXBlOmN1cnJlbnQtbGF5ZXI9ImcxMCIgLz48cmVjdAogICAgIGlkPSJOZWdyby1zb2JyZS1ibGFuY28iCiAgICAgc2VyaWY6aWQ9Ik5lZ3JvIHNvYnJlIGJsYW5jbyIKICAgICB4PSIwIgogICAgIHk9IjAiCiAgICAgd2lkdGg9IjQ4NjQuOTYiCiAgICAgaGVpZ2h0PSIxNTQ2LjE5IgogICAgIHN0eWxlPSJmaWxsOm5vbmU7IiAvPjxjbGlwUGF0aAogICAgIGlkPSJfY2xpcDEiPjxyZWN0CiAgICAgICB4PSIwIgogICAgICAgeT0iMCIKICAgICAgIHdpZHRoPSI0ODY0Ljk2IgogICAgICAgaGVpZ2h0PSIxNTQ2LjE5IgogICAgICAgaWQ9InJlY3QzIiAvPjwvY2xpcFBhdGg+PGcKICAgICBjbGlwLXBhdGg9InVybCgjX2NsaXAxKSIKICAgICBpZD0iZzEyIj48ZwogICAgICAgaWQ9ImcxMCI+PHBhdGgKICAgICAgICAgZD0iTTMzOTIuMjUsMTQ3Ljk1NWwtMTk4Ny4xMSwwYy00MC40OTUsMCAtNzMuMzI0LC0zMy4wMTcgLTczLjMyNCwtNzMuNzRjMCwtNDAuNzQgMzIuODI5LC03My43NTggNzMuMzI0LC03My43NThsMjUwOC4zMywwYzQwLjQ5LDAgNzMuMzEsMzMuMDE4IDczLjMxLDczLjc1OGMwLDQwLjcyMyAtMzIuODIsNzMuNzQgLTczLjMxLDczLjc0bC0zNzMuNzE1LDBsMCw5NjEuMTU0YzAsMzkuMzk2IC0zMy4wMDgsNzEuMzMxIC03My43NDUsNzEuMzMxYy00MC43NCwwIC03My43NTMsLTMxLjkzNSAtNzMuNzUzLC03MS4zMzFsMCwtOTYxLjE1NFptNTEyLjc2MSwxMDMyLjQ4Yy05LjAwMiwwIC0xOC4xNDIsLTEuNjIyIC0yNy4wNDUsLTUuMDgyYy0zOC41MDYsLTE0Ljk0MiAtNTcuNjEsLTU4LjI4OCAtNDIuNjcyLC05Ni43ODVsNDUzLjc1LC0xMDMzLjI1YzEwLjk0NiwtMjguMjIxIDM2LjY4NywtNDQuMjU4IDY2Ljk1MSwtNDQuOTMzYzMwLjUwNSwtMC43MTUgNTUuMjIyLDE3LjA2MSA2Ny4zODksNDQuNzVsNDM1LjI1LDEwMzAuNDFjMTYuNjA1LDM3Ljg0NSAtMC41OTQsODEuOTU1IC0zOC40MTIsOTguNTZjLTM3LjgyMiwxNi42MjMgLTgxLjkzNywtMC41ODUgLTk4LjU1NSwtMzguNDAzbC0zNjYuNjY5LC04OTkuMDEzbC0xNjguMDA2LDQwNS41NjVsLTIxMi4yMzIsNDkwLjQzNWMtMTEuNDk2LDI5LjYyMSAtMzkuNzgsNDcuNzQ1IC02OS43NDksNDcuNzQ1Wm0tMzgzMS4xLC0xMDMyLjQ4Yy0yOS41MDUsMCAtNzAuMzY1LC0yOS41MDQgLTcwLjM2NSwtNzMuNzQ4YzAsLTQ0LjI1IDQwLjg2MSwtNzMuNzQ5IDcwLjM2NSwtNzMuNzQ5bDg4OC4zNywtMS4xMDljNDYuODI0LDAgOTIuNzE0LDU5LjQ1MSA2OS4wMDcsMTE2LjI5MWMtMi42MTksNi4yODQgLTE2LjAxOSwyNi41NjMgLTIwLjU0MiwzMS42NTRsLTc3MC40MTcsODg1LjY0OGwyNzkxLjE0LC0wLjY2MWM4OC40OTksMCA4OC40OTksMTQ3LjQ5OCAwLDE0Ny40OThsLTI5NTcuNTUsMC42NjFjLTQ3LjAzNCwwIC04NS40NjQsLTQ0LjYxNiAtNzAuNzE0LC0xMDMuNjE5YzEuNjU4LC02LjYzOCA3LjE2OSwtMjQuMDI0IDExLjcxNSwtMjkuMTMzbDc5OS44NzIsLTg5OS43MzNsLTc0MC44NzMsMFptMTY4NS41Myw1MjAuNzY5Yy0yOC4zLDc5LjY4MyAtMTAxLjM5OSwxMzguODA5IC0xODguNDU1LDE0Ni41NzRjLTEzNy44NTMsMTIuMjgyIC0yNTIuODQ3LC0xMDIuNzI2IC0yNDAuNTM4LC0yNDAuNTQzYzkuNDQ0LC0xMDUuNjQ0IDk0LjUyOCwtMTkwLjcxIDIwMC4xNzMsLTIwMC4xMjdjMTA0LjcwMiwtOS4zMjQgMTk2LjIyNyw1NC44MDggMjI4Ljc4LDE0Ni41OThsOTc0LjIxMSwwYzM5LjE2MywwIDcwLjkxMSwzMy4wMjYgNzAuOTExLDczLjc0OWMwLDQwLjc0MSAtMzEuNzQ4LDczLjc0OSAtNzAuOTExLDczLjc0OWwtOTc0LjE3MSwwWm0tMjA4LjMwMiwtMTUwLjg4OWM0MC42MzQsMCA3My42MjMsMzIuOTg5IDczLjYyMyw3My42MjNjMCw0MC42MzMgLTMyLjk4OSw3My42MjMgLTczLjYyMyw3My42MjNjLTQwLjYzMywwIC03My42MjMsLTMyLjk5IC03My42MjMsLTczLjYyM2MwLC00MC42MzQgMzIuOTksLTczLjYyMyA3My42MjMsLTczLjYyM1oiCiAgICAgICAgIGlkPSJwYXRoNiIKICAgICAgICAgc3R5bGU9ImZpbGw6IzAwOWNhMztmaWxsLW9wYWNpdHk6MSIgLz48cGF0aAogICAgICAgICBkPSJNMTQxNi41MywxNTQ2Ljg0bC0yOC40OTQsLTI4LjQ5NGMtMTguOTk2LDE4LjM4OCAtNDIuODk1LDI3LjU3OCAtNzEuNjg5LDI3LjU3OGMtMTQuMjk4LDAgLTI3LjE3NSwtMi4zNTYgLTM4LjYwNCwtNy4wNDVjLTExLjQ0MiwtNC42OTcgLTIwLjM2OCwtMTEuMjg1IC0yNi44MDQsLTE5Ljc2NGMtNi40MzcsLTguNDc1IC05LjY1OSwtMTguMTIgLTkuNjU5LC0yOC45NTVjMCwtMTMuMDY5IDQuMDM2LC0yNC42NTQgMTIuMTA4LC0zNC43NzNjOC4wNjgsLTEwLjEwNiAyMS4zOTYsLTIwLjQ3MSAzOS45ODEsLTMxLjA5NWMtOS40LC05LjM5MSAtMTYuMDM3LC0xNy44NjEgLTE5LjkxMiwtMjUuNDI4Yy0zLjg4LC03LjU1NCAtNS44MjQsLTE1LjQxNiAtNS44MjQsLTIzLjU4NmMwLC0xNC41MDQgNS4yNTYsLTI2LjEzOSAxNS43NzgsLTM0LjkyMmMxMC41MTcsLTguNzgzIDI0LjU1NiwtMTMuMTgxIDQyLjEyNiwtMTMuMTgxYzE2LjMzNiwwIDI5LjMwNywzLjk4NyAzOC45MDMsMTEuOTUyYzkuNjA2LDcuOTYxIDE0LjQwNiwxOC44ODkgMTQuNDA2LDMyLjc4YzAsMTAuODMgLTMuNTI2LDIwLjUyOSAtMTAuNTcxLDI5LjEwMmMtNy4wNDgsOC41ODIgLTE4Ljc0NSwxNy41NzUgLTM1LjA4MiwyNi45NjFsNTMuMDAxLDUyLjY5M2M2LjEyOCwtMTEuNjM5IDEwLjUxNywtMjQuNjEgMTMuMTc3LC0zOC45bDIzLjU5LDcuNjUyYy0zLjY3OCwxOC44IC05LjkxMywzNS4xMzIgLTE4LjY4Nyw0OS4wMTlsMjguNzkzLDI4Ljc5M2wtMTYuNTM3LDE5LjYxM1ptNTk0Ljc1MSwtMTEuNjYyYy0xNS4wMTMsLTUuMDAxIC0yNi44MDksLTExLjQ4NyAtMzUuMzkxLC0xOS40NjFsMTEuMzQsLTIzLjg5YzguMTY2LDcuMzQ4IDE4LjYzOCwxMy4zMiAzMS40MDQsMTcuOTIzYzEyLjc2LDQuNTkxIDI1Ljg4OCw2Ljg4OCAzOS4zNjQsNi44ODhjMTcuNzcxLDAgMzEuMDQxLC0zLjAwMyAzOS44MjQsLTkuMDI5YzguNzgzLC02LjAzNCAxMy4xODEsLTE0LjA0OCAxMy4xODEsLTI0LjA2YzAsLTcuMzQ4IC0yLjQwOSwtMTMuMzE5IC03LjIwNSwtMTcuOTIzYy00LjgsLTQuNTkgLTEwLjcxOCwtOC4xMDggLTE3Ljc2NywtMTAuNTdjLTcuMDUzLC0yLjQ1NCAtMTcuMDAyLC01LjIwMyAtMjkuODcsLTguMjY1Yy0xNi4xMzEsLTMuODc5IC0yOS4xNTYsLTcuNzY0IC0zOS4wNjUsLTExLjY0OGMtOS45MDksLTMuODc1IC0xOC4zNzksLTkuODQ2IC0yNS40MjgsLTE3LjkyM2MtNy4wMzksLTguMDYzIC0xMC41NjYsLTE4LjkzMyAtMTAuNTY2LC0zMi42MTljMCwtMTEuNDM4IDMuMDEzLC0yMS43NTggOS4wNDIsLTMwLjkzOWM2LjAxNiwtOS4xOTQgMTUuMTY2LC0xNi41NTEgMjcuNDEyLC0yMi4wNjJjMTIuMjU2LC01LjUxNiAyNy40NjYsLTguMjczIDQ1LjY0OSwtOC4yNzNjMTIuNjYyLDAgMjUuMTE5LDEuNjQgMzcuMzc1LDQuOTAzYzEyLjI1NSwzLjI2NyAyMi43NzMsNy45NiAzMS41NTUsMTQuMDkzbC0xMC4xMDUsMjQuNTAyYy04Ljk4OSwtNS43MDggLTE4LjU5NCwtMTAuMDU3IC0yOC44MDMsLTEzLjAxNmMtMTAuMjEzLC0yLjk1OCAtMjAuMjExLC00LjQ0MiAtMzAuMDIyLC00LjQ0MmMtMTcuMzY1LDAgLTMwLjM4NSwzLjE2OSAtMzkuMDU2LDkuNDk4Yy04LjY4NCw2LjMyOSAtMTMuMDI0LDE0LjUwOCAtMTMuMDI0LDI0LjUwMmMwLDcuMzYyIDIuNDUzLDEzLjMzMyA3LjM1MiwxNy45MjNjNC45MDMsNC42IDEwLjk3OCw4LjE4IDE4LjIyNywxMC43MjdjNy4yNTQsMi41NjIgMTcuMTEsNS4yNjYgMjkuNTY3LDguMTIyYzE2LjEzMSwzLjg3NSAyOS4xMDIsNy43NjggMzguOTA4LDExLjYzNGM5LjgwMiwzLjg4NCAxOC4yMjcsOS44MTYgMjUuMjgsMTcuNzc2YzcuMDM1LDcuOTY5IDEwLjU2MiwxOC42NzkgMTAuNTYyLDMyLjE2OGMwLDExLjIzNyAtMy4wNjIsMjEuNDk0IC05LjE5LDMwLjc5MWMtNi4xMjMsOS4yOTcgLTE1LjQyLDE2LjYzNiAtMjcuODgxLDIyLjA0OWMtMTIuNDYxLDUuNDEzIC0yNy43NzQsOC4xMjEgLTQ1Ljk1Miw4LjEyMWMtMTYuMTM2LDAgLTMxLjcxMiwtMi40OTggLTQ2LjcxNywtNy41Wm0tMTgwOS45Miw2Ljg0NWwtMC4zMDQsLTE1Ni4yNDVsLTc3LjUxMywxMzAuMjA0bC0xNC4wODgsMGwtNzcuNTEyLC0xMjkuMjc5bDAsMTU1LjMybC0yOS40MSwwbDAsLTIxNC40NTNsMjUuMTE5LDBsODkuNDU1LDE1MC43MjVsODguMjM1LC0xNTAuNzI1bDI1LjEyLDBsMC4zMDgsMjE0LjQ1M2wtMjkuNDEsMFptMjIyMy4zMiwtMTExLjIwOWwyOS40MSwwbDAsODMuNjRjLTEwLjYyNCw4Ljc4MyAtMjIuOTc4LDE1LjUyOCAtMzcuMDcxLDIwLjIyNWMtMTQuMDkzLDQuNjk4IC0yOC43OTMsNy4wNDQgLTQ0LjExNSw3LjA0NGMtMjEuNjUxLDAgLTQxLjE1MiwtNC43NTUgLTU4LjUxNywtMTQuMjUzYy0xNy4zNiwtOS40ODkgLTMwLjk4NywtMjIuNTU4IC00MC44OTcsLTM5LjIwM2MtOS45MDksLTE2LjY1IC0xNC44NTIsLTM1LjM5MSAtMTQuODUyLC01Ni4yMjRjMCwtMjAuODI4IDQuOTQzLC0zOS42MjMgMTQuODUyLC01Ni4zNjZjOS45MSwtMTYuNzQ0IDIzLjU5MSwtMjkuODIyIDQxLjA0OSwtMzkuMjIxYzE3LjQ3MiwtOS4zODcgMzcuMTIxLC0xNC4wOTMgNTguOTc3LC0xNC4wOTNjMTcuMTY0LDAgMzIuNzM2LDIuODExIDQ2LjcyMSw4LjQyNWMxMy45OTUsNS42MjMgMjUuODkzLDEzLjg0NyAzNS42OSwyNC42NjRsLTE4Ljk5NiwxOC45OTZjLTE3LjE1NCwtMTYuNTM4IC0zNy44OCwtMjQuODIgLTYyLjE4NiwtMjQuODJjLTE2LjM1LDAgLTMwLjk5MiwzLjUyNiAtNDMuOTYzLDEwLjU3Yy0xMi45NzEsNy4wNTMgLTIzLjEzNSwxNi44NTEgLTMwLjQ4MywyOS40MTFjLTcuMzUyLDEyLjU2NCAtMTEuMDMxLDI2LjcwNiAtMTEuMDMxLDQyLjQzNGMwLDE1LjUyMyAzLjY3OSwyOS41NjIgMTEuMDMxLDQyLjEyMmM3LjM0OCwxMi41NjQgMTcuNTEyLDIyLjQyIDMwLjQ4MywyOS41NjdjMTIuOTcxLDcuMTUxIDI3LjUyLDEwLjcyMiA0My42NTksMTAuNzIyYzE5LjE4OSwwIDM1Ljk0MSwtNC41OSA1MC4yMzksLTEzLjc4NGwwLC02OS44NTZabS0xOTkxLjM5LDU1Ljc5OWwtMTEzLjk2NywwbC0yMy41ODYsNTMuNjIybC0zMS41NiwwbDk3LjEyLC0yMTQuNDU3bDMwLjMyNywwbDk3LjQyNCwyMTQuNDU3bC0zMi4xNjcsMGwtMjMuNTkxLC01My42MjJabTQ2Mi4xMzMsMjYuOTY4bDAsMjYuNjUybC0xNTUuNjI4LDBsMCwtMjE0LjQ1N2wxNTEuMzQxLDBsMCwyNi42NTJsLTEyMC43MTEsMGwwLDY1Ljg3OGwxMDcuNTM5LDBsMCwyNi4wMzFsLTEwNy41MzksMGwwLDY5LjI0NGwxMjQuOTk4LDBabTEwNDQuMjYsMGwwLDI2LjY1MmwtMTU1LjYyOCwwbDAsLTIxNC40NTdsMTUxLjM0MSwwbDAsMjYuNjUybC0xMjAuNzExLDBsMCw2NS44NzhsMTA3LjUzOSwwbDAsMjYuMDMxbC0xMDcuNTM5LDBsMCw2OS4yNDRsMTI0Ljk5OCwwWm05NDMuOTg3LDBsMCwyNi42NTJsLTE1NS42MjgsMGwwLC0yMTQuNDU3bDE1MS4zNDEsMGwwLDI2LjY1MmwtMTIwLjcwNiwwbDAsNjUuODc4bDEwNy41MywwbDAsMjYuMDMxbC0xMDcuNTMsMGwwLDY5LjI0NGwxMjQuOTkzLDBabS0xMzQ3LjM5LC0xODcuODA1bDkwLjM4NSwwYzIyLjY2NSwwIDQyLjc3OSw0LjQ5NiA2MC4zNTMsMTMuNDhjMTcuNTYxLDguOTkzIDMxLjE5NCwyMS42MDcgNDAuODk3LDM3LjgzNmM5LjY5LDE2LjIzOCAxNC41NTMsMzQuODc2IDE0LjU1Myw1NS45MTVjMCwyMS4wMzggLTQuODYzLDM5LjY2OCAtMTQuNTUzLDU1LjkwMmMtOS43MDMsMTYuMjM4IC0yMy4zMzYsMjguODU1IC00MC44OTcsMzcuODM1Yy0xNy41NzQsOS4wMDIgLTM3LjY4OCwxMy40ODkgLTYwLjM1MywxMy40ODlsLTkwLjM4NSwwbDAsLTIxNC40NTdabTY4My42NTIsMC4wMDRsLTMwLjYzNSwwbDAsMjE0LjQ1MmwzMC42MzUsMGwwLC0yMTQuNDUyWm04NDYuNDQsMjE0LjQ1MmwtNDYuMjcsLTY1Ljg3NGMtNS43MTYsMC40MTEgLTEwLjIwOCwwLjYyMSAtMTMuNDcxLDAuNjIxbC01My4wMDYsMGwwLDY1LjI1M2wtMzAuNjMsMGwwLC0yMTQuNDUzbDgzLjYzNiwwYzI3Ljc2NSwwIDQ5LjYyNiw2LjY0MSA2NS41NiwxOS45MTJjMTUuOTI2LDEzLjI3OSAyMy44OSwzMS41NTUgMjMuODksNTQuODM4YzAsMTYuNTQ2IC00LjA4NSwzMC42MzUgLTEyLjI1MSw0Mi4yNzhjLTguMTc1LDExLjYzOSAtMTkuODE0LDIwLjEyMiAtMzQuOTIxLDI1LjQyM2w1MC44NTEsNzIuMDAybC0zMy4zODgsMFptLTE5OTEuNDUsMGwtNDYuMjY1LC02NS44NzRjLTUuNzIxLDAuNDExIC0xMC4yMDksMC42MjEgLTEzLjQ3MiwwLjYyMWwtNTMuMDA5LDBsMCw2NS4yNTNsLTMwLjYzMSwwbDAsLTIxNC40NTNsODMuNjQsMGMyNy43NjEsMCA0OS42MjIsNi42NDEgNjUuNTU3LDE5LjkxMmMxNS45MjUsMTMuMjc5IDIzLjg5NCwzMS41NTUgMjMuODk0LDU0LjgzOGMwLDE2LjU0NiAtNC4wOSwzMC42MzUgLTEyLjI1Niw0Mi4yNzhjLTguMTcsMTEuNjM5IC0xOS44MTMsMjAuMTIyIC0zNC45MjEsMjUuNDIzbDUwLjg1Niw3Mi4wMDJsLTMzLjM5MywwWm0tNDg2LjUyNywtOTUuMjg1bC0zOS4yMDcsMzkuODM0bDAsNTUuNDVsLTMwLjY0LDBsMCwtMjE0LjQ1M2wzMC42NCwwbDAsMTE5Ljc5MWwxMTYuNDExLC0xMTkuNzkxbDM0LjkzLDBsLTkxLjYsOTYuNWw5Ny40MTUsMTE3Ljk1M2wtMzUuODQyLDBsLTgyLjEwNywtOTUuMjg0Wm0yMDk2LjIxLC0xMTkuMTY5bDAsMjE0LjQ1M2wtMjUuMTE5LDBsLTEyOC42NjgsLTE1OS45MTlsMCwxNTkuOTE5bC0zMC42NDQsMGwwLC0yMTQuNDUzbDI1LjEyNCwwbDEyOC42NzIsMTU5LjkxOWwwLC0xNTkuOTE5bDMwLjYzNSwwWm0tMTMxNC4wMywxNzUuNDE0bC02MC45NjEsLTYwLjY2MmMtMTQuNzA1LDguMTcgLTI0Ljk2NywxNS43ODIgLTMwLjc5MSwyMi44MjJjLTUuODIsNy4wNTMgLTguNzI5LDE0Ljk2OCAtOC43MjksMjMuNzUxYzAsMTAuMjEzIDQuNDM4LDE4LjQ4MiAxMy4zMjQsMjQuODExYzguODg1LDYuMzI5IDIwLjQ3LDkuNDk4IDM0Ljc3Myw5LjQ5OGMyMS4yNCwwIDM4LjcwMywtNi43NCA1Mi4zODQsLTIwLjIyWm0yNTQuMjUxLDEyLjM4OWMxNy4zNiwwIDMyLjYyOSwtMy4zNyA0NS44MDUsLTEwLjExOWMxMy4xNjgsLTYuNzM2IDIzLjMyNywtMTYuMTg1IDMwLjQ3OCwtMjguMzM4YzcuMTUyLC0xMi4xMzkgMTAuNzIzLC0yNi4xODcgMTAuNzIzLC00Mi4xMTdjMCwtMTUuOTM0IC0zLjU3MSwtMjkuOTc4IC0xMC43MjMsLTQyLjEzMWMtNy4xNTEsLTEyLjE0NCAtMTcuMzEsLTIxLjU5NyAtMzAuNDc4LC0yOC4zMzdjLTEzLjE3NiwtNi43NDEgLTI4LjQ0NSwtMTAuMTExIC00NS44MDUsLTEwLjExMWwtNTcuOTA0LDBsMCwxNjEuMTUzbDU3LjkwNCwwWm0tMTIwMi4yNSwtNTEuNDcxbC00Ni4yNjYsLTEwNS4wOWwtNDYuMjYsMTA1LjA5bDkyLjUyNiwwWm02MzYuMTEsLTI1LjczNWMxMC4yMDQsLTguMzggMTUuMzEzLC0yMC4zMjMgMTUuMzEzLC0zNS44NDZjMCwtMTUuNTE5IC01LjEwOSwtMjcuNDE3IC0xNS4zMTMsLTM1LjY4NmMtMTAuMjEzLC04LjI3MyAtMjUuMDI2LC0xMi40MTIgLTQ0LjQxOSwtMTIuNDEybC01Mi4wODksMGwwLDk2LjUwOGw1Mi4wODksMGMxOS4zOTMsMCAzNC4yMDYsLTQuMTgzIDQ0LjQxOSwtMTIuNTY0Wm0xOTkxLjQ0LDBjMTAuMjA5LC04LjM4IDE1LjMyMiwtMjAuMzIzIDE1LjMyMiwtMzUuODQ2YzAsLTE1LjUxOSAtNS4xMTMsLTI3LjQxNyAtMTUuMzIyLC0zNS42ODZjLTEwLjIwOCwtOC4yNzMgLTI1LjAyMSwtMTIuNDEyIC00NC40MTksLTEyLjQxMmwtNTIuMDg1LDBsMCw5Ni41MDhsNTIuMDg1LDBjMTkuMzk4LDAgMzQuMjExLC00LjE4MyA0NC40MTksLTEyLjU2NFptLTE3NDcuMTEsLTgwLjU0OWMtNS40MTMsNC44IC04LjExNywxMS4wOCAtOC4xMTcsMTguODM5YzAsNS41MTYgMS40NzUsMTAuODI2IDQuNDQzLDE1LjkzYzIuOTU5LDUuMTA5IDguNzI5LDEyLjA1IDE3LjMwNiwyMC44MjhjMTQuMDkzLC03Ljk2IDIzLjg0MSwtMTQuODAzIDI5LjI1OCwtMjAuNTJjNS40MDksLTUuNzIxIDguMTIyLC0xMS45NTEgOC4xMjIsLTE4LjY5MmMwLC03LjE0MiAtMi40OTksLTEyLjg2OCAtNy41MTQsLTE3LjE1NGMtNS4wMDEsLTQuMjg2IC0xMS45OTYsLTYuNDMyIC0yMC45OCwtNi40MzJjLTkuNjAxLDAgLTE3LjExLDIuNCAtMjIuNTE4LDcuMjAxWiIKICAgICAgICAgc3R5bGU9ImZpbGwtcnVsZTpub256ZXJvO2ZpbGw6IzAwOWNhMztmaWxsLW9wYWNpdHk6MSIKICAgICAgICAgaWQ9InBhdGg4IiAvPjwvZz48L2c+PC9zdmc+Cg==" alt="logo">

  </div>

  <!-- Gráfico -->
  <div class="chart-section">
  <canvas id="sequenceChart"></canvas>
</div>

<script>
    // Valores por defecto de fábrica
  const factoryDefaults = {
    period: 16,
    amp1: 20,
    amp2: 30,
    amp3: 9,
    fase1: 180,
    fase2: 0,
    fase3: 180,
    offset1: 0,
    offset2: 4,
    offset3: 0,
  };

  // Al cargar la página, poner valores por defecto
  window.onload = () => {
    resetToFactory();
  };

  async function submitSequenceForm(event) {
    event.preventDefault();

    const form = event.target;

    // Recoger todos los valores actuales
    const currentValues = {
      period: Number(form.period.value),
      amp1: Number(form.amp1.value),
      amp2: Number(form.amp2.value),
      amp3: Number(form.amp3.value),
      offset1: Number(form.offset1.value),
      offset2: Number(form.offset2.value),
      offset3: Number(form.offset3.value),
      fase1: Number(form.fase1.value),
      fase2: Number(form.fase2.value),
      fase3: Number(form.fase3.value),
    };

    // Enviar todos los valores sin filtrar
  const modifiedValues = { ...currentValues };

  // Mostrar mensaje de envío
  const responseDiv = document.getElementById('responseMessage');
  responseDiv.textContent = '⏳ Enviando datos...';

    try {
      const response = await fetch('/updateSequence', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(modifiedValues),
      });

      if (!response.ok) {
        responseDiv.textContent = `❌ Error HTTP: ${response.status}`;
        return;
      }

      const jsonResp = await response.json();
      responseDiv.textContent = '✅ Respuesta: ' + JSON.stringify(jsonResp);
    } catch (error) {
      responseDiv.textContent = '❌ Error enviando datos: ' + error.message;
    }
  }

  function resetToFactory() {
    const form = document.getElementById('sequenceForm');
    for (const key in factoryDefaults) {
      if (form[key]) {
        form[key].value = factoryDefaults[key];
      }
    }
    // Limpiar mensaje respuesta al resetear
    const responseDiv = document.getElementById('responseMessage');
    responseDiv.textContent = '';
  }


  let sequenceChart; // variable global para el gráfico

async function fetchData() {
  try {
    const response = await fetch("/sequence.json");
    if (!response.ok) throw new Error("No se pudo cargar sequence.json");
    const data = await response.json();

    const labels = [...Array(data.frames).keys()];
    const u = data.positions.U.map(Number);
    const v = data.positions.V.map(Number);
    const w = data.positions.W.map(Number);

    if (!sequenceChart) {
      // Crear gráfico por primera vez
      const ctx = document.getElementById('sequenceChart').getContext('2d');
      sequenceChart = new Chart(ctx, {
        type: 'line',
        data: {
          labels: labels,
          datasets: [
            { label: 'Cola (U)', data: u, borderColor: '#009CA3', fill: false },
            { label: 'Cuerpo (V)', data: v, borderColor: '#3CA1D5', fill: false },
            { label: 'Cabeza (W)', data: w, borderColor: '#FF5733', fill: false }
          ]
        },
        options: {
          responsive: true,
          scales: {
            x: { display: true, title: { display: true, text: 'Frame' } },
            y: { display: true, title: { display: true, text: 'Valor' } }
          }
        }
      });
    } else {
      // Actualizar datos del gráfico existente
      sequenceChart.data.labels = labels;
      sequenceChart.data.datasets[0].data = u;
      sequenceChart.data.datasets[1].data = v;
      sequenceChart.data.datasets[2].data = w;
      sequenceChart.update();
    }
  } catch (err) {
    console.error("Error cargando datos del gráfico:", err);
  }
}
 
// Llamada inicial
fetchData();

// Actualización cada segundo
setInterval(fetchData, 1000);



</script>

</div>
</html>

  )=====";


#endif