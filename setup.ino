void setup(void) {
  delay(1000);
  Serial.begin(115200);
  Serial.print("\n");



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
    String pwd_ap = "home2016";
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
    WiFi.begin(wifiConfig.ssid_wifi, wifiConfig.pwd_wifi);

    for (int i = 0; i < 20; i++) {
      delay(1000);
      Serial.println(WiFi.status());
      if (WiFi.status() == 3) {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        break;
      }
    }
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
    String rele4,rele5,rele6;
    String rele7,rele8;
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
        if (!Rele_4_flag) {
      rele4 = "0";
    } else {
      rele4 = "1";
    };

       if (!Rele_5_flag) {
      rele5 = "0";
    } else {
      rele5 = "1";
    };

     if (!Rele_6_flag) {
      rele6 = "0";
    } else {
      rele6 = "1";
    };

     if (!Rele_7_flag) {
      rele7 = "0";
    } else {
      rele7 = "1";
    };
     if (!Rele_8_flag) {
      rele8 = "0";
    } else {
      rele8 = "1";
    };

    // Formata a dados canais em json
   
    String json = "{";

    json += "\"rele1\":\"" + String(rele1) + "\",";
    json += "\"rele2\":\"" + String(rele2) + "\",";
    json += "\"rele3\":\"" + String(rele3) + "\",";
    json += "\"rele4\":\"" + String(rele4) + "\",";    
    json += "\"rele5\":\"" + String(rele5) + "\",";    
    json += "\"rele6\":\"" + String(rele6) + "\",";    
    json += "\"rele7\":\"" + String(rele7) + "\",";    
    json += "\"rele8\":\"" + String(rele8) + "\",";            
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
        mcu.println('a');
        Serial.println('a');
      } else{                
        Rele_1_flag = true;
       mcu.println('A');
       Serial.println('A');
      }
        break;
      case 2:
      if(!valor){//desligar rele1
        Rele_2_flag = false;
        mcu.println('b');
        Serial.println('b');
      }else{
        Rele_2_flag = true;
        mcu.println('B');
        Serial.println('B');
      }
        break;
            case 3:
      if(!valor){
        Rele_3_flag = false;
       mcu.println('c');
       Serial.println('c');
      }
      else{//ligar rele1
        Rele_3_flag = true;
       mcu.println('C');
       Serial.println('C');
      }
        break;
            case 4:
      if(!valor){
        Rele_4_flag = false;
       mcu.println('d');
       Serial.println('d');
      }
      else{//ligar rele1
        Rele_4_flag = true;
       mcu.println('D');
       Serial.println('D');
      }
        break;

case 5:
 if(!valor){
        Rele_5_flag = false;
       mcu.println('e');
       Serial.println('e');
      } else{                
        Rele_5_flag = true;
        mcu.println('E');
        Serial.println('E');
      }
        break;
      case 6:
      if(!valor){//desligar rele1
        Rele_6_flag = false;
        mcu.println('f');
        Serial.println('f');
      }else{
        Rele_6_flag = true;
       mcu.println('F');
       Serial.println('F');
      }
        break;
       case 7:
      if(!valor){
        Rele_7_flag = false;
        mcu.println('h');
        Serial.println('h');
      }
      else{//ligar rele1
        Rele_7_flag = true;
        mcu.println('H');
        Serial.println('H');
      }
        break;
            case 8:
      if(!valor){
        Rele_8_flag = false;
      mcu.println('g');
      Serial.println('g');
      }
      else{//ligar rele1
        Rele_8_flag = true;
        mcu.println('G');
        Serial.println('G');
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

