package main

import (
    "fmt"
    "net"
    "time"
)

func sendStun(socket *net.UDPConn, stunData []byte) {
    for {
        _, err := socket.Write(stunData) // 发送数据
        if err != nil {
            fmt.Println("发送数据失败, err:", err)
        }
        time.Sleep(time.Second)
    }
}

func main() {
    socket, err := net.DialUDP("udp", nil, &net.UDPAddr{
        IP:   net.IPv4(10, 12, 186, 247),
        Port: 8010,
    })
    if err != nil {
        fmt.Println("连接服务端失败, err:", err)
        return
    }
    defer socket.Close()

    var stunData = []byte{
        0x00, 0x01, 0x00, 0x50, 0x21, 0x12, 0xA4, 0x42, 0x5A, 0x69, 0x54, 0x38, 0x76, 0x41, 0x37, 0x72,
        0x2F, 0x72, 0x63, 0x36, 0x00, 0x06, 0x00, 0x09, 0x52, 0x65, 0x64, 0x30, 0x3A, 0x77, 0x69, 0x6E,
        0x65, 0x00, 0x00, 0x00, 0xC0, 0x57, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x80, 0x2A, 0x00, 0x08,
        0x67, 0x01, 0xA1, 0xC3, 0x7E, 0xE6, 0xFC, 0xB9, 0x00, 0x25, 0x00, 0x00, 0x00, 0x24, 0x00, 0x04,
        0x6E, 0x7F, 0x1E, 0xFF, 0x00, 0x08, 0x00, 0x14, 0x8F, 0x35, 0xE7, 0x5E, 0x05, 0x31, 0x0D, 0x37,
        0xA1, 0xFA, 0x7B, 0x35, 0xDE, 0x86, 0xA7, 0x6B, 0xAB, 0xC3, 0x34, 0x24, 0x80, 0x28, 0x00, 0x04,
        0x9A, 0x7A, 0xC6, 0x1E,
    }

    go sendStun(socket, stunData)

    data := make([]byte, 4096)
    for {
        n, remoteAddr, err := socket.ReadFromUDP(data) // 接收数据
        if err != nil {
            fmt.Println("接收数据失败, err:", err)
            return
        }
        fmt.Printf("recv:%v addr:%v count:%v\n", string(data[:n]), remoteAddr, n)
    }
}
