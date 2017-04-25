  /*
   * w[i][j] = amount bidder j is willing to pay for item i (0 if he is not bidding)
   * run time is O(nm^2) where items = #of items and bidders = #of bidders
   * resets negative bids in w to 0
   * returns asg, where asg[i] = j means ith item got assigned to bidder j
   * asg[i] = -1 means item i did not get assigned
   */

// --- hungarian ---

#define MAXIT 4000
#define MAXBID 500

#define PHI -1
#define NOL -2

namespace hungarian {

int items, bidders;

int w[MAXIT][MAXBID];
  bool x[MAXIT][MAXBID];

int asg[MAXIT];

void hungarian() {

  bool ss[MAXIT];
  bool st[MAXBID];
  
  int u[MAXIT];
  int v[MAXBID];
  int p[MAXBID];
  int ls[MAXIT];
  int lt[MAXBID];
  
  int f = 0;
  
  FOR(i,0,items) FOR(j,0,bidders) f = max(f, w[i][j]);
  
  FOR(i,0,items) FOR(j,0,bidders) x[i][j] = false;
  
  FOR(i,0,items) u[i] = f, ls[i] = PHI, ss[i] = false;

  FOR(j,0,bidders) v[j] = 0, p[j] = INF, lt[j] = NOL, st[j] = false;

  while(true) {
  
    f = -1;
    
    FOR(i, 0, items) if(ls[i] != NOL && !ss[i]) {
      f = i;
      break;
      }
    
    if(f != -1) {
      ss[f] = true;
      FOR(j, 0, bidders)
        if(!x[f][j] && u[f] + v[j] - w[f][j] < p[j])
          {
            lt[j] = f;
            p[j] = u[f] + v[j] - w[f][j];
          }
      }
    else {
    
      FOR(j, 0, bidders)
        if(lt[j] != NOL && !st[j] && p[j] == 0) {
          f = j;
          break;
          }
        
      if(f == -1) {
        int d1 = INF, d2 = INF, d;
        FOR(i, 0, items) d1 = min(d1, u[i]);
      
        FOR(j, 0, bidders) if(p[j] > 0) d2 = min(d2, p[j]);
        
        d = min(d1, d2);
        
        FOR(i, 0, items) if(ls[i] != NOL) u[i] -= d;
        
        FOR(j, 0, bidders) {
          if(p[j] == 0) v[j] += d;
          if(p[j] > 0 && lt[j] != NOL) p[j] -= d;
          }
 
        if(d2 >= d1) break;
        }
      else {
        st[f] = true;
        int s = -1;
        
        for(int i = 0; i < items && s == -1; i++)
          if(x[i][f])
            s = i;
 
        if(s == -1) {
          int r = f;

          while(true) {
            int l = lt[r];
            if(l < 0) break;
            x[l][r] = !x[l][r];
            
            r = ls[l];
            if(r < 0) break;
            x[l][r] = !x[l][r];
            }
            
          FOR(i,0,items) ls[i] = PHI, ss[i] = false;

          FOR(j,0,bidders) p[j] = INF, lt[j] = NOL, st[j] = false;
        
          FOR(i,0,items) {
            FOR(j,0,bidders) if(x[i][j]) { 
              ls[i] = NOL;
              break;
              }
            }
          }
        else {
          ls[s] = f;
          }
        }
      }
    }
 
  FOR(i,0,items) {
    asg[i] = -1;
    FOR(j,0,bidders) if(x[i][j]) asg[i] = j;
    }
  }

}

// --- end of hungarian ---
