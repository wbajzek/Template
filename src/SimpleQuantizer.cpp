#include "WTB_Basic.hpp"
#include "dsp/digital.hpp"

struct SimpleQuantizer : Module {
  enum ParamIds {
    PITCH_PARAM,
    NUM_PARAMS
  };
  enum InputIds {
    TRIGGER_INPUT,
    PITCH_INPUT,
    NUM_INPUTS
  };
  enum OutputIds {
    QUANTIZED_OUTPUT,
    NUM_OUTPUTS
  };
  enum LightIds {
    NUM_LIGHTS
  };

  float phase = 0.0;
  float quantized_pitch = 0.0;
  SchmittTrigger trigger;

  SimpleQuantizer() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}
  void step() override;

  // For more advanced Module features, read Rack's engine.hpp header file
  // - toJson, fromJson: serialization of internal data
  // - onSampleRateChange: event triggered by a change of sample rate
  // - onReset, onRandomize, onCreate, onDelete: implements special behavior when user clicks these from the context menu
};


void SimpleQuantizer::step() {
  float pitch = inputs[PITCH_INPUT].value;
  if (trigger.process(inputs[TRIGGER_INPUT].value) || !inputs[TRIGGER_INPUT].active) {
    pitch = clamp(pitch, -5.0f, 5.0f);
    pitch += 5.0f;
    pitch *= 12.0/1.0;
    pitch = (int)pitch;
    pitch *= 1.0/12.0;
    pitch -= 5.0f;
    quantized_pitch = pitch;
  }
  outputs[QUANTIZED_OUTPUT].value = quantized_pitch;
}


struct SimpleQuantizerWidget : ModuleWidget {
  SimpleQuantizerWidget(SimpleQuantizer *module) : ModuleWidget(module) {
    setPanel(SVG::load(assetPlugin(plugin, "res/SimpleQuantizer.svg")));

    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(Widget::create<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

    addInput(Port::create<PJ301MPort>(Vec(3, 110), Port::INPUT, module, SimpleQuantizer::TRIGGER_INPUT));

    addInput(Port::create<PJ301MPort>(Vec(3, 205), Port::INPUT, module, SimpleQuantizer::PITCH_INPUT));

    addOutput(Port::create<PJ301MPort>(Vec(3, 300), Port::OUTPUT, module, SimpleQuantizer::QUANTIZED_OUTPUT));
  }
};


// Specify the Module and ModuleWidget subclass, human-readable
// author name for categorization per plugin, module slug (should never
// change), human-readable module name, and any number of tags
// (found in `include/tags.hpp`) separated by commas.
Model *modelSimpleQuantizer = Model::create<SimpleQuantizer, SimpleQuantizerWidget>("WTB", "SimpleQuantizer", "Simple Quantizer", UTILITY_TAG);
