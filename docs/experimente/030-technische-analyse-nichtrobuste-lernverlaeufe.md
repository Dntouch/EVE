# Technische Kausalanalyse – nichtrobuste Lernverläufe B.5.6

## Untersuchungsstand

B.5.6 verändert keine Produktionsregel. Der Runner bindet den Stand B.5.5 ein,
verwendet S4-d16 ausschließlich experimentell und referenziert die unveränderten
B.5.5-Rohdaten über `research/results/b56/reference-manifest.csv`. Frühere
Ergebnisbäume werden nicht kopiert.

Die neue Datenbasis umfasst:

- 384 historische Entdeckungsnetze (Seeds 1–64),
- 384 historische Bestätigungsnetze (Seeds 10001–10064),
- 384 frische Bestätigungsnetze (Seeds 20001–20064),
- 9.216 frische Zeitnetzwerke für drei Varianten und acht Zeitpläne,
- 384 No-Learning-Kontrollen,
- 1.152 frische Umkehrläufe,
- 144 präregistriert ausgewählte Detailbiografien,
- 72 nichtrobuste Netze mit Kausalinterventionen.

Alle Gruppen enthalten drei unabhängige Zufallsfamilien und normale sowie
inverse Zielzuordnung. Alle 11.904 primären Netzläufe waren gültig. Der Lauf
benötigte 90,97 Sekunden und höchstens 11.344 KiB RSS.

## Klassifikation

Nichtrobust bedeutet weiterhin `E1_X < 0,75` oder `E1_Y < 0,75`. Die Klassen
N1–N8 sind absichtlich überlappend. In der frischen Bestätigung waren 236 von
384 Netzen robust und 148 nichtrobust:

| Klasse | Bedeutung | Nichtrobuste Netze |
|---|---|---:|
| N1 | X unter, Y über Schwelle | 82 |
| N2 | Y unter, X über Schwelle | 66 |
| N3 | beide unter Schwelle | 0 |
| N4 | mindestens eine Beziehung nie erworben | 0 |
| N5 | beide erworben, später mindestens eine momentan verloren | 145 |
| N6 | zeitweise korrekt, aber mit Verlust/Reakquisition | 148 |
| N7 | momentaner Abruf korrekt, E1 nicht robust | 3 |
| N8 | E1 robust, natürliche gemeinsame Verfügbarkeit unter 0,50 | 0 in der nichtrobusten Population; 8 insgesamt |

Damit ist unvollständiger Erwerb für die bestätigte Restpopulation keine
Erklärung. Alle 148 nichtrobusten Netze hatten beide Beziehungen autonom
verfügbar; das Problem ist ihre spätere synaptische Stabilität und nicht die
prinzipielle Erreichbarkeit.

## Kausalinterventionen

Für jede Phase, Familie und Zuordnung wurden vor Kenntnis der Detailergebnisse
die ersten vier robusten und ersten vier nichtrobusten Seeds ausgewählt. Ein
historischer Zustand wurde vor einem transienten Vergleich ereignisgetrieben
auf den Endzeitpunkt ausgerichtet. Vergangene Queue-Termine werden deshalb nie
in einen aktuellen Zustand transplantiert. Zeit und Queue selbst sind kein
isolierbarer Faktor.

Ergebnis der 72 nichtrobusten Detailnetze:

- Gemeinsame Rücknahme aller vier sensorischen Gewichte repariert 57 Fälle
  (79,17 %).
- Einsetzen der aktuellen sensorischen Gewichte in den früher erfolgreichen
  Zustand zerstört in 60 Fällen die gemeinsame Fähigkeit (83,33 %).
- Einzelne sensorische Pfade sind in 11, 14, 20 beziehungsweise 15 Fällen
  hinreichend oder notwendig. Es gibt daher keinen universellen einzelnen
  Problemkanal.
- Isolierte inhibitorische Gewichte reparieren oder zerstören keinen Fall.
- Isolierte aktuelle Membranwerte zerstören drei frühere Fähigkeiten, aber die
  Rücknahme alter Membranen repariert keinen Endzustand.
- Eligibility, G3, synaptische Aktivität, Recovery/S4 und Kontext reparieren
  isoliert keinen Endzustand.
- Die acht S/I/T-Faktorkopien sind in allen 576 Fällen gültig. 57 Fälle sind
  primär sensorisch, drei benötigen eine Kombination und zwölf bleiben offen.

Die drei transient empfindlichen Fälle zeigen, dass momentane Erregung einzelne
Abrufe stören kann. Sie erklärt jedoch nicht den bestätigten Populationsrest.
Die inhibitorische Konkurrenz beeinflusst natürliche Episoden, ist aber hier
kein isolierter Endzustandsverursacher.

