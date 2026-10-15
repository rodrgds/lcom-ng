addToLibrary({
  lcom_browser_start: function () { Module['player'].attach(Module); },
  lcom_browser_present: function (pixels, width, height) {
    Module['player'].present(HEAPU8.subarray(pixels, pixels + width * height * 4), width, height);
  },
  lcom_browser_audio: function (samples, frames, rate, channels) {
    Module['player'].audio(HEAP16.subarray(samples >> 1, (samples >> 1) + frames * channels), rate, channels);
  },
  lcom_browser_audio_stop: function () { Module['player'].stopAudio(); },
  lcom_browser_paused: function () { return Module['player'].paused ? 1 : 0; },
});
