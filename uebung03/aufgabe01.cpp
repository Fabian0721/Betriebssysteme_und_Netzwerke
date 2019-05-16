#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string.h>

int main(void) {

  /*Erzeugung einer Pipe fd mit zwei Grundzuständen:
  fd[0] = filedeskriptor zum lesen und fd[1 = filedeskriptor zum schreiben
  nbytes = die Anzahl der zu lesenden Zeichen*/
  int fd[2], nbytes;

  //Variable um die Pid ( = Prozess Identifizierung) vom Childprozess zu spreichern
  pid_t childpid;

  //Erzeugung zweier Arrays
  char string[] = "Hello World\n";
  char readbuffer[80];

  //Initialisierung durch die Funktion Pipe
  pipe (fd);

  //Fehler abfangen und Kindprozess erstellen
  if((childpid = fork()) == -1) {
    //Druckt eine Systemfehlermeldung
    perror("fork");
    //Beendet Prozess und gibt 1, also Fehler zurück
    exit(1);
  }

  if(childpid == 0) {

    //Kindprozess schließt die input Seite der pipe
    close (fd[0]);

    //String spiegeln
    int len = strlen(string);
    char rev_string[len];
    for (int i = 0; i < len; i++) {
      rev_string[i] = string[len - i - 1];
    }
    for (int i = 0; i < len; i++) {
      string[i] = rev_string[i];
    }

    //fd[1] in dese Datei schreibt ein Prozess den String: string
    write (fd[1], string, (strlen(string)+1));

    //Beendet Prozess und gibt 0, also alles ok zurück
    exit(0);

  } else {

    //Elternprozess schließt die output Seite der pipe
    close (fd[1]);

    //fd[0] aus dieser Datei ließt ein Prozess
    nbytes = read (fd[0], readbuffer, sizeof(readbuffer));
    
    //Drucken des Ergebnisses
    printf ("Received string: %s", readbuffer);
    std::cout << std::endl;
  }
  return(0);
}
