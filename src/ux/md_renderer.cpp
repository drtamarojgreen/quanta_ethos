#include "md_renderer.h"

MdRenderer::MdRenderer() {}

std::string MdRenderer::render(const std::string& md) {
    // Simple mock rendering: replace **bold** with [BOLD]bold[/BOLD]
    return md;
}
