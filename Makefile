
all: shell

./obj/errors.o: ./err/errors.c ./err/errors.h
	gcc -c ./err/errors.c -o ./obj/errors.o


./obj/prompt.o: ./shPrompt/prompt.c ./shPrompt/prompt.h
	gcc -c ./shPrompt/prompt.c -o ./obj/prompt.o

./obj/print_wd.o: ./print_wd/print_wd.c ./print_wd/print_wd.h
	gcc -c ./print_wd/print_wd.c -o ./obj/print_wd.o


shell: ./obj/errors.o ./obj/prompt.o ./obj/print_wd.o shell.c
	gcc ./obj/errors.o ./obj/prompt.o shell.c ./obj/print_wd.o -o shell
