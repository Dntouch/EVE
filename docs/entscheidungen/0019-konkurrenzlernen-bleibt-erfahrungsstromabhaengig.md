# Entscheidung 0019: Konkurrenzlernen bleibt erfahrungsstromabhängig

- Status: angenommen
- Datum: 18. November 2026
- Bezug: Entscheidung 0018

## Kontext

B.4.7 reproduziert B.4.6 und prüft 17 konsistente technische
Rollenabbildungen. Alle 1.088 gepaarten Seed-Abbildungen sind logisch exakt
gleich. Eine technische Bevorzugung bestimmter Sensor-, Motor-, Neuronen- oder
Synapsennummern wird nicht gefunden.

Der beste unveränderte Kandidat bleibt bei 83,85 % und 35/64 robusten Seeds;
die Bestätigung erreicht 81,64 % und 30/64. Seine autonome Antwort beträgt
nur 68,41 %. Eine inverse Zielzuordnung mit unverändertem Explorationsstrom
fällt auf 59,00 %. Wird ausschließlich die Handlungsrolle des
Explorationsstroms konsistent gespiegelt, entstehen exakt die normalen
Ergebnisse.

Die Zufallsstrominventur zeigt, dass alle 64 historischen Seeds exakt dieselbe
67:61-Kopplung zugunsten der normalen Zielrolle besitzen. Nach der isolierten
Seedentkopplung sinkt die normale Leistung auf 70,07 %, die inverse erreicht
73,78 %; Bestätigungen liegen bei 72,33 % und 68,77 %. Keine korrigierte
Gruppe überschreitet 16/64 robuste Seeds. Der historische Aufbau überschätzte
damit die allgemeine Fähigkeit.

Gelernte Hemmung wirkt im trainierten Zustand kausal, ist aber gegenüber
fester Hemmung weiterhin nicht statistisch überlegen. Umkehrlernen bleibt
instabil und verbessert sich durch längeres Training nicht monoton.

## Entscheidung

Das bestehende Konkurrenzlernen wird nicht allgemein freigegeben.
Inhibitorische Plastizität, lokale Batch-Integration und die B.4.6-Schaltung
bleiben explizite Forschungsfunktionen. Es werden weder Winner-take-all noch
globale Handlungswahl, semantische Zielzustände oder eine technisch
ausbalancierte richtige Aktion eingeführt.

Als gesichert gelten:

- konsistente technische Rollenabbildungen sind invariant,
- der historische binäre Zufallsplan ist technisch gekoppelt und darf nicht
  als unabhängige Seedvariation interpretiert werden,
- beide angeborenen Explorationswege sind strukturell erreichbar,
- gelernte Reizpfade und Hemmung tragen kausal zum Verhalten bei,
- konkrete frühe Explorationsrollen beeinflussen die spätere Fähigkeit stark,
- die bestehende lokale Regel kompensiert diese Abhängigkeit nicht robust.

## Auftrag für B.4.8

B.4.8 soll zunächst die Erfahrungsstromabhängigkeit weiter isolieren:

1. Die in B.4.7 eingeführte nichtlineare Seedtrennung wird zur methodischen
   Referenz; weitere Strompermutationen müssen unabhängig von niedrigen
   Zufallsbits sowie paarweise mit handlungskomplementierten Strömen geprüft
   werden.
2. Der Zeitpunkt, an dem zufällige Exploration zu dauerhafter Pfadbindung
   wird, ist durch frühzeitige Zustandskopien kausal einzugrenzen.
3. Erst danach darf eine einzelne lokale Forschungsvariante untersuchen, ob
   negative Rückmeldung oder lokale Neuheit die weiterhin erreichbare
   Handlungsalternative stärker explorieren kann.
4. Eine solche Variante darf weder Reizsemantik noch richtige Zielhandlung
   kennen und muss normale, inverse und Umkehraufgaben mit E1 und E2 getrennt
   bestehen.

Eine Freigabe bleibt an mindestens 48/64 robuste Seeds in Entdeckung und
Bestätigung, beide Zielzuordnungen, positive gepaarte Intervalle, Retention und
stromunabhängige Reproduktion gebunden.
