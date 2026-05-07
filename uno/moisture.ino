const int relayPin = 7; 
String dataMasuk = "";

// Variabel default jika tidak ada koneksi
int modeOtomatis = 0;
int jedaDetik = 10;
int durasims = 2000;

void setup() {
  // Komunikasi serial internal ke chip WiFi
  Serial.begin(9600); 
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Relay Mati (Active Low)
  
  Serial.println("Arduino Ready. Menunggu data WiFi...");
}

void loop() {
  // 1. Cek apakah ada kiriman data baru dari chip WiFi (ESP)
  if (Serial.available() > 0) {
    dataMasuk = Serial.readStringUntil('\n');
    
    // Parsing data yang dikirim ESP (format: mode,jeda,durasi)
    int index1 = dataMasuk.indexOf(',');
    int index2 = dataMasuk.lastIndexOf(',');
    
    if (index1 != -1 && index2 != -1) {
      modeOtomatis = dataMasuk.substring(0, index1).toInt();
      jedaDetik = dataMasuk.substring(index1 + 1, index2).toInt();
      durasims = dataMasuk.substring(index2 + 1).toInt();
      
      Serial.print("Data Diterima - Mode: "); Serial.print(modeOtomatis);
      Serial.print(" Jeda: "); Serial.print(jedaDetik);
      Serial.print(" Durasi: "); Serial.println(durasims);
    }
  }

  // 2. Logika Eksekusi Relay (DI LUAR blok Serial.available)
  // Agar sistem tetap menyiram berulang kali meskipun tidak ada data serial masuk
  if (modeOtomatis == 1) {
    Serial.println("Menyiram...");
    digitalWrite(relayPin, LOW);  // Pompa Nyala
    delay(durasims);              // Durasi siram
    
    digitalWrite(relayPin, HIGH); // Pompa Mati
    Serial.println("Jeda...");
    
    // Jeda sesuai pengaturan Firebase
    delay(jedaDetik * 1000); 
  } else {
    digitalWrite(relayPin, HIGH);
    // Tambahkan sedikit delay agar loop tidak terlalu cepat saat standby
    delay(500); 
  }
}