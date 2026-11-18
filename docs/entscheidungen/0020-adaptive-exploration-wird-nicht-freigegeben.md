# Entscheidung 0020: Adaptive Exploration wird nicht freigegeben

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidung 0019

## Kontext

B.4.8 reproduziert 128/128 entkoppelte B.4.7-Referenzen exakt und prüft drei
direkt unabhängige Erfahrungsstromfamilien. Autonome Reizbindung entsteht
bereits nach 4–8 Erfahrungen, bleibt aber instabil. Das Rücksetzen der vier
Reizgewichte beseitigt E1 vollständig. Das Einfrieren früher Gewichte
behindert spätere Korrektur. Eine eindeutige falsche autonome Antwort
unterdrückt zugleich die bisherige Exploration, obwohl beide motorischen Wege
strukturell erhalten bleiben.

Die einzige adaptive Forschungsvariante setzt nach negativer allgemeiner
Rückmeldung ein lokales Bereitschaftsbit und wählt die zusätzliche Handlung
aus einem unabhängigen allgemeinen Explorationsstrom. Sie kennt weder Reiz
noch Zielhandlung. Die Variante verbessert eine Entdeckungsgruppe leicht,
reproduziert diesen Effekt aber nicht über Stromfamilien und Bestätigungs-Seeds.
Eine gleich häufige zeitlich umverteilte Kontrolle ist mehrfach besser.

Keine adaptive Gruppe erreicht 75 % E1 für beide Reize oder 48/64 robuste
Seeds. Umkehrlernen bleibt unzureichend. 459/459 Tests und 384/384
Persistenzproben bestehen; der negative Befund ist kein technischer Abbruch.

## Entscheidung

Die rückmeldungsabhängige adaptive Exploration wird weder als allgemeine
angeborene Stammhirnfunktion noch als Produktionsmechanismus freigegeben. Sie
bleibt ausschließlich dokumentierte Forschungsvariante. Mehr zufällige
Motoraktivität gilt nicht als Ersatz für autonome neuronale Bindung.

Als gesichert gelten:

- frühe Erfahrungen verändern autonome Antworten schon nach 4–8 Episoden,
- weitere Erfahrung stabilisiert diese Antworten nicht monoton,
- gelernte Reizgewichte tragen E1 kausal,
- falsche autonome Ausgaben blockieren die bisherige geschützte Exploration,
- negative Modulation allein stellt keine alternative Handlung bereit,
- der Zeitpunkt zusätzlicher Exploration ist mindestens so wichtig wie ihre
  bloße Anzahl,
- Retention und Persistenz funktionieren exakt, speichern aber auch
  unzureichende oder einseitige Fähigkeiten.

## Auftrag für B.4.9

B.4.9 darf keinen breiten Explorationsmechanismus voraussetzen. Zuerst ist zu
prüfen, ob eine rein lokale, ereignisgebundene Alternative nach einer
fehlbewerteten autonomen Entscheidung entstehen kann, ohne Zielmotorwissen,
Gewichtsreset oder globale Auswahl. Primär bleibt E1; E2 und zusätzliche
Aktivität sind nur Diagnostik. Jede Variante muss erneut mehrere unabhängige
Stromfamilien, normale/inverse Rollen, No-Learning, Timingkontrollen,
Retention und Umkehrlernen bestehen.
