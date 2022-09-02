
all: shell


./obj/errors.o: ./err/errors.c ./err/errors.h
	gcc -c ./err/errors.c -o ./obj/errors.o


./obj/prompt.o: ./shPrompt/prompt.c ./shPrompt/prompt.h
	gcc -c ./shPrompt/prompt.c -o ./obj/prompt.o -g

./obj/print_wd.o: ./print_wd/print_wd.c ./print_wd/print_wd.h
	gcc -c ./print_wd/print_wd.c -o ./obj/print_wd.o -g


./obj/ech.o: ./ech/ech.c ./ech/ech.h
	gcc -c ./ech/ech.c -o ./obj/ech.o -g

./obj/cdir.o: ./change_dir/cdir.c ./change_dir/cdir.h
	gcc -c ./change_dir/cdir.c -o ./obj/cdir.o -g



shell: ./obj/errors.o ./obj/prompt.o ./obj/print_wd.o ./obj/ech.o ./obj/cdir.o gvars.h shell.c
	gcc ./obj/errors.o ./obj/prompt.o ./obj/print_wd.o ./obj/ech.o ./obj/cdir.o shell.c -o shell -g


