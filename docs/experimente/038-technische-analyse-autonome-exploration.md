# Technische Analyse – autonome interne Exploration in C.0

## Auslösung

Die neue Exploration ist kein Runner-Befehl. Ein tatsächlich verarbeitetes,
nichtmodulatorisches lokales Feuerereignis kann sie auslösen. Die Entscheidung
verwendet nur den Ausgangsgrad des auslösenden Neurons, den lokalen Radius,
das verbleibende Budget und einen getrennten deterministischen Zufallsstrom.

Das ausgewählte freie Neuron erhält ein eigenes
`internal_exploration_fire`-Ereignis. Dieses Feuer darf Growth auslösen, aber
nicht erneut Exploration. Damit entsteht keine selbstverstärkende
Explorationskaskade.

## Ausgeschlossene Information

Die Auswahl kennt weder Aufgabenlabel noch richtige Handlung, Zielpfad,
Bewertung oder spätere Erfolgsmetrik. Die zehn Aufgabenbezeichnungen verändern
nur den vorregistrierten Seedbereich, nicht die neuronale Auswahl.

## Beobachtung

- 102.400 autonome Aktivierungen in 12.800 zentralen Läufen,
- Budgetüberschreitung: 0,
- Schutzverletzungen: 0,
- maximale zentrale Queue blieb klein und ereignisgebunden,
- Zufallszustand, Restbudget und pending Trigger sind vollständig persistent,
- Fortsetzung nach Speichern/Laden ist fingerprint-identisch.

K2 bestätigt Exploration ohne Growth; K1 bestätigt Growth ohne autonome
Exploration. K0 bleibt vollständig statisch. Bei deaktiviertem C.0-Pfad bleibt
das historische Verhalten exakt erhalten. Die kombinierte Aktivierung von
Exploration und Growth verlangt dagegen zwingend einen vollständigen sicheren
Schutzvertrag; dieselbe Prüfung gilt bei späteren Umschaltungen und nach dem
Laden persistierter Zustände.
