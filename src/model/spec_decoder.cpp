#include "spec_decoder.h"

SpeculativeDecoder::SpeculativeDecoder(IModelBackend* draft_model, IModelBackend* target_model)
    : draft(draft_model), target(target_model) {}

std::string SpeculativeDecoder::generate(const std::string& prompt) {
    // Basic speculative logic: draft generates, target verifies.
    std::string draft_output = draft->run_model(prompt);
    // verification step simulation
    return target->run_model(draft_output);
}
