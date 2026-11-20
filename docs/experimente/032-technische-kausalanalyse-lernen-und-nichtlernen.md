# Technische Kausalanalyse – Lernen und Nichtlernen B.5.8

## Gegenstand

B.5.8 trennt neuronale Aktivität, Erfahrung, Feedback, Eligibility und
tatsächliche synaptische Veränderung. Die Forschungsstufe prüft insbesondere,
ob vorhandene Fähigkeiten ohne fortwährende Gewichtsschreiboperationen
verfügbar bleiben und ob dieselben Netze nach einer Umweltänderung erneut
lernen können.

Die Analyse führt keine neue Produktions-Lernregel ein. S4 bleibt
standardmäßig deaktiviert.

## Technisches Audit

EVE besitzt zwei voneinander getrennte Plastizitätswege:

1. Die ältere aktivitätsabhängige Verstärkung wird durch
   `learning_enabled` gesteuert. Die B.5.8-Referenz verwendet hier weiterhin
   `false`.
2. Die Eligibility-basierte Drei-Faktor-Plastizität wird durch lokal
   eintreffende Modulation ausgelöst und bleibt in der Forschungsreferenz
   aktiv.

Neutrale Modulation wird bereits über die konfigurierte neuronale
Modulationsroute übertragen. Sie verändert keine Gewichte und verbraucht die
Eligibility nicht. Fehlendes Feedback erzeugt dagegen überhaupt kein
Modulationsereignis. Diese beiden Kontrollen sind technisch verschieden.

## Experimentelle Gewichtsschreibsperre

Für P1, P3 und F5 wurde
`experimental_plasticity_writes_suppressed` ergänzt. Die Schnittstelle ist
standardmäßig `false`, ausschließlich diagnostisch und bewirkt:

- sensorische, inhibitorische und motorische Aktivität laufen unverändert,
- Eligibility wird normal erzeugt, übertragen, verbraucht oder zeitlich
  ungültig,
- positive, negative und neutrale Modulation laufen über dieselbe neuronale
  Route,
- nur die tatsächliche Änderung eines Synapsengewichts wird unterdrückt,
- unterdrückte Schreibversuche werden separat gezählt,
- geschützte Synapsen, Gewichtsgrenzen und Budgets behalten ihre vorhandene
  Semantik.

Die Sperre wirkt bei Bedarf auch auf die ältere aktivitätsabhängige
Verstärkung. Das ändert die B.5.8-Referenz nicht, weil dieser Plastizitätsweg
dort bereits deaktiviert ist.

Das Persistenzformat wurde auf Version 15 erweitert. Gate und Zähler werden
gespeichert, in den Zustandsfingerabdruck aufgenommen und nach Laden exakt
fortgesetzt. Eine tatsächlich ausstehende Modulation unter aktiver Sperre
unterdrückte nach dem Laden drei Schreibversuche; Laden und Fortsetzung waren
fingerprint-identisch.

## Versuchsanordnung

### Zentrale Matrizen

Jede zentrale Matrix verwendet drei unabhängige Stromfamilien, normale und
inverse Zuordnung sowie 64 Seeds je Zelle.

| Phase | Seeds | Netze | Episoden je Netz |
|---|---:|---:|---:|
| B.5.7 Discovery-Reproduktion | 30001–30064 | 384 | 512 |
| B.5.7 Bestätigungs-Reproduktion | 40001–40064 | 384 | 512 |
| B.5.8 Discovery | 50001–50064 | 384 | 512 |
| B.5.8 unabhängige Bestätigung | 60001–60064 | 384 | 512 |

Zusätzlich wurden Fest-1, Fest-12, Fest-32 und unregelmäßige Abstände sowie
R2-50, S5-aus und No-Learning geprüft. Eine echte Überlappung gleichzeitig
bewerteter Episoden war nicht Teil von B.5.8; Fest-1 ist eine sehr kurze, aber
weiterhin getrennt bewertete Folge.

### Stabilitätsstichprobe

Aus Discovery und Bestätigung wurden je Stromfamilie, Zuordnung und verfügbarer
Population die ersten vier Netze ausgewählt. Das ergab:

- 48 robuste Ausgangsnetze,
- 48 zeitweise gemeinsam korrekte, nichtrobuste Netze,
- keine Netze in der vorab definierten schmalen Kategorie `near_robust`
  (beide E1-Werte mindestens 0,625, aber nicht beide mindestens 0,75).

