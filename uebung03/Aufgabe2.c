#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <stdlib.h>

/*
shmget -> shared memory get // Es wird ein gemeinsames Speichersegment für den Prozess zur Verfügung gestellt.
shmat -> shared memory atach to // Ein bestimmetes Speichersegment wird in den Adressraum des Prozesses eingebunden.
                                    Mit Hilfe eines Zeigers kann nun auf diesem gemeinsamen Speicher zugegriffen werden.
shmdt -> shared memory detach // Koppelt aus dem Adressraum des Prozesses einen gemeinsamen Speichersegment aus.
                                  Von nun an kann dieser Prozess nicht mehr auf diesem Speichersegment zugreifen.
shmctl -> shared memory control // Aendert den Status eines gemeinsamen Speichers oder wird zur Statusabfrage
                                    eines gemeinsamen Speichers verwendet.
*/


/*
Das Programm printet bei mir jedesmal zuerst "Kindprozess: 2" bevor es "Elternprozess: 1" printet
Dies bedeutet, dass zwar *shared_mem += 1 von Elternprozess vor Kindprozess aufgerufen wird, jedoch der
scheduler die printf vom Kindprozess vor dem printf von dem Elternprozess bevorzugt aufruft.
Insgesamt werden die Kindrpozess printf s vor den printf s von dem Elternprozess ausgegeben, da
nach dem printf des Kindprozesses nur ein sleep von 0-2 sekunden ausgeführt wird, bei dem
sleep des Elternprozess jedoch jedesmal 2 sekunden gesleept wird.
*/
int main(){
  int i, shmID, *shared_mem, count=0, total=0,rnd;
  shmID = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0644); //shmget -> shared memory get (erstellt ein segment des gem. speichers)
  shared_mem = (int*)shmat(shmID,0,0);  //shmat -> shared memory attach to (abonniere dieses segment)
  *shared_mem = 0;
  if (fork())
    for (i=0; i<500; i++){
      *shared_mem+=1;
      printf("\n Elternprozess: %i", *shared_mem);
      sleep(2);
    }
  else
    for (i=0; i<500;i++){
      *shared_mem+=1;
      printf("\n Kindprozess: %i", *shared_mem);
      rnd=rand();
      sleep(rnd%3);
    }
    shmdt(shared_mem); //shmdt -> sared memory detach (von dem gemeinsamen speicher trennen)
    shmctl(shmID, IPC_RMID, 0); //shmctl -> shared memory control (aendert die eigenschaften meines segments)
return 0;
}
