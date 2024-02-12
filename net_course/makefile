FLAGS := -Wall  -Wpedantic -Iout -Iinc 
COMPILER := gcc

app.exe : out/main.o out/connection.o out/log.o out/mime.o out/server.o out/worker.o
	$(COMPILER) $(FLAGS) $^ -o $@
	
out/main.o : src/main.c
	$(COMPILER) $(FLAGS) $< -c -o $@

out/connection.o : src/connection.c
	$(COMPILER) $(FLAGS) $< -c -o $@

out/log.o : src/log.c
	$(COMPILER) $(FLAGS) $< -c -o $@

out/mime.o : src/mime.c
	$(COMPILER) $(FLAGS) $< -c -o $@

out/server.o : src/server.c
	$(COMPILER) $(FLAGS) $< -c -o $@

out/worker.o : src/worker.c
	$(COMPILER) $(FLAGS) $< -c -o $@


.PHONY : clean
.PHONY : reset


clean : 
	rm *.exe out/*.o
	
reset:
	rm docs/*.aux