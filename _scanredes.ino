void scanwifi(){
  Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  Serial.println("");
}

void wifiConnect(){
  Serial.print("SSID");
  Serial.println(wifiConfig.ssid_wifi);
  Serial.print("PWD");
  Serial.println(wifiConfig.pwd_wifi);
  WiFi.begin(wifiConfig.ssid_wifi, wifiConfig.pwd_wifi);

    for (int i = 0; i < 20; i++) {
      delay(1000);
      Serial.println(WiFi.status());
      if (WiFi.status() == 3) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        wifi_connected=true;
        //WiFi.localIP().toCharArray(connected_ip,12);
        String ipconectado;
        ipconectado = WiFi.localIP().toString();
        ipconectado.toCharArray(connected_ip,14);
        Serial.print("Connected IP");
        Serial.println(connected_ip);
        break;
      }
      if(i==19){
      wifi_connected=false;
      Serial.print(wifi_connected);
      wifi_error=1;
      EEPROM.put(WIFI_ERROR, wifi_error);
      EEPROM.commit();
      }
    }
}

