// WiFi.ino
// Copyright (c) 2020 Roman Hujer   http://hujer.net
//
#ifdef WIFI_ON

void wifi_setup(){
 
  if( ! WiFiConnected ){  
  
    WiFi.mode(WIFI_STA);  
    WiFi.begin(ssid, password);
#ifdef DEBUG_WIFI_ON  
    Serial.printf("Wait for WiFi.");
#endif
  for (uint8_t t = 30; t > 0; t--) {
    if ( WiFi.status() == WL_CONNECTED )break;
    delay(500);
#ifdef DEBUG_WIFI_ON  
    Serial.print(".");
#endif 
  }  
#ifdef ALT_SSID_ON

#ifdef DEBUG_WIFI_ON  
    Serial.printf("\nALT_SSID");
#endif

  if ( WiFi.status() != WL_CONNECTED ){
  
    WiFi.begin(ssid2, password2);
    for (uint8_t t = 30; t > 0; t--) {
      if ( WiFi.status() == WL_CONNECTED )  break;
      delay(500);
#ifdef DEBUG_WIFI_ON  
      Serial.print(":");
#endif 
    }
  } 
#endif  
 }
 if (WiFi.status() != WL_CONNECTED) {
#ifdef DEBUG_WIFI_ON      
    Serial.printf("\nWIFI not connect.");
#endif        
    WiFiConnected = false; 
  } else {
    WiFiConnected = true; 
#ifdef DEBUG_WIFI_ON     
   Serial.println("\nWiFi connected");
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP());
#endif    
  }
  

}

void wifi_main(double mpsas, double dmpsas, int temp, byte hum , int pres )
{
  if ( ! WiFiConnected ) return; 
  String response;
  String url;
  float battery = 0;
  
  battery = int(analogRead(A0) / 1023.0 * 11 * 100 + 0.5) / 100.;
#ifdef DEBUG_WIFI_ON  
  Serial.println("");
  Serial.print("SQM: ");
  Serial.print(mpsas);
  Serial.print("+-");
  Serial.print(dmpsas);
  Serial.println("mas^2");
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println("°C");
  Serial.print("Humidity: ");
  Serial.print(hum );
  Serial.println("%");
  Serial.print("Pressure: ");
  Serial.print(pres/100);
  Serial.println(" hPa");
  Serial.print("Battery Voltage: ");
  Serial.println(battery);
#endif  
    url = "?ID=";
    url += SensorID;
    url += "&KEY=";
    url += sensor_key;
    url += "&T=";
    url +=  temp;
    url += "&H=";
    url += hum;
    url += "&P=";
    url += pres/100;
    url += "&S=";
    url += mpsas;
    url += "&D=";
    url += dmpsas;
    url += "&V=";
    url += battery;
#ifdef GPS_ON     
    if ( g_sat > 2 ){
     url += "&Alt=";      
     url += g_alt;
     url += "&Lat=";      
     url += g_lat;
     url += "&Lon=";      
     url += g_lng;
    }
#endif    
    send_cloud ( url  );
}



void send_cloud ( String url)
{
  

  WiFiClient wifi_client;
  const int httpPort = HTTP_PORT;


  if (!wifi_client.connect(host, httpPort)) {
    Serial.println("connection failed");
    WiFiConnected = false;
    return;
  }

  // This will send the request to the server
  wifi_client.print(String("GET ") + app + url + " HTTP/1.1\r\n" +
                    "Host: " + host + "\r\n" +
                    "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (wifi_client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("HTTP Timeout !");
      wifi_client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (wifi_client.available()) {
    String line = wifi_client.readStringUntil('\r');
   Serial.print(line);
  }
  Serial.println();
  Serial.println("closing connection");

}

#endif