## Erfahrungen und frühe Entwicklung

Die drei Fortsetzungen beginnen jeweils mit exakt demselben Zustand nach
Episode 128. Nur der unabhängige Cue- oder Explorationsstrom wird geändert; es
werden keine richtigen Erfahrungen erzwungen.

Bei den 72 ursprünglich nichtrobusten Detailnetzen erreichen nach Episode 256:

| Fortsetzung | E1 X | E1 Y | robust |
|---|---:|---:|---:|
| ursprünglicher Strom | 0,8881 | 0,7492 | 46 |
| unabhängiger Cue-Strom | 0,8194 | 0,8750 | 50 |
| unabhängiger Explorationsstrom | 0,8325 | 0,7500 | 42 |

Erfahrung beeinflusst das Ergebnis kausal, aber keine alternative Folge macht
alle Netze robust. Die frühe Entwicklung bestimmt, welche sensorischen Pfade
später verstärkt oder geschwächt werden.

## Episodenraster und Zeitgrenzen

S2 und S5 sind in allen Zeitplänen exakt gleich. S4-d16 ist stark
rasterabhängig:

| Zeitplan | robuste S2/S5 | robuste S4 | Δ gemeinsames E1 (S4−S2) | 95-%-Bootstrapintervall |
|---|---:|---:|---:|---:|
| fest 8 | 0 | 0 | −0,0209 | [−0,0468; 0,0035] |
| fest 12 | 0 | 0 | −0,0518 | [−0,0774; −0,0294] |
| fest 16 | 87 | 236 | 0,0119 | [−0,0510; 0,0733] |
| fest 20 | 87 | 236 | 0,0119 | [−0,0510; 0,0733] |
| fest 24 | 87 | 236 | 0,0119 | [−0,0510; 0,0733] |
| fest 32 | 87 | 273 | 0,1089 | [0,0512; 0,1669] |
| unregelmäßig | 68 | 163 | −0,0123 | [−0,0523; 0,0272] |
| kontinuierlich | 18 | 35 | −0,0179 | [−0,0510; 0,0149] |

S4 erhöht bei unregelmäßigen und kontinuierlichen Folgen die Zahl der Netze
oberhalb der harten 0,75-Schwelle, aber nicht den mittleren gemeinsamen E1-Wert
mit abgesicherter positiver Differenz. Bei Abstand 12 ist S4 signifikant
schädlich, bei Abstand 32 signifikant nützlich. Ein allgemeiner,
rasterunabhängiger Lernvorteil ist damit widerlegt.

Die Grenzprobe bestätigt die Ereignisordnung: bei `t=15` integriert der zweite
Eingang noch und feuert; bei `t=16` wird S4 vor dem neuen Eingang verarbeitet,
so dass kein Feuer entsteht. Dasselbe gilt nach der Grenze bei `t=17`.

## E1, Retention und Umkehr

Die Checkpoints 128, 256, 384 und 512 verwenden unabhängige E1- und E2-Kopien.
Die drei N7-Fälle belegen, dass ein momentaner korrekter Doppelabruf keine
robuste E1-Fähigkeit garantiert. Umgekehrt zeigen N8-Fälle, dass robuste E1
trotz geringer natürlicher gemeinsamer Verfügbarkeit möglich ist.

Zeitfreie Retention verändert die Endwerte praktisch nicht. In der frischen
Umkehrmatrix sind 220 S4-Netze robust, gegenüber 78 in S2 und 78 in S5. S4
hilft also auch unter Umkehrbedingungen, ohne die Rasterabhängigkeit zu
entkräften.

## Technische Gültigkeit

- 611/611 Tests bestehen (588 historische + 23 B.5.6-Tests).
- 64/64 Persistenzläufe restaurieren und setzen exakt fort.
- Alle Gewichts-, Transient- und Faktorkopien sind gültig.
- No-Learning ergibt E1 X = 0 und E1 Y = 0 in allen 384 Netzen.
- S4 bleibt in der Produktionskonfiguration deaktiviert.
- Es wurde keine Lern-, Hemmungs- oder Gedächtnisregel ergänzt.

## Technische Folgerung

Der lokal beobachtbare Ansatzpunkt ist nicht eine pauschale Membran- oder
Hemmungsregel, sondern der konkrete sensorische Gewichtsverlust nach bereits
erfolgreichem Erwerb. B.5.7 soll die lokalen Eligibility-, Modulations- und
Konkurrenzereignisse unmittelbar vor diesen Gewichtsverlusten kausal zerlegen.
Eine Schutzregel oder automatische Wiederherstellung ist durch B.5.6 nicht
freigegeben.
