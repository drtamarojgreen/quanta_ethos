#include "core_engine.h"
#include "model_backend.h"
#include "ethics_logic.h"

CoreEngine::CoreEngine() {}

std::string CoreEngine::generate_response(const std::string& prompt) {
    std::string ethicalPrompt = makeEthicalPrompt(prompt);

    ModelBackend model_backend;
    std::string model_output = model_backend.run_model(ethicalPrompt);

    std::string reviewedOutput = applyTrustworthinessChecks(model_output);
    std::string reviewNotes = runSelfReview(model_output);
    float score = computeTrustScore(reviewNotes);

    logInteractionToJson(prompt, ethicalPrompt, model_output, reviewNotes, score);
    logInteractionToCsv(prompt, score);

    return reviewedOutput;
}
