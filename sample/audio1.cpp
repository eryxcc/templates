#include <audio.h>

// przykladowy program odtwarzajacy dzwiek
// uzywamy wczytywania bezposredniego, a nie SDLa

wavheader hdr;

int numSample, curSample;

Sint16 tsamples[1000000];

void myAudio(void *userdata, Uint8* stream, int len) {
  sample* samples = (sample*) stream;
  len /= sizeof(sample);

  for(int i=0; i<len; i++) {
    samples[i][0] = tsamples[curSample];
    samples[i][1] = tsamples[curSample];
    curSample++; curSample%= numSample;
    }
  }

int main(int argc, char ** argv) {

  printf("h size = %d (should be 44)\n", (int) sizeof(hdr));

  FILE *f = fopen("wavtest.wav", "rb");
  scanerr=fread(&hdr, sizeof(hdr), 1, f);
  numSample = hdr.dataSize * 8 / hdr.bitsPerSample;
  printf("numSample = %d\n", numSample);
  
  scanerr=fread(tsamples, numSample, 2, f);
  fclose(f);
  
  spec.callback = myAudio;
  initAudio(hdr.sampleRate);
  
  getchar();
  return 0;
  }
