#include "WTB_Basic.hpp"
#include <cmath>

struct WaveFolder : Module {
  enum ParamIds {
    AMOUNT_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    AMOUNT_INPUT,
    SIGNAL_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    SIGNAL_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  WaveFolder() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;
  float previous = 0.0;

  // For more advanced Module features, read Rack's engine.hpp header file
  // - toJson, fromJson: serialization of internal data
  // - onSampleRateChange: event triggered by a change of sample rate
  // - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void WaveFolder::step() {
  float input = inputs[SIGNAL_INPUT].value;
  float amount;
  if (inputs[AMOUNT_INPUT].active)
    amount = clamp(inputs[AMOUNT_INPUT].value, 0.0f, 5.0f);
  else
    amount = 5.0f;

  amount *= params[AMOUNT_PARAM].value;

  // exponential scaling
  amount *= 0.04f;
  amount *= amount;

  // scale to a musical range
  amount *= 2.5f;

  previous = amount = (amount + previous) * 0.5f; // smoothing
  float output = input - cos(input * amount);
  outputs[SIGNAL_OUTPUT].value = output;
}


struct WaveFolderWidget : ModuleWidget {
  WaveFolderWidget(WaveFolder *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/WaveFolder.svg")));

    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 4 * RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 1 * RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 4 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 1 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addParam(ParamWidget::create<Davies1900hBlackKnob>(Vec(13, 70), module, WaveFolder::AMOUNT_PARAM, 0.0, 5.0, 0.0));

    addInput(Port::create<PJ301MPort>(Vec(18, 150), Port::INPUT, module, WaveFolder::AMOUNT_INPUT));
    addInput(Port::create<PJ301MPort>(Vec(18, 225), Port::INPUT, module, WaveFolder::SIGNAL_INPUT));

    addOutput(Port::create<PJ301MPort>(Vec(18, 300), Port::OUTPUT, module, WaveFolder::SIGNAL_OUTPUT));
  }
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelWaveFolder = Model::create<WaveFolder, WaveFolderWidget>("William Bajzek", "WaveFolder", "Wave Folder", UTILITY_TAG);

