#define PRODUCTEUR int c;  while((c = getchar()) != EOF){ push(c); }

#define CONSOMMATEUR while(1) { putchar(pop()); fflush(stdout); }
