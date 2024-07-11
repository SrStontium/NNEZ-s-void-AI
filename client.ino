#include <WiFi.h>
void setup() {
    //初始化
    Serial.begin(115200);
    WiFi.init();
    //连接WiFi
    WiFi.begin("504", "12345678");
    Serial.println("连接中.");
    while(WiFi.status()!=WL_CONNECTED)
    {
        Serial.println(".");
        delay(1000);
    }
    Serial.println("连接成功！");
}

void loop() {
    TCPClient client;
    const char* ServerAddress="127.0.0.1";
    int ServerPort=11451;
    //尝试连接服务器
    if(!client.connect(ServerAddress, ServerPort))
    {
        Serial.println("与服务器断开连接，正在尝试重新连接...");
        delay(5000); // 等待一段时间再尝试连接（这里是 5 秒）
        return;
    }
    //若连接成功
    else
    {
        char question[10005]="This is a question.",answer[10005];
        //假设question里是问题
        client.send(question);
        client.receive(answer);
        Serial.println("answer");
    }
}