Alle 96 Zustände besaßen am Ausgangspunkt beide Beziehungen. P0 verarbeitete
512 weitere Erfahrungen mit normaler Plastizität, P1 dieselben Ströme bei
gesperrten Gewichtsschreibungen. Danach folgten 256 umgekehrt bewertete
Erfahrungen:

- P2: durchgehend aktive Plastizität,
- P3: weiterhin gesperrte Gewichtsschreibungen,
- P4: nach der stabilen Sperrphase wieder aktivierte Plastizität.

## Reproduktion und Zeitabhängigkeit

| Variante | robuste Netze | Gesamt |
|---|---:|---:|
| B.5.7 Discovery-Reproduktion | 242 | 384 |
| B.5.7 Bestätigungs-Reproduktion | 254 | 384 |
| B.5.8 Discovery, Fest-16 | 240 | 384 |
| B.5.8 Bestätigung, Fest-16 | 244 | 384 |
| Fest-1 | 0 | 384 |
| Fest-12 | 0 | 384 |
| Fest-32 | 259 | 384 |
| unregelmäßig | 156 | 384 |
| R2-50 ohne S4 | 73 | 384 |
| S5, S4 deaktiviert | 73 | 384 |
| No-Learning | 0 | 384 |

Die B.5.7-Referenz wurde exakt reproduziert. Die seed-geclusterte robuste Rate
beträgt für B.5.8 Discovery 0,625 (95-%-Bootstrapintervall
0,573–0,672) und für die unabhängige Bestätigung 0,635
(0,594–0,680). Keine Stromfamilie-/Zuordnungszelle erreicht 48/64 robuste
Seeds.

## Fähigkeiten ohne weitere Gewichtsschreibungen

Über 49.152 Erfahrungen je Kopie ergaben sich:

| Kopie | positive Änderungen | negative Änderungen | unterdrückte Schreibversuche | Erfahrungen ohne Änderung |
|---|---:|---:|---:|---:|
| P0 aktiv | 19.350 | 19.351 | 0 | 26.013 |
| P1 gesperrt | 0 | 0 | 27.777 | 49.152 |

Nach 512 Erfahrungen:

| Kopie | E1 X | E1 Y | gleichzeitig robust |
|---|---:|---:|---:|
| P0 aktiv | 85,48 % | 79,17 % | 62/96 |
| P1 gesperrt | 95,83 % | 97,92 % | 90/96 |

Der gepaarte Vorteil von P1 gegenüber P0 im mittleren E1 beträgt 0,116
(seed-geclustertes 95-%-Intervall 0,031–0,183). Vorhandene Struktur kann die
untersuchte Fähigkeit somit ohne Dauerplastizität bereitstellen. Das ist kein
Nachweis, dass Gewichtsschreibungen allgemein abgeschaltet werden sollten.

## Erneute Lernfähigkeit

Nach 256 umgekehrt bewerteten Erfahrungen:

| Kopie | E1 X | E1 Y | gleichzeitig robust |
|---|---:|---:|---:|
| P2 durchgehend aktiv | 82,19 % | 80,21 % | 60/96 |
| P3 weiterhin gesperrt | 0,00 % | 0,00 % | 0/96 |
| P4 reaktiviert | 79,10 % | 84,38 % | 61/96 |

P2 übertrifft P3 gepaart um 0,827 (0,769–0,881), P4 übertrifft P3 um
0,818 (0,753–0,874). P4 und P2 unterscheiden sich nicht abgesichert:
-0,009 (-0,091–0,075). Eine diagnostische Sperrphase zerstört die spätere
Lernfähigkeit also nicht, sofern die Plastizität wieder aktiviert wird.

## Fehlende Handlung und Feedback

In 96 präregistrierten Nichtantwortfällen wurden identische Zustandskopien
verglichen:

| Kopie | Modulationsimpulse | mittlere negative Änderungen | X korrekt | Y korrekt |
|---|---:|---:|---:|---:|
| F0 natürlich | 20 | 2,52 | 64,58 % | 66,67 % |
| F1 kein Feedback | 0 | 0 | 34,38 % | 41,67 % |
| F2 neutral | 20 | 0 | 34,38 % | 41,67 % |
| F3 ein Eligibility-Eintrag | 20 | 1,25 | 50,00 % | 52,08 % |
| F4 um 4 verzögert | 20 | 2,52 | 64,58 % | 66,67 % |
| F5 Schreibsperre | 20 | 0, 2,52 unterdrückt | 34,38 % | 41,67 % |

