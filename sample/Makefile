INC=../inc/audio.h ../inc/graph.h ../inc/topcoder.h ../inc/client.h ../inc/luascript.h

FLAGS=-O3 -std=c++0x -I../inc/ -Wall

all: 
	@echo podaj cel

dead: dead.cpp $(INC)
	g++ $(FLAGS) dead.cpp -o dead -lSDL -lSDL_ttf -lgd

audio: audio.cpp $(INC)
	g++ $(FLAGS) audio.cpp -o audio -lSDL

audio1: audio1.cpp $(INC)
	g++ $(FLAGS) audio1.cpp -o audio1 -lSDL

luascript: luascript.cpp $(INC)
	g++ $(FLAGS) -I/usr/include/lua5.1/ luascript.cpp -o luascript -llua5.1

ui: ui.cpp $(INC)
	g++ $(FLAGS) ui.cpp -o ui -lSDL -lSDL_ttf -lgd

obraz: obraz.cpp $(INC)
	g++ $(FLAGS) obraz.cpp -o obraz -lSDL -lSDL_ttf -lgd

grafika: grafika.cpp $(INC)
	g++ $(FLAGS) grafika.cpp -o grafika -lSDL -lSDL_ttf -lSDL_gfx -lgd

svgout: svgout.cpp $(INC)
	g++ $(FLAGS) svgout.cpp -o svgout -lSDL -lSDL_ttf -lSDL_gfx -lgd

server: server.cpp $(INC)
	g++ $(FLAGS) server.cpp -o server

client: client.cpp $(INC)
	g++ $(FLAGS) client.cpp -o client

uiclient: uiclient.cpp $(INC)
	g++ $(FLAGS) uiclient.cpp -o uiclient -lSDL -lSDL_ttf -lgd

threadtest: threadtest.cpp $(INC)
	g++ $(FLAGS) threadtest.cpp -o threadtest -lSDL

mjpeg: mjpeg.cpp $(INC)
	g++ $(FLAGS) mjpeg.cpp -o mjpeg -lSDL -lgd -lSDL_ttf

udp_eryx: udp_eryx.cpp $(INC)
	g++ $(FLAGS) udp_eryx.cpp -o udp_eryx

udp_eryx_listen: udp_eryx_listen.cpp $(INC)
	g++ $(FLAGS) udp_eryx_listen.cpp -o udp_eryx_listen
