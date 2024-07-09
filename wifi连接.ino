#include<WiFi.h>
#include <bits/stdc++.h>
const char* id="YOUR_WIFI_NAME";   //定义两个字符串指针常量

void waiting()
{
  for(;WiFi.status()!=WL_CONNECTED;)
    {
      Serial.print(".");
      delay(500);
    }
}

const char* psw="YOUR_PASS_WORD";

void setup() {
  Serial.begin(115200);
  Serial.print("has started");
  WiFi.begin(id,psw);
  while(WiFi.status()!=WL_CONNECTED)
  {			//未连接上
    delay(500);
    Serial.println("正在连接");
    waiting();
  }
  Serial.println("连接成功！");				//连接上
}
void loop(){		
  Serial.print("ok");
  delay(100);		//空循环
}
