KeySync

Ein Softskills-Projekt der Carl von Ossietzky Universität Oldenburg.

Wir haben im Rahmen des Moduls "Softskills und Technische Kompetenzen" einen smarten Schlüsselkassten entwickelt, der die Vermietung von Räumen vereinfachen soll.

Wichtig: Dies ist ein Prototyp und sollte auf keinen Fall in seiner jetztigen Form tatsächlich verwendet werden.

Das Projekt besteht aus zwei Komponenten, einem Mainboard/Hub und einer beliebigen Anzahl an Schlüsselkästen. 
Auf dem Mainboard werden auf Anfrage Zahlencodes generiert, die dann am entsprechenden Schlüsselkasten eingegeben werden können um den Schlüssel freizugeben.

Betrieben wird das System von mehreren ESP8266 und nutzt das ESPnow protocoll zur Komunikation. Die Gehäuse wurden mit einem Lasercutter hergestellt.

Die Nutzung ist simpel genug:
  1. Ein Raum wird bei einer Person (z.B. Raumverwaltung oder Rezeption) angefordert.
  2. Durch Knopfdruck am Mainboard wird ein 4 stelliger Pin generiert und kann an die ensprechende Person weiter geleitet werden.
  3. Bei eingabe des Pins öffnet sich der Deckel und der Schlüssel kann entnommen werden
  4. Wenn der richtige Schlüssel wieder in den Kassten gelegt wird schließt sich der Deckel automatisch.

     Sowohl auf dem Mainbard, als auch auf dem Schlüsselkasten wird der Raumstatus mittels farbiger LEDs gekenzeichnet.
