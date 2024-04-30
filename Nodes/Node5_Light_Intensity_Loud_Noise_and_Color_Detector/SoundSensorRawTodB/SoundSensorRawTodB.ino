const int sampleWindow = 50;  // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;

void setup() {
  Serial.begin(115200);
}

void loop() {

  unsigned long startMillis = millis();  // Start of sample window
  float peakToPeak = 0;                  // peak-to-peak level

  unsigned int signalMax = 0;    //minimum value
  unsigned int signalMin = 280;  //maximum value

  while (millis() - startMillis < sampleWindow) {
    sample = analogRead(A2);  //get reading from microphone
    if (sample < 1024)        // toss out spurious readings
    {
      if (sample > signalMax) {
        signalMax = sample;  // save just the max levels
      } else if (sample < signalMin) {
        signalMin = sample;  // save just the min levels
      }
    }
  }

  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  float db = map(peakToPeak, 10, 280, 55, 100);
  Serial.println(db);

  delay(1);  // delay in between reads for stability
}
