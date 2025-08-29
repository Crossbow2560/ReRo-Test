#define N 8

int pinArr[N] = {12, 13, 14, 15, 16, 17, 18, 19};
int valArr[N] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  for(int i = 0; i < N; i++){
    pinMode(pinArr[i], INPUT);
  }
  Serial.begin(9600);
}

void loop() {
  for(int i = 0; i < N; i++){
    valArr[N] = digitalRead(pinArr[N]);
  }
  for(int i = 0; i < N; i++){
    Serial.print(valArr[i]);
    Serial.print(" ");
  }
  Serial.println();
}
