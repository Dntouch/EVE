# Technische Analyse – G3 und Eligibility in B.4.3

## Bestehender Pfad

Jeder gewöhnliche synaptische Impuls wird in einem lokalen G3-Ring des
Empfängerneurons gespeichert, einschließlich Nullgewicht-Eingängen. Bei einer
Feuerentscheidung kopiert E1 die jüngsten eindeutigen Ringelemente in eine
zeitlich begrenzte Eligibility-Entscheidung. Modulation läuft über eine
neuronale Route und verändert nur gültige, nicht rückmeldungsinduzierte und
nicht geschützte Kandidaten.

G3 und Eligibility sind getrennte Speicher. Der Ring behält Ereignisse über
Feuerentscheidungen und sensorische Episoden hinweg; Eligibility besitzt eine
eigene Generation, Ablaufzeit, Kandidaten- und Entscheidungskapazität. R2F
ändert Membran/Refraktärzustand, nicht G3 oder Eligibility.

## Interferenzmechanismus

E1 kennt lokal nur zeitliche Nähe. Bei G3-Kapazität 8 können Ereignisse des
vorherigen Reizes noch im Ring liegen, wenn der nächste Motorweg feuert. Das
aktuelle Feedback erreicht dann gültige Kandidaten beider Erfahrungen. Eine
Verbindung kann deshalb in X verstärkt und später während Y wieder
abgeschwächt werden. Reizname und Episodennummer stehen korrekt nicht zur
Disambiguierung zur Verfügung.

Die B.4.3-Diagnose bestätigt dies: L1 enthielt 11.950 Fremdreiz-Kandidaten in
39.632 Kandidaten (30,15 %). Im sequenziellen X→Y-Lauf waren es 16,96 %, im
umgekehrten Lauf 16,89 %. Diese Werte stammen aus der technischen Diagnose;
sie fließen nicht in die Lernregel ein.

## Varianten

- L1 bleibt bytegleiches E1/G3.
- L2 (`pre_post_fire`) markiert nur den unmittelbar feuerauslösenden Eingang.
- L3 (`active_phase`) filtert G3 auf Eingänge mit lokaler Aktivität seit der
  letzten Feuerentscheidung. Nach der Entscheidung werden nur die lokalen
  Aktivitätszähler abgegrenzt; G3 bleibt für Diagnose und L1 unverändert.
- L4 (`membrane_participation`) bildet pro eingehendem Membranbeitrag eine
  lokale Spur. Mehrere Eingänge und Nullgewicht-Synapsen bleiben möglich.

`active_phase` ergänzt lediglich den vorhandenen Auswahl-Enum. Es speichert
weder Reizkennung noch Episodenzähler und benötigt keine synaptischen
Zeitstempel. Der bestehende Persistenzdatensatz enthält den Enumwert bereits;
es entsteht kein zusätzlicher Zustand pro Synapse oder Neuron.

## Ergebnis der Isolierung

L3 senkte den Fremdreizanteil auf 6,33 %, verbesserte das Verhalten aber nicht:
Der ausgeglichene Erfolg fiel von 39,03 % (L1) auf 36,20 %. L2 erzeugte keine
wirksamen Änderungen, weil im kontrollierten Aufbau der unmittelbar
feuerauslösende Explorationsweg geschützt war. L4 markierte zwar lokal, war
aber zu breit und endete bei 0,92 % ausgeglichenem Erfolg sowie fast nur
doppelten Ausgaben.

Damit ist H1/H3 als realer Mechanismus belegt, aber eine engere Auswahl allein
nicht hinreichend. Der verbleibende Engpass ist die lokale Zuordnung zwischen
sensorischem Beitrag und späterer postsynaptischer Entscheidung, ohne die
lernfähigen Nullgewicht-Wege auszuschließen.

