 CC=gcc

program: program.c database kaydet
	$(CC) -o program program.c
database: database.c  
	$(CC) -o database database.c
kaydet: kaydet.c  
	$(CC) -o kaydet kaydet.c
