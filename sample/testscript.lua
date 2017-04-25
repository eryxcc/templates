-- skrypty w Lua, po to, zeby mozna bylo zmieniac parametry przez
-- edycje pliku (i je testowac bez ponownego uruchamiania ani
-- ponownej kompilacji), a takze po to, zeby w ogole mozna bylo 
-- programowo wplywac na dzialanie algorytmu (szybko, tzn. bez
-- ponownego uruchamiania ani ponownej kompilacji), ja chcialbym
-- miec cos takiego, ale to pewnie nie jest bardzo potrzebne

function testcall()
  print("called!")
  end

param_x = 1000
param_y = 750

setarg()
