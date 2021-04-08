(function () {
    var host = location.hostname;
    let webSocket;

    function init()
    {
        checkIfLocalFile();
        initWebSocket();
    }

    function checkIfLocalFile()
    {
        if (window.location.protocol != 'file:') {
            return;
        }
        const hostVar = 'host';
        var remoteHost = localStorage.getItem(hostVar);
        if (remoteHost) {
            host = remoteHost;
            return;
        }
        
        remoteHost = prompt("IP of Remote Host?")
        if (remoteHost) {
            localStorage.setItem(hostVar, remoteHost);
            host = remoteHost;
        }
    }

    function initWebSocket()
    {
        webSocket = new WebSocket("ws://" + host + "/ws");
        webSocket.onopen    = onOpen;
        webSocket.onclose   = onClose;
        webSocket.onmessage = onMessage;
    }

    function onOpen(event) {
        console.log('Connection opened');
    }

    function onClose(event) {
        console.log('Connection closed');
        setTimeout(initWebSocket, 1000);
    }

    function onMessage(event) {
        console.log(event);
    }

    document.getElementById('controller').addEventListener("click", function(e) {
        const target = e.target;
        if (target.classList.contains("action-button")) {
            console.log('wow!');
            e.preventDefault(); // stop the button if not type=button 
            webSocket.send(target.id);
        }
    });

    init();
})();