
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

./obj/ls.o: ./list_dir/ls.c ./list_dir/ls.h
	gcc -c ./list_dir/ls.c -o ./obj/ls.o -g

./obj/history.o: ./history/history.c ./history/history.h
	gcc -c ./history/history.c -o ./obj/history.o -g

./obj/fgbg.o: ./fgbg/fgbg.c ./fgbg/fgbg.h
	gcc -c ./fgbg/fgbg.c -o ./obj/fgbg.o -g

./obj/pinfo.o: ./pinfo/pinfo.c ./pinfo/pinfo.h
	gcc -c ./pinfo/pinfo.c -o ./obj/pinfo.o -g


./obj/discover.o: ./discover/discover.c ./discover/discover.h
	gcc -c ./discover/discover.c -o ./obj/discover.o -g


shell: ./obj/errors.o ./obj/prompt.o ./obj/print_wd.o ./obj/ech.o ./obj/cdir.o ./obj/ls.o ./obj/history.o ./obj/fgbg.o ./obj/pinfo.o ./obj/discover.o gvars.h shell.c
	gcc ./obj/errors.o ./obj/prompt.o ./obj/print_wd.o ./obj/ech.o ./obj/cdir.o ./obj/ls.o ./obj/history.o ./obj/fgbg.o ./obj/pinfo.o ./obj/discover.o shell.c -o shell -g


