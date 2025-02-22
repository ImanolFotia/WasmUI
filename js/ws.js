var hrws = undefined;
let connect = () => {
  hrws = new WebSocket("ws://" + window.location.hostname + ":" + window.location.port);
  hrws.addEventListener("message", (event) => {
    if(event.data == "reload") window.location.reload()
  });
}
