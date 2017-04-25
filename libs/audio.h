#ifndef _AUDIO_H_
#define _AUDIO_H_

#include <SDL/SDL.h>
#include <topcoder.h>

// sample: na wszelki wypadek lewy i prawy kanal, gdyby przysylali nam
// stereofoniczne audio

struct sample {
  Sint16 left, right;
  // [0] - lewy kanal, [1] - prawy kanal
  Sint16& operator [] (int i) { return (&left) [i]; }
  };

SDL_AudioSpec spec;

struct wav {
  sample *data;
  Uint32 len;
  };

// wczytywanie wav'a
// dziala z mono, ale tylko w formacie Sint16

void load(wav& w, const char* fname) {
  Uint8* t; 
  if(!SDL_LoadWAV(fname, &spec, &t, &w.len))
    fprintf(debuglog, "loadWAV: %s\n", SDL_GetError());

  fprintf(debuglog, "%dB channels = %d\n", w.len, spec.channels);
    
  if(spec.channels == 2) {
    w.len /= 4;
    w.data = (sample*) t;
    }
  
  if(spec.channels == 1) {
    w.len /= 2;
    Sint16 *d = (Sint16*) t;
    w.data = new sample[w.len];
    for(int i=0; i<int(w.len); i++) w.data[i].left = d[i], w.data[i].right = d[i];
    spec.channels = 2;
    SDL_FreeWAV(t);
    }  

  }

// do wczytywania wava bardziej ogolnego
struct wavheader {
  Uint32 chunkID;
  Uint32 chunksize;
  Uint32 format;
  Uint32 fmtID;
  Uint32 fmtSize;
  Uint16 fmtAudioFormat;
  Uint16 numChannels;
  Uint32 sampleRate;
  Uint32 byteRate;
  Uint16 blockAlign;
  Uint16 bitsPerSample;
  Uint32 dataID;
  Uint32 dataSize;
  };
// podziel dataSize przez (bitsPerSample/8) by dostac liczbe sampli

void close(wav& w) {
  SDL_FreeWAV((Uint8*) w.data);
  }
   
void initAudio(int freq = 44100, int samp = 4096) {
  spec.freq = freq;
  spec.format = AUDIO_S16SYS;
  spec.channels = 2;
  spec.samples = samp;

  if(SDL_OpenAudio(&spec, NULL) != 0) {
    fprintf(debuglog, "OpenAudio: %s\n", SDL_GetError());
    }
  else {
    fprintf(debuglog, "Initialized audio (freq %d, channels %d, buffer %d)\n", spec.freq, spec.channels, spec.samples);
    }
  
  SDL_PauseAudio(0);
  }

#endif
