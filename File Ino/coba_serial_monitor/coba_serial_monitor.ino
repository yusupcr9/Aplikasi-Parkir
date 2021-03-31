String Answer;
void setup(){
  Serial.begin(9600);
}
void loop(){
  while(Serial.available()==0);
  Answer = Serial.readStringUntil('\n');
  if (Answer == "yusup"){
    Serial.println(Answer+" , Ganteng");
  }
  else{
    Serial.println(Answer+", wrong Answer");
  }
}
