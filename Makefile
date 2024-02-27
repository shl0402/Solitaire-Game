FLAGS = -g -std=c++11
ifeq ($(OS),Windows_NT)
    command := del /F /Q
else
	command := rm -f
endif

checkInput.o: checkInput.cpp checkInput.h redoUndo.h
	g++ $(FLAGS) -c $<

initTable.o: initTable.cpp initTable.h card.h
	g++ $(FLAGS) -c $<

gui.o: gui.cpp gui.h card.h
	g++ $(FLAGS) -c $<

move.o: move.cpp move.h card.h
	g++ $(FLAGS) -c $<

redoUndo.o: redoUndo.cpp redoUndo.h card.h
	g++ $(FLAGS) -c $<
	
checkWin.o: checkWin.cpp checkWin.h card.h
	g++ $(FLAGS) -c $<
	
start.o: start.cpp gui.h
	g++ $(FLAGS) -c $<

saveLoadFile.o: saveLoadFile.cpp saveLoadFile.h card.h
	g++ $(FLAGS) -c $<

main.o: main.cpp initTable.h
	g++ $(FLAGS) -c $<

main: main.o initTable.o move.o checkInput.o redoUndo.o checkWin.o gui.o start.o saveLoadFile.o
	g++ $(FLAGS) $^ -o $@ -I/ncursesw -lncursesw

clean:
	$(command) *.o *.exe main

run: clean main
	./main
	$(command) *.o *.exe main

run: clean main
	./main

.PHONY: clean
