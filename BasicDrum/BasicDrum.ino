const int threshold = 200;    // 敲擊蜂鳴片的臨界值
unsigned long startTime = 0;  // 記錄開始「擊鼓」的時間
boolean active = 0;           // 代表是否在「作用期間」
unsigned long offTime = 150;  // 代表「敲擊訊號截止時間」 
boolean firstHit = 1;         // 代表是否為「初次敲擊訊號」 
boolean setTime = 0;          // 代表是否「已設定起始時間」

void midiMsg(byte cmd, byte pitch, byte velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  int val = analogRead(A0);

  if((val > threshold)) {
    if (firstHit == 1) {       // 若是「初次敲擊訊號」 
      if (setTime == 0) {      // 若尚未「設定起始時間」
        startTime = micros();  // 記錄起始時間值（微秒）
        setTime = 1;
      }
      
      if (micros() - startTime > 800) {
        firstHit = 0;
        setTime = 0;
      }
    } else {
      if((active == 0)) { 
        val = map(val, threshold, 1023, 50, 127);

        midiMsg(0x99,47,val);
        active = 1;
        startTime = millis();
      }
    }
  }
  
  if((active == 1)) {
    if(millis() - startTime > offTime){
      active = 0;
      midiMsg(0x89,47,0); 
    }
  } 
}