F1, F2 und F5 besitzen im gemessenen Folgepunkt dieselbe Abrufrate, sind aber
technisch nicht gleich: Nur F2/F5 übertragen Modulation; nur F5 verbraucht die
Spur wie natürliche negative Modulation. Bis zum Ende des 16er-Abstands sind
auch die nicht verbrauchten Spuren zeitlich abgelaufen.

Negative Modulation bleibt funktional notwendig: F0 ist deutlich besser als
F5. Eine Nichtantwort begründet dennoch keine konkrete sensorische
Abschwächung. In Zustandskopien reparierten weder isolierte Hemmungsentfernung,
Membran-Nullsetzung noch deren Kombination einen der 96 untersuchten
M3-Ausgangszustände unmittelbar.

## Notwendige, neutrale und schädliche Änderungen

Für die ersten 64 stabilen Erfahrungen je Kopie wurden 35.391 lokale
Kandidatenbeobachtungen mit autonomen Vorher-/Nachher-Proben verbunden:

| Polarität | Beobachtungen | unmittelbar nützlich | unmittelbar schädlich | unmittelbar neutral |
|---|---:|---:|---:|---:|
| negativ | 3.786 | 1.318 | 0 | 2.468 |
| positiv | 31.605 | 1.362 | 3.175 | 27.068 |

Die Neutral-Klasse sagt nur aus, dass sich die zwei festgelegten unmittelbaren
Abrufprüfungen nicht änderten. Langfristige Nutzlosigkeit folgt daraus nicht.

Positive Konkurrenz ist kausal relevant: Positive Änderungen besitzen in
3.175 Beobachtungen einen unmittelbaren Verlust gegenüber 1.362 unmittelbaren
Verbesserungen. Pauschales Unterdrücken positiver Verstärkung ist daraus nicht
zulässig.

## Lokale Informationsgrenze

Der lokale Signaturvergleich enthält Polarität, Beitragsrichtung,
Synapsenwirkung, Ausgangsgewicht, Eligibility-Stärke und -Umfang,
Membranpotenzial, lokalen Aktivitätszähler, Eligibility-Alter und Anzahl
aktiver Einträge. 20 von 29 beobachteten Signaturen traten mit mehr als einer
globalen Funktionsklasse auf.

Damit ist keine sichere neue lokale Regel aus den untersuchten Zuständen
ableitbar. Der Befund ist eine nachgewiesene Mehrdeutigkeit des erfassten
Inventars, kein mathematischer Beweis, dass jede denkbare zusätzliche lokale
Information unzureichend wäre.

## Zwölf historische Fälle

Alle zwölf offenen B.5.6-Fälle wurden vom letzten gemeinsamen Erfolgszustand
bis Episode 512 mit natürlicher Plastizität, vollständiger Schreibsperre sowie
getrennt unterdrückter positiver und negativer Plastizität fortgesetzt. Nur ein
Fall enthielt nach diesem sehr späten Ausgangspunkt noch unterdrückbare
Schreibversuche. Keine Variante änderte die abschließende Beziehungsklasse.

Die Fälle bleiben deshalb offen. B.5.8 ordnet sie nicht nachträglich einer
unbewiesenen Ursache zu.

## Ressourcen und Validität

- 4.224 zentrale Netze,
- 96 gepaarte Stabilitätsquellen,
- rund 32 CPU-Sekunden,
- 27 Ereignisse maximale Queuegröße,
- 25.120 Byte größter geschätzter Runtime-Zustand,
- Persistenzversion 15,
- 0 ungültige Fälle,
- 665/665 Kerntests bestanden.

Die großen Tabellen `network-classifications`, `stable-experiences` und
`plasticity-events` werden deterministisch mit gzip gespeichert.

## Technische Entscheidung

Die Gewichtsschreibsperre bleibt eine standardmäßig deaktivierte
Diagnoseschnittstelle. Sie ist kein Mechanismus, mit dem EVE global erkennt,
dass eine Fähigkeit gelernt sei. K18/K19 und eine neue lokale Lernregel werden
nicht implementiert.
