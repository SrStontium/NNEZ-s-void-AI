#include <WiFi.h>

const char* ssid = "504";
const char* password = "12345678";

WiFiClient client;
const char* serverAddress = "127.0.0.1"; // Server IP address
const int serverPort = 11451;

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // Attempt to connect to the server
    if (!client.connected()) {
        Serial.println("Connecting to server...");
        if (client.connect(serverAddress, serverPort)) {
            Serial.println("Connected to server");
        } else {
            Serial.println("Connection to server failed, retrying in 5 seconds...");
            delay(5000);
            return;
        }
    }
}
      
/*
#include <bits/stdc++.h>
using namespace std;
double attspeed=100,maxhp=4198,hp=4198,sheld=0,sp=0,spspeed=1;
int lastatt=0,lastskill=0;
bool inskill;
double spspdcale(int h)
{
	return 1+clamp(hp-2099,2099,4198)/maxhp*2.3;
}
double attspdcale(int h)
{
	return 100+clamp(hp-2099,2099,4198)/maxhp*70;
}
double attcale(int attspd)
{
	return 1.2/(clamp(spspdcale(hp),20,600));
}
double clamp(int x,int minn,int maxn)
{
	if(x>maxn)
		return maxn;
	else if(x<minn)
		return minn;
	return x;
}
void skill()
{
	hp/=2;
	sheld+=maxhp*1.2;
	sheld=clamp(sheld,0,maxhp*2);
}
void attack()
{
	hp+=70;
	hp=clamp(hp,0,maxhp);
}
bool dmgtest(int dmg)
{
	for(int i=1;i<=9000;i++)
	{
		hp-=dmg;
		cout<<"第"<<i<<"帧（第"<<i/30<<"秒） "; 
		if(i-lastatt>attcale(attspdcale(hp))*30)
		{
			attack();
			lastatt=i;
			cout<<"进行普通攻击 ";
		}
		if(i-lastskill>12&&inskill)
		{
			inskill=false;
			cout<<"技能结束 ";
		}
		if(sp>=20)
		{
			skill();
			inskill=true;
			lastskill=i;
			cout<<"释放技能 "; 
		}
		cout<<"HP:"<<hp<<" SP:"<<sp<<endl;
	}
	return true;
}
int main()
{
	dmgtest(200);
	return 0;
} 
*/
