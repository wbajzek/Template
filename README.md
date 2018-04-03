
# VCV plugins

1. SimpleQuantizer - quantizes input CV to the equally tempered chromatic
   scale. Send it any voltage, and every time it receives a trigger, it will output
   a quantized voltage. When trigger is disconnected, it will quantize the
   input as it changes.
2. WaveFolder - A voltage-controllable cosine waveshaper. Reponds to positive
   CV between 0 and 5 volts, scaled by the amount control. The CV input is
   normalled to +5v when disconnected.
