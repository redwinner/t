package main

import (
    "fmt"
    "net/http"
    "github.com/gorilla/websocket"
    "log"
    "time"
)

func main() {
    dialer := websocket.Dialer{}
    header := http.Header{
        "Sec-Websocket-Protocol": []string{"janus-protocol"},
    }
    connect, _, err := dialer.Dial("ws://r.redwinner.cn:8800/brtc", header)
    if nil != err {
        log.Println(err)
        return
    }
    defer connect.Close()

    go onMessage(connect)

    connect.WriteMessage(websocket.TextMessage, []byte("{\"janus\":\"create\"," +
        "\"transaction\":\"J1N0by6A3UUK\",\"sdktag\":\"BRTC.Go.SDK V0.0.1\",\"uri\":\"ws://r.redwinner.cn:8800/brtc\"," +
        "\"userevent\":true,\"sessionevent\":true}"))

    for {
        fmt.Printf("********** videocall demo V0.1 *********\n");
        fmt.Printf("Please input: h to Hangup.\n");
        fmt.Printf("              r to ReDial.\n");
        fmt.Printf("              t to LoopTest.\n");
        fmt.Printf("              q to Quit.\n");
        var commandid string

        ret, _ := fmt.Scanf("%s", &commandid)
        fmt.Printf("scanf %d.\n", ret);

        if (ret < 0) {
            time.Sleep(time.Second * 200);
            continue;
        }

        switch commandid[0] {
            case 'i':
                break;
            case 't': // loop test, t100 = 100 times.
            {
                break;
            }
        }

        if (commandid[0] == 'q') {
            break;
        }
    }
}

func tickWriter(connect *websocket.Conn) {
    for {
        err := connect.WriteMessage(websocket.TextMessage, []byte("from client to server"))
        if nil != err {
            log.Println(err)
            break
        }
        time.Sleep(time.Second)
    }
}

func onMessage(connect *websocket.Conn) {
    for {
        messageType, messageData, err := connect.ReadMessage()
        if nil != err {
            log.Println(err)
            break
        }
        switch messageType {
        case websocket.TextMessage:
            fmt.Println(string(messageData))
        case websocket.BinaryMessage:
            fmt.Println(messageData)
        case websocket.CloseMessage:
        case websocket.PingMessage:
        case websocket.PongMessage:
        default:
        }
    }
}