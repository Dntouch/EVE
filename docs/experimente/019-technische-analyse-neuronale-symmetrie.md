# Technische Analyse – neuronale Symmetrie in B.4.7

## Gemeinsame Versuchsarchitektur

Alle kausalen B.4.7-Gruppen verwenden dasselbe Netz mit 32 Neuronen, Schwelle
2, Gewichtslimit 3, `active_phase`-Eligibility, G3-Spuren, ternärer lokaler
Modulation, inputabhängiger Erholung und aktivierter lokaler Batch-Integration.
Wachstum und globale Handlungswahl bleiben deaktiviert.

Die logischen Rollen sind:

| Rolle | normale IDs | permutierte IDs | Verbindungen |
|---|---|---|---|
| Sensor X/Y | 0/1 | 17/19 | je Gewicht 0 zu beiden Entscheidungsneuronen |
| Exploration A/B | 2/3 | 21/23 | geschützt, Gewicht 2 zum zugehörigen Entscheidungsneuron |
| Entscheidung A/B | 4/5 | 25/27 | erregend zum eigenen und hemmend zum konkurrierenden Motor |
| Motor A/B | 6/7 | 29/31 | ausschließlich binär beobachtet |
| Modulation | 10–14 | 0–4 | fünf gleichwertige lokale Kontakte je Ziel, Gewicht 0 |

Die zwei inhibitorischen Gegenverbindungen sind strukturell symmetrisch. Ihre
Wirkung, Plastizität und Startstärke unterscheiden ausschließlich die
vorab definierten I0–I6-Gruppen.

## Ereignisse und Zufall

Die historische B.4.6-Reproduktion erzeugt Reiz- und Explorationspläne aus
`0x4601000 + seed` und `0x4602000 + seed`. Obwohl zwei RNG-Instanzen verwendet
werden, ist ihre binäre Ausgabe technisch gekoppelt: Alle 64 Seeds liefern in
128 Episoden exakt 67 Übereinstimmungen von Reiz- und normaler Zielhandlung
gegen 61 inverse; im frühen Drittel stets 24 gegen 18. Die Seedvariation
verändert diese Zuordnung nicht.

B.4.7 behält diesen Strom ausschließlich für exakte historische Referenzen.
Die einzelne Phase-G-Korrektur mischt die beiden Seedfamilien vor der
Initialisierung getrennt nichtlinear. Danach variieren normale Treffer je Seed
zwischen 48 und 80 bei Mittel 64,03. Der Runtime-Strom verwendet weiterhin
`0x4703000 + seed`. Rückmeldung wird als skalares `-1`, `0` oder `+1` über
alle fünf Modulationseingänge gesendet. Sie enthält weder Reiz-ID noch
Zielmotor.

Technisch gleichzeitige Eingänge werden je Empfänger atomar nach kanonischer
Synapsen-ID integriert. 64 direkte Gegenproben mit vertauschter Eingabeliste
erzeugen denselben Feuerentscheid, Membranwert und vollständigen
Zustandsfingerabdruck.

## Symmetrieplan

Die 15 Kombinationen aus Sensor-, Motor-, Explorations- und
Modulationsvertauschung sowie separate Neuronen- und Synapsen-ID-Permutationen
werden gegen denselben logischen Ausgangszustand geprüft. Nach Rückabbildung
der Rollen sind 1.088/1.088 Seed-Varianten exakt gleich; maximale
seedbezogene Erfolgsabweichung ist 0. Der vorab definierte
Fünf-Prozentpunkte-Äquivalenzbereich wird damit deutlich unterschritten.

Queue-Reihenfolge wird separat an identischen gleichzeitigen erregenden und
hemmenden Eingängen geprüft. Konstruktionsreihenfolge wird durch umgekehrte
Synapsenanlage abgedeckt. Keine dieser Kontrollen erzeugt oder zerstört eine
Fähigkeit.

## Modulatorische Gleichwertigkeit

Normaler und vollständig vertauschter Modulationsaufbau besitzen je fünf
Eingänge, fünf Verbindungen je Ziel, Startgewicht 0 und sofortige Ankunft zur
aktuellen simulierten Zeit. Beide erzeugen im Mittel 101,547 positive und
90,859 negative Änderungen und sind in 64/64 Seeds logisch exakt gleich.

## B.4.5, B.4.6 und kontrollierte Kausalität

B.4.5 und B.4.6 unterscheiden sich unter anderem in Netzgröße, expliziter
Hemmung und Batch-Verarbeitung. Ihre historischen Erfolgsquoten sind daher
nur Kontext. B.4.7 reproduziert B.4.6 unverändert, isoliert Hemmung aber in
I0–I6 innerhalb einer einzigen Umgebung. Netzgröße, Exploration, Erholung,
Rückmeldung, Aufgabenstrom und Seed bleiben bei diesen Vergleichen konstant.

I4 und I5 entstehen aus Kopien desselben trainierten Zustands. I4 friert
gelernte Gewichte ein; I5 deaktiviert anschließend ausschließlich die
inhibitorische Wirkung. Weitere Kopien setzen gelernte Reizgewichte zurück
oder vertauschen die erlernten Ausgangspfade. Der Trainingszustand selbst wird
dabei nie verändert.

## Persistenz und Grenzen

Alle 64 gespeicherten Zustände werden bitgenau wiederhergestellt und
deterministisch fortgesetzt. Sämtliche Netze bleiben gültig, Gewichte bleiben
in `[0,3]`, es gibt keine technischen Abbrüche und keine strukturell erzeugten
Synapsen. Die unveränderten 421 B.4.6-Tests sowie 18 neue Symmetrie- und
Zufallsstromtests
bestehen vollständig.
