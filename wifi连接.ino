#include<WiFi.h>
#include <bits/stdc++.h>
const char* id="HUAWEI-H5SRCA";   //定义两个字符串指针常量
const char* psw="Su092600";

void setup() {
  Serial.begin(115200);
  Serial.print("has started");
  WiFi.begin(id,psw);
  while(WiFi.status()!=WL_CONNECTED){			//未连接上
    delay(500);
    Serial.println("正在连接...");
  }
  Serial.println("连接成功！");				//连接上
}
void loop(){		
  Serial.print("ok");
  delay(100);		//空循环
}