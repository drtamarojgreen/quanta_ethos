console.log("PrismQuanta UI Loaded");
function generate() {
    fetch('/v1/generate', { method: 'POST', body: JSON.stringify({prompt: 'hello'}) });
}
