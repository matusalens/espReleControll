void setup(void) {
  delay(1000);
  Serial.begin(115200);
  Serial.print("\n");

  pinMode(RELE_1, OUTPUT);
  pinMode(RELE_2, OUTPUT);
  pinMode(RELE_3, OUTPUT);


  EEPROM.begin(512);
  delay(10);

  // Inicia FS
  SPIFFS.begin();
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next())
    {
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }

  EEPROM.get(WIFI_CONFIG, wifiConfig);
  if(wifiConfig.config_inicial!=1){
    Serial.print("Configuracoes iniciais criadas");
    String ssid_ap ="ETempCtrl_" + String(ESP.getChipId()%1000);;
    String pwd_ap = "Esptempctl";
    String ssid_wifi = "";
    String pwd_wifi = "";
    wifiConfig.config_inicial = 1;
    ssid_ap.toCharArray(wifiConfig.ssid_ap,30);
    pwd_ap.toCharArray(wifiConfig.pwd_ap,30); 
    ssid_wifi.toCharArray(wifiConfig.ssid_wifi,30);
    pwd_wifi.toCharArray(wifiConfig.pwd_wifi,30); 
    
    EEPROM.put(WIFI_CONFIG, wifiConfig);
    EEPROM.commit();
    ESP.restart();
  }
  if(wifiConfig.ssid_wifi==""){
    wifi_error=1;
    EEPROM.put(WIFI_ERROR, wifi_error);  
    EEPROM.commit();
  }
  EEPROM.get(WIFI_ERROR, wifi_error);
  WiFi.mode(WIFI_AP_STA);

  Serial.print("Status do WIFI_ERROR");
  Serial.println(wifi_error);
  if (wifi_error == 0) {
    wifiConnect();
  }

  Serial.print("Configuring access point...");
  WiFi.softAP(wifiConfig.ssid_ap, wifiConfig.pwd_ap, 6, false);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // SERVER INIT
  httpUpdater.setup(&server);
  server.on("/", HTTP_GET, []() {
    if (!handleFileRead("/index.html")) server.send(404, "text/plain", "FileNotFound");
  });

  server.on("/getstatusreles", HTTP_GET, []() {
    String rele1, rele2, rele3;
    if (!Rele_1_flag) {
      rele1 = "0";
    } else {
      rele1 = "1";
    };
        if (!Rele_2_flag) {
      rele2 = "0";
    } else {
      rele2 = "1";
    };
        if (!Rele_3_flag) {
      rele3 = "0";
    } else {
      rele3 = "1";
    };

    // Formata a dados canais em json
   
    String json = "{";

    json += "\"rele1\":\"" + String(rele1) + "\",";
    json += "\"rele2\":\"" + String(rele2) + "\",";
    json += "\"rele3\":\"" + String(rele3) + "\",";
    json += "\"rele4\":\"" + String(Rele_4_flag) + "\",";    
    json += "\"rele5\":\"" + String(Rele_5_flag) + "\",";    
    json += "\"rele6\":\"" + String(Rele_6_flag) + "\",";    
    json += "\"rele7\":\"" + String(Rele_7_flag) + "\",";    
    json += "\"rele8\":\"" + String(Rele_8_flag) + "\"";            
    json += "}";

    server.send(200, "text/json", json);
    json = String();

  });
  server.on("/getconninfo", HTTP_GET, []() {
    
    // Formata a dados canais em json
   
    String json = "{";

    json += "\"connected\":\"" + String(wifi_connected) + "\",";
    json += "\"ip\":\"" + String(connected_ip) + "\"";           
    json += "}";

    server.send(200, "text/json", json);
    json = String();

  });
  
  server.on("/getserverstatus", HTTP_GET, []() {
 // Formata a dados em json
    String json = "{";
    json += "\"status\":\"ok\"";
    json += "}";

    server.send(200, "text/json", json);
    json = String();

  });


  server.on("/setrele", HTTP_GET, []() {
    int rele;
    int valor_temp;
    boolean valor;
    rele = server.arg("rele").toInt();
    valor_temp = server.arg("valor").toInt();
    if(valor_temp == 0){valor=false;}else{valor=true;}
    switch (rele)
    {
      case 1:
      if(!valor){
        Rele_1_flag = false;
        digitalWrite(RELE_1,LOW);
      } else{                
        Rele_1_flag = true;
        digitalWrite(RELE_1,HIGH);
      }
        break;
      case 2:
      if(!valor){//desligar rele1
        Rele_2_flag = false;
        digitalWrite(RELE_2,LOW);
      }else{
        Rele_2_flag = true;
        digitalWrite(RELE_2,HIGH);
      }
        break;
            case 3:
      if(!valor){
        Rele_3_flag = false;
        digitalWrite(RELE_3,LOW);
      }
      else{//ligar rele1
        Rele_3_flag = true;
        digitalWrite(RELE_3,HIGH);
      }
        break;
            case 4:
      if(!valor){
        Rele_4_flag = false;
        digitalWrite(RELE_4,LOW);
      }
      else{//ligar rele1
        Rele_4_flag = true;
        digitalWrite(RELE_4,HIGH);
      }
        break;            
    }
    
    String json = "{";
    json += "\"salvo\":true";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
    
  });


  server.on("/scan1", HTTP_GET, []() { //funcao de apoio
    scanwifi();
    
    String json = "{";
    json += "\"salvo\":true";
    json += "}";
    server.send(200, "text/json", json);
    json = String();
    
  });

  server.on("/scan", HTTP_GET, []() {
    int n = WiFi.scanNetworks();
    StaticJsonBuffer<800> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["redes"] = n;
    JsonArray& data = root.createNestedArray("data");
    for (int i = 0; i < n; i++)
    {
      data.add(WiFi.SSID(i));
    }
    char buffer[800];
    root.printTo(buffer, sizeof(buffer));
    server.send(200, "text/json", buffer);
  });

  server.on("/setwifi", HTTP_GET, []() {
    String rede, senha;
    rede = server.arg("s_redes");
    senha = server.arg("pwd");
    wifi_error = 0;
    rede.toCharArray(wifiConfig.ssid_wifi,30);
    senha.toCharArray(wifiConfig.pwd_wifi, 30);
    EEPROM.put(WIFI_CONFIG, wifiConfig);
    EEPROM.put(WIFI_ERROR, wifi_error);
    EEPROM.commit();        

    if(wifi_connected){
      WiFi.disconnect();
    }
    wifiConnect();
    
    String json = "{";
    json += "\"salvo\":true";
    json += "}";
    server.send(200, "text/json", json);
    json = String();

  });


  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.begin();
  Serial.println("HTTP server started");
}

