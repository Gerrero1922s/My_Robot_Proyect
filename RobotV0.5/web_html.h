
#define WEBSOCKET_HTML_H
const char htmlPage[] = R"rawliteral(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
   
    <script defer>
    //Aqui va hacer lo que van ser el sistema logico del sitio wed
/*
    Esto se va encargar en mandar los o Packet of Information atraves de la red local hacia el arduino.
    nos permitira que los Botones del WedSite tenga funciones y puedan mandar Datos atraves de la red.
*/

// Variables
let connected = false;
let socket = null;
let ipBox = document.getElementById("textIP");
let puertoBox = document.getElementById("textPuerto");
const keySave = new Map();

// Ip informacion
let ipInfo = '192.168.4.1';
let puerto = "81";

function socketConexion(){
    if (connected){
        console.log("Cerrando conxion");
        closeConexion();
    }else{
        try{
            updateIP();
        }catch(e){
            console.log('Valiendo Verga');
            console.log(`Codigo De fallo al Actuliza ${e}`);
            alert("Fallo al actulizar \n Usando valores por defecto...");
        }
        
        console.log("Inciando Conexion");
        startConexion();
    }
}

function updateIP(){
    let ipValue = ipInfo;
    let puertoValue = puerto;

    if (ipValue.trim() !== "" && puertoValue.trim() !== ""){
        ipInfo = ipValue.trim();
        puerto = puertoValue.trim();
        
    }else{
        console.log("Cajas vacías. Usando valores por defecto: " + ipInfo + ":" + puerto);
    }
}
function startConexion(){
    try
    {
        socket  = new WebSocket(`ws://${ipInfo}:${puerto}`); // Es el puerto que se tiene que conectar

        socket.onopen = () => {
            console.log(`Conexion establecidad ${ipInfo}`);
            socket.send("Hello World");
            connected = true;
        }

        socket.onmessage = (event) => {
            console.log(`Mensaje Recivido: ${event.data}`);
        }

        socket.onclose = (event) => {
            if (event.wasClean) {
                alert(`Conexion se a cerrado con exito, y limpia mente. Code= ${event.code} reason= ${event.reason}`);
                connected = false;
            }else{
                alert(`Conexion colapsado o Conexcion Cerrada \n Posible falla IP Error: ${ipInfo} \n Puerto: ${puerto}`);
                connected = false;
            }
        }
    }catch(falla) {
        connected = false;
        alert(`Falla al intentar la conexion \n Codigo de falla: \n ${falla}. \n Revise: \n   IP ${ipInfo} \n   Puerto ${puerto}`);
        console.log(`Codigo de Error: ${falla}`);

    }
}
function closeConexion(){
    if (socket){
        socket.close(1000, "Prueva terminada");
    }
}
//Tengo que crear un sistema que escuche el teclado

document.addEventListener("keyup", (tecla) => keySave.delete(tecla.key.toLocaleLowerCase()));
document.addEventListener("keydown", (tecla)=>{
    keySave.set(tecla.key.toLocaleLowerCase());

    DesCode();
});


function DesCode(){
    if (keySave.has("w")) {
        if (keySave.has("a")) {
            motor(1,-1);
            return;
        }
        if(keySave.has("d")){
            motor(1,1);
            return;
        }
        motor(1);
    }
    if (keySave.has("s")){
        if (keySave.has("a")) {
            motor(-1,-1);
            return;
        }
        if(keySave.has("d")){
            motor(-1,1);
            return;
        }
        motor(-1);
    }
    if (keySave.has("d")){
        motor(0,1);
    }
    if (keySave.has("a")){
        motor(0,-199)
    }
}

//Funciones para los botones y Datos para los motores
 function motor(velocity, rotation = 0){
    if (connected){
        socket.send('V= '+ velocity + ' R= ' + rotation);
    }else{
        alert("Connection Requierment!!");
    }
 }
 </script>
    <title>Robotic Control</title>
  
<body>
    <header class="header">
        <div class="tabboard" >
            <div><h1>Sistema de comunicacion con el arduino</h1></div>
            <div>
                <input type="text" placeholder="IP Information 192.168.0.3" id="textIP" value="192.168.4.1">
                <br>
                <input type="text" placeholder="Puerto Information 81" id="textPuerto" value="81">
            </div>
            <div><div>
        </div>

    </header>
    <main>
        <br>
        <br>
        <br>
        <button onclick="socketConexion()">Iniciar Pruevas</button>
        <button class="buttonSistem" onclick="socket.send('a')">W</button>
        <button class="buttonSistem"onclick="socket.send('b')">A</button>
        <button class="buttonSistem" onclick="motor(-1)">S</button>
        <button class="buttonSistem" onclick="motor(1, -1)">D</button>
    </main>
    <footer>
        <p>
            Hola
        </p>
    </footer>
</body>
</html>
)rawliteral";