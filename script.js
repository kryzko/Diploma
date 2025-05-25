import Paho from "paho-mqtt";

// Используйте библиотеку Paho MQTT
const clientId = "client_Krushka" ;
// Create a client instance: Broker, Port, Websocket Path, Client ID
var client = new Paho.Client("c331dfc8368c4832b294451e6869daee.s1.eu.hivemq.cloud", Number(8883), "web_clientkrushka");

// Connect the client, providing an onConnect callback
client.connect({
  userName: "krushka",  // Replace with your HiveMQ credentials
  password: "Kr5st9k2006",
  onSuccess: () => console.log("Connected"),
  onFailure: (err) => console.log("Connection failed", err),

});

// set callback handlers
client.onConnectionLost = function (responseObject) {
    console.log("Connection Lost: "+responseObject.errorMessage);
}

client.onMessageArrived = function (message) {
  console.log("Message Arrived: "+message.payloadString);
}

