#include <Adafruit_NeoPixel.h>

volatile bool PatternChanged = false;

Adafruit_NeoPixel Strip(40, 6, NEO_GRB + NEO_KHZ800);

void boyGirlPattern(Adafruit_NeoPixel* Strip, long unsigned Time);
void solidBluePattern(Adafruit_NeoPixel* Strip, long unsigned Time);
void breathingBluePattern(Adafruit_NeoPixel* Strip, long unsigned Time);
void solidPinkPattern(Adafruit_NeoPixel* Strip, long unsigned Time);
void breathingPinkPattern(Adafruit_NeoPixel* Strip, long unsigned Time);
void rainbowPattern(Adafruit_NeoPixel* Strip, long unsigned Time);

typedef void (*PatternFunc_t)(Adafruit_NeoPixel*, long unsigned Time);

int const PatternCount = 6;
PatternFunc_t PatternLookupTable[PatternCount] = {
  boyGirlPattern,
  solidBluePattern,
  breathingBluePattern,
  solidPinkPattern,
  breathingPinkPattern,
  rainbowPattern,
};

void setup(void) {
  Strip.begin();
  
  int const ButtonPin = 2;
  pinMode(ButtonPin, INPUT_PULLUP);
  
  int const Interrupt = digitalPinToInterrupt(ButtonPin);
  attachInterrupt(Interrupt, advanceToNextPattern, LOW);
}

void loop(void) {
  static int ActivePattern = 0;
  static long unsigned PatternTime = 0;
  static long unsigned PreviousTime = millis();

  if (PatternChanged) {
    ActivePattern += 1;
    ActivePattern %= PatternCount;
    
    PatternTime = 0;
    PatternChanged = false;
  }
  
  if (ActivePattern < PatternCount) {
    if (PatternLookupTable[ActivePattern] != NULL) {
      PatternLookupTable[ActivePattern](&Strip, PatternTime);
    }
  }

  long unsigned CurrentTime = millis();
  PatternTime += CurrentTime - PreviousTime;
  PreviousTime = CurrentTime;
}

void advanceToNextPattern(void) {
  static long unsigned PreviousTime = 0;
  
  long unsigned CurrentTime = millis();
  long unsigned const DebounceThreshold = 50;
  if ((CurrentTime - PreviousTime) > DebounceThreshold) {
    PatternChanged = true;
    
    /// @Incomplete:
    /// Should this be inside or outside the `if` statement?
    ///                     - Brian Rowlett, 2021-07-12
    PreviousTime = CurrentTime;
  }
}

///
/// Patterns
///

struct State_t {
  long ActiveValue;
  long MinimumValue;
  long MaximumValue;
  long DeltaPerFrame;
};

void boyGirlPattern(Adafruit_NeoPixel* Strip, long unsigned Time) {
  long const Rate = 1;
  static State_t State = {};
  
  if (Time == 0) {
    State.ActiveValue = 36408;
    State.MinimumValue = 36408;
    State.MaximumValue = 58254;
    State.DeltaPerFrame = Rate;
  }

  Strip->setBrightness(255);
  for (int i = 0; i < Strip->numPixels(); ++i) {
    Strip->setPixelColor(i, Strip->gamma32(Strip->ColorHSV(State.ActiveValue)));
  }
  
  Strip->show();

  State.ActiveValue += State.DeltaPerFrame;
  if (State.ActiveValue > State.MaximumValue) {
    State.ActiveValue = State.MaximumValue;
    State.DeltaPerFrame = -Rate;
  }
  else if (State.ActiveValue < State.MinimumValue) {
    State.ActiveValue = State.MinimumValue;
    State.DeltaPerFrame = Rate;
  }
}

void solidBluePattern(Adafruit_NeoPixel* Strip, long unsigned Time) {
  Strip->setBrightness(255);
  for (int i = 0; i < Strip->numPixels(); ++i) {
    Strip->setPixelColor(i, 0, 0, 255);
  }

  Strip->show();
}

void breathingBluePattern(Adafruit_NeoPixel* Strip, long unsigned Time) {
  long const Rate = 1;
  long const Scale = 8;
  static State_t State = {};
  
  if (Time == 0) {
    State.ActiveValue = 16 * Scale;
    State.MinimumValue = 16 * Scale;
    State.MaximumValue = 255 * Scale;
    State.DeltaPerFrame = Rate;
  }

  Strip->setBrightness(State.ActiveValue / Scale);
  for (int i = 0; i < Strip->numPixels(); ++i) {
    Strip->setPixelColor(i, 0, 0, 255);
  }

  Strip->show();

  State.ActiveValue += State.DeltaPerFrame;
  if (State.ActiveValue > State.MaximumValue) {
    State.ActiveValue = State.MaximumValue;
    State.DeltaPerFrame = -Rate;
  }
  else if (State.ActiveValue < State.MinimumValue) {
    State.ActiveValue = State.MinimumValue;
    State.DeltaPerFrame = Rate;
  }
}

void solidPinkPattern(Adafruit_NeoPixel* Strip, long unsigned Time) {
  Strip->setBrightness(255);
  for (int i = 0; i < Strip->numPixels(); ++i) {
    Strip->setPixelColor(i, 255, 0, 255);
  }

  Strip->show();
}

void breathingPinkPattern(Adafruit_NeoPixel* Strip, long unsigned Time) {
  long const Rate = 1;
  long const Scale = 8;
  static State_t State = {};
  
  if (Time == 0) {
    State.ActiveValue = 16 * Scale;
    State.MinimumValue = 16 * Scale;
    State.MaximumValue = 255 * Scale;
    State.DeltaPerFrame = Rate;
  }

  Strip->setBrightness(State.ActiveValue / Scale);
  for (int i = 0; i < Strip->numPixels(); ++i) {
    Strip->setPixelColor(i, 255, 0, 255);
  }

  Strip->show();

  State.ActiveValue += State.DeltaPerFrame;
  if (State.ActiveValue > State.MaximumValue) {
    State.ActiveValue = State.MaximumValue;
    State.DeltaPerFrame = -Rate;
  }
  else if (State.ActiveValue < State.MinimumValue) {
    State.ActiveValue = State.MinimumValue;
    State.DeltaPerFrame = Rate;
  }
}

void rainbowPattern(Adafruit_NeoPixel* Strip, long unsigned Time) {
  long const Rate = 4;
  static State_t State = {};
  
  if (Time == 0) {
    State.ActiveValue = 0;
    State.MinimumValue = 0;
    State.MaximumValue = 65536;
    State.DeltaPerFrame = Rate;
  }

  Strip->setBrightness(255);
  for (int i = 0; i < Strip->numPixels(); ++i) {
    Strip->setPixelColor(i, Strip->gamma32(Strip->ColorHSV(State.ActiveValue)));
  }
  
  Strip->show();

  State.ActiveValue += State.DeltaPerFrame;
  if (State.ActiveValue > State.MaximumValue) {
    State.ActiveValue = State.MinimumValue;
  }
}
