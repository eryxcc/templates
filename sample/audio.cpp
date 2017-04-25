#include <audio.h>

// przykladowy program odtwarzajacy dzwiek

// loadWAV z SDLa zle dziala, jak format nie jest odpowiedni...

wav wavtest;

int j;

void myAudio(void *userdata, Uint8* stream, int len) {
  if(!wavtest.len) return;
  sample* samples = (sample*) stream;
  len /= sizeof(sample);

  for(int i=0; i<len; i++) {
    samples[i] = wavtest.data[j];
    j++; j%= wavtest.len;
    }
  }

int main(int argc, char ** argv) {
  spec.callback = myAudio;
  wavtest.len = 0;
  initAudio();

  SDL_LockAudio();
  load(wavtest, "wavtest.wav");
  SDL_UnlockAudio();
  
  getchar();
  return 0;
  }
